#include "Queue.h"
#include "Teller.h"

Queue::Queue()
{
	m_numTeller = UDEDB.GetInt("Queue", "InitialTeller");
	m_countForFree = 0;
	m_tellerIsJustAdded = 0;
	m_bFired = false;
	for (int l1 = 0; l1 < m_numTeller; l1++) m_Teller[l1] = _StateTeller::FREE;

	AddInPort(0); // in
	for (int l1 = 0; l1 < m_numTeller; l1++) {
		AddInPort(l1 + 1); // ready1
		AddOutPort(l1 + 1); // out1
	}

	m_State = _State::NORMAL;

	SetName("Queue");
}

Queue::~Queue()
{

}

bool Queue::ExtTransFn(UDEMessage *_msg)
{
	if (_msg->GetPort() == 0) {
		m_Waiting.push_back((Customer *)(_msg->GetMsg()));

		//UDELOG_INFO << "Queuing customer(" << ((Customer *)(_msg->GetMsg()))->GetID() << ")" << " // Queue size : " << m_Waiting.size();

		if (UDEDB.GetFlag("Queue", "AddTeller") && m_tellerIsJustAdded-- < 0 && m_Waiting.size() > 15 && m_numTeller < UDEDB.GetInt("Queue", "MAXTeller")) {
			m_numTeller++;

			UDELOG_INFO << "Teller" << m_numTeller << " is assigned" << std::endl;

			AddInPort(m_numTeller);
			AddOutPort(m_numTeller);
			UDEModel *_newTeller = new Teller(m_numTeller);
			UDECModel *_BANK = (UDECModel *)(((UDEModel *)this)->GetParent());

			_BANK->AddComponent(_newTeller);
			_BANK->AddCoupling(this, m_numTeller, _newTeller, 0);
			_BANK->AddCoupling(_newTeller, Teller::_OutPort::READY, this, m_numTeller);
			_BANK->AddCoupling(_newTeller, Teller::_OutPort::DONE, _BANK, 0);

			UDEIF->ModelIsChanged();
			m_Teller[m_numTeller - 1] = _StateTeller::FREE;

			m_tellerIsJustAdded = 10;
		}
	}
	else {
		m_Teller[_msg->GetPort() - 1] = _StateTeller::FREE;
	}

	if (UDEDB.GetFlag("Queue", "RemoveTeller") && m_bFired && m_Teller[m_numTeller - 1] == _StateTeller::FREE) {
		UDECModel *_BANK = (UDECModel *)(((UDEModel *)this)->GetParent());

		char _tellerName[10];
		sprintf(_tellerName, "Teller%d", m_numTeller);

		UDELOG_INFO << _tellerName << " is dropped out";
		UDEModel *_teller = _BANK->FindComponent(_tellerName);

		_BANK->RemoveCoupling(this, m_numTeller, _teller, 0);
		_BANK->RemoveCoupling(_teller, Teller::_OutPort::READY, this, m_numTeller);
		_BANK->RemoveCoupling(_teller, Teller::_OutPort::DONE, _BANK, 0);

		_BANK->RemoveComponent(_teller);

		RemoveInPort(m_numTeller);
		RemoveOutPort(m_numTeller);

		m_numTeller--;

		UDEIF->ModelIsChanged();
		m_bFired = false;
	}

	if (m_State == _State::SENDING) Continue();
	else if (m_Waiting.size() != 0) {
		int l1;
		for (l1 = 0; l1 < m_numTeller; l1++) {
			if (m_Teller[l1] == _StateTeller::FREE) break;
		}
		if (l1 != m_numTeller) {
			m_targetTeller = l1 + 1;
			m_State = _State::SENDING; // NORMAL -> SENDING
		}
	}

	return true;
}

bool Queue::IntTransFn()
{
	if (m_State == _State::SENDING) {
		if (m_Waiting.size() == 0) {
			m_State = _State::NORMAL;
			return true;
		}
		int l1;
		for (l1 = 0; l1 < m_numTeller; l1++) {
			if (m_Teller[l1] == _StateTeller::FREE) break;
		}
		if (l1 != m_numTeller) {
			m_targetTeller = l1 + 1;
		}
		else {
			m_State = _State::NORMAL;
		}
	}

	return true;
}

bool Queue::OutputFn(UDEMessage *_msg)
{
	m_Teller[m_targetTeller - 1] = _StateTeller::BUSY;
	Customer *cust = m_Waiting.front(); m_Waiting.pop_front();
	_msg->SetPortValue(m_targetTeller, (void*)cust);

	if (m_Waiting.size() < 6 && m_numTeller != 1) m_countForFree++; else m_countForFree = 0;
	if (m_countForFree > 15) {
		m_countForFree = 0;
		m_bFired = true;
	}
	return true;
}

double Queue::TimeAdvanceFn()
{
	if (m_State == _State::NORMAL) return Infinity;
	else return UDEDB.GetDouble("Queue", "BufferSendingTime");
}
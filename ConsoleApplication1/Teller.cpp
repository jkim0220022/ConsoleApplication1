#include "Teller.h"

Teller::Teller()
{
	SetName("Teller1"); // test

	AddInPort(0);
	AddOutPort(_OutPort::DONE);
	AddOutPort(_OutPort::READY);

	m_State = FREE;
}

Teller::Teller(int num)
{
	char name[10];
	sprintf(name, "Teller%d", num);
	SetName(name);

	AddInPort(0);
	AddOutPort(_OutPort::DONE);
	AddOutPort(_OutPort::READY);

	m_State = FREE;
}

Teller::~Teller()
{

}

bool Teller::ExtTransFn(UDEMessage *_msg)
{
	cust = (Customer *)(_msg->GetMsg());
	m_State = _State::BUSY;
	//std::cout << std::fixed << std::setprecision(5) << UDEIF->GetCurrentTime() << " : Customer(" << cust->GetID() << ") in " << GetName() << std::endl;
	cust->SetWaiting(UDEIF->GetCurrentTime());
	return true;
}

bool Teller::IntTransFn()
{
	if (m_State == _State::BUSY) m_State = _State::FREE;
	return true;
}

bool Teller::OutputFn(UDEMessage *_msg)
{
	_msg->SetPortValue(_OutPort::DONE, (void*)cust);
	_msg->SetPortValue(_OutPort::READY, nullptr);

	return true;
}

double Teller::TimeAdvanceFn()
{
	if (m_State == _State::BUSY) {
		return cust->GetProcTime();
	}
	return Infinity;
}


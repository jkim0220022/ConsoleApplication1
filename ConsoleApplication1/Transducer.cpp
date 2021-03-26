#include "Transducer.h"
#include "Customer.h"

Transducer::Transducer()
{
	SetName("Transducer");
	AddInPort(0);  // Customer
	AddOutPort(0); // Stop

	m_count = 0;
	m_sumWaitTime = 0.0;
	m_State = _State::WAIT;
}

Transducer::~Transducer()
{

}

bool Transducer::ExtTransFn(UDEMessage *_msg)
{
	if (_msg->GetPort() != 0) return false;
	m_count++;
	Customer *aCust = (Customer *)(_msg->GetMsg());
	STDOUT << "Customer(" << aCust->GetID() << ") is serviced after " << aCust->GetWaitingTime() << "s of waiting time" << std::endl;

	m_sumWaitTime += aCust->GetWaitingTime();

	if (m_count == UDEDB.GetInt("Transducer", "MAXCustomer")) {
		m_State = _State::SEND;
		std::cout << "# of customers		= " << m_count << std::endl;
		std::cout << "Average waiting time	= " << m_sumWaitTime / m_count << std::endl;
		UDEDB.RegisterDouble("", "TransResult", m_sumWaitTime / m_count);
	}

	delete aCust;
	return true;
}

bool Transducer::IntTransFn()
{
	if (m_State == _State::SEND) m_State = _State::END;
	return true;
}

bool Transducer::OutputFn(UDEMessage *_msg)
{
	if (m_State == _State::SEND) _msg->SetPortValue(0, nullptr);
	return true;
}

double Transducer::TimeAdvanceFn()
{
	if (m_State == _State::SEND) return 0.0;
	return Infinity;
}


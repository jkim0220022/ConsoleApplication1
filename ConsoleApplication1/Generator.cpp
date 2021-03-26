#include "Generator.h"
#include "Customer.h"
#include <random>

Generator::Generator()
{
	SetName("Generator");
	m_State = _State::ACTIVE;

	m_count = 0;

	AddInPort(_InPort::STOP);
	AddOutPort(_OutPort::SEND);

	distProc = std::exponential_distribution<double>(UDEDB.GetDouble("Generator", "ProcTimeLambda"));
	distGen = std::exponential_distribution<double>(UDEDB.GetDouble("Generator", "IntervalLambda"));
}

Generator::~Generator()
{

}

bool Generator::ExtTransFn(UDEMessage *_msg)
{
	if (_msg->GetPort() == _InPort::STOP) {
		m_State = _State::END;
		UDEIF->SimulationEnd();
		return true;
	}
	else return false;
}

bool Generator::IntTransFn()
{
	return true;
}

bool Generator::OutputFn(UDEMessage *_msg)
{


	Customer *new_cust = new Customer(++m_count, distProc(UDEIF->RandomGen), UDEIF->GetCurrentTime());
	//Customer *new_cust = new Customer(++m_count, 200, UDEIF->GetCurrentTime());
	STDOUT << new_cust->GetID() << " is created" << std::endl;

	_msg->SetPortValue(_OutPort::SEND, (void*)new_cust);
	return true;
}

double Generator::TimeAdvanceFn()
{
	if (m_State == _State::END) return Infinity;


	double _ta = distGen(UDEIF->RandomGen);
	if (_ta < 0.0) _ta = 1.0;
	return _ta;
	//return 100.0;
}
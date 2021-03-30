#include "EF.h"
#include "Generator.h"
#include "Transducer.h"

EF::EF()
{
	SetName("EF");

	UDEModel *gen = new Generator();
	UDEModel *trans = new Transducer();

	AddComponent(gen);
	AddComponent(trans);

	AddInPort(0);
	AddOutPort(0);

	AddCoupling(gen, Generator::_OutPort::SEND, this, 0); //EOC
	AddCoupling(this, 0, trans, 0);						  //EIC
	AddCoupling(trans, 0, gen, Generator::_InPort::STOP); //IC

	SetPriority(2, trans, gen);
}

EF::~EF()
{

}

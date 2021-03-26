#include "Bank.h"
#include "Queue.h"
#include "Teller.h"

BANK::BANK()
{

	SetName("BANK");

	UDEModel *q = new Queue();
	UDEModel **t = new UDEModel*[UDEDB.GetInt("Queue", "InitialTeller")];
	for (int l1 = 0; l1 < UDEDB.GetInt("Queue", "InitialTeller"); l1++) {
		t[l1] = new Teller(l1 + 1);
	}

	AddInPort(0); AddOutPort(0);

	AddComponent(q);

	for (int l1 = 0; l1 < UDEDB.GetInt("Queue", "InitialTeller"); l1++) {
		AddComponent(t[l1]);
	}

	AddCoupling(this, 0, q, 0);

	for (int l1 = 0; l1 < UDEDB.GetInt("Queue", "InitialTeller"); l1++) {
		AddCoupling(q, l1 + 1, t[l1], 0);
		AddCoupling(t[l1], Teller::_OutPort::READY, q, l1 + 1);
		AddCoupling(t[l1], Teller::_OutPort::DONE, this, 0);
	}

}

BANK::~BANK()
{

}

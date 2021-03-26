#include "BankSim.h"
#include "EF.h"
#include "BANK.h"

BankSim::BankSim()
{
	SetName("OutMost");

	UDEModel *ef = new EF();
	UDEModel *bank = new BANK();

	AddComponent(ef);
	AddComponent(bank);

	AddCoupling(ef, 0, bank, 0); // EF.OUT -> BANK.IN
	AddCoupling(bank, 0, ef, 0); // BANK.OUT -> EF.IN

	SetPriority(2, ef, bank);
}

BankSim::~BankSim()
{

}


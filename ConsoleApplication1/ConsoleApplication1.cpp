#include <tchar.h>
#include "EinsSim.h"
#include "BankSim.h"
#include "easylogging++.h"

int _tmain(int argc, _TCHAR* argv[])
{
	UDEEngine* engineIF = new UDEEngine();
	SET_UDEIF(engineIF);

	UDEDB.RegisterDouble("Generator", "IntervalLambda", 1.0);
	UDEDB.RegisterDouble("Generator", "ProcTimeLambda", 0.33);
	UDEDB.RegisterDouble("Queue", "BufferSendingTime", 0.1);
	UDEDB.RegisterInt("Queue", "InitialTeller", 2);
	UDEDB.RegisterInt("Transducer", "MAXCustomer", 100);
	UDEDB.RegisterInt("Queue", "MAXTeller", 10);
	UDEDB.RegisterFlag("Queue", "AddTeller", false);
	UDEDB.RegisterFlag("Queue", "RemoveTeller", false);

	engineIF->Randomize();
	engineIF->SetDefaultLogger();

	engineIF->SetModel(new BankSim());
	engineIF->SimulationStart();
	engineIF->DeleteModel();

	return 0;
}
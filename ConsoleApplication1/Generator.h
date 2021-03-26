#include "EinsSim.h"

class Generator : public UDEAModel {

	enum _State { ACTIVE, END } m_State;

	int m_count;
	std::mt19937_64 randGen;
	std::exponential_distribution<double> distProc;
	std::exponential_distribution<double> distGen;

public:

	enum _InPort { STOP = 0 };
	enum _OutPort { SEND = 0 };

	Generator();
	~Generator();

	virtual bool ExtTransFn(UDEMessage *);
	virtual bool IntTransFn();
	virtual bool OutputFn(UDEMessage *);
	virtual double TimeAdvanceFn();

};
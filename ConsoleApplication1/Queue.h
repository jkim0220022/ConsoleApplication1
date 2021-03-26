#include "EinsSim.h"
#include "Customer.h"

#define MAX_TELLER 100

class Queue : public UDEAModel {

	enum _State { NORMAL, SENDING } m_State;
	enum _StateTeller { BUSY, FREE };

	int m_Teller[MAX_TELLER];
	int m_numTeller;

	int m_targetTeller;
	int m_countForFree;
	int m_tellerIsJustAdded;

	bool m_bFired;

	std::list<Customer *> m_Waiting;

public:
	Queue();
	~Queue();

	virtual bool ExtTransFn(UDEMessage *);
	virtual bool IntTransFn();
	virtual bool OutputFn(UDEMessage *);
	virtual double TimeAdvanceFn();

};
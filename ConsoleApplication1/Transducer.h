#include "EinsSim.h"

class Transducer : public UDEAModel {

public:

	int m_count;
	double m_sumWaitTime;
	enum _State { WAIT, SEND, END } m_State;
	Transducer();
	~Transducer();

	virtual bool ExtTransFn(UDEMessage *);
	virtual bool IntTransFn();
	virtual bool OutputFn(UDEMessage *);
	virtual double TimeAdvanceFn();

};
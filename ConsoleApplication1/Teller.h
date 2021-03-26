#include "EinsSim.h"
#include "Customer.h"

class Teller : public UDEAModel {

	double procTime;
	Customer *cust;

public:

	enum _OutPort { DONE, READY };
	enum _State { BUSY, FREE } m_State;

	Teller();
	Teller(int num);
	~Teller();

	virtual bool ExtTransFn(UDEMessage *);
	virtual bool IntTransFn();
	virtual bool OutputFn(UDEMessage *);
	virtual double TimeAdvanceFn();

};
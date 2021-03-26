#ifndef __CUSTOMER_H__
#define __CUSTOMER_H__

class Customer {
	int m_ID;
	double m_procTime;
	double m_waitingStart;
	double m_waitingEnd;

public:

	Customer() {
		m_ID = -1;
		m_procTime = -1.0;
	}
	Customer(int _id, double _procTime, double _waitingStart) {

		m_ID = _id;
		m_procTime = _procTime;
		m_waitingStart = _waitingStart;
	}
	~Customer() {

	}

	int GetID() { return m_ID; }

	void SetWaiting(double _time) {
		m_waitingEnd = _time;
	}
	double GetWaitingTime() {
		return m_waitingEnd - m_waitingStart;
	}
	double GetProcTime() {
		return m_procTime;
	}
};

#endif
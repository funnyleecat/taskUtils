#pragma once
#include "ThreadComDef.hpp"
#include "TimerThread.hpp"
#include <boost/atomic.hpp>
namespace TaskUtils {

class CTimeSlot
{
	public:
		CTimeSlot();
		void Swap(COperationHandlerSPtrVec &tmp);
		void Push_back(COperationHandlerSPtr pData);
	private:
		COperationHandlerSPtrVec m_SlotInfo;
		RWMutex m_mutex;
};

typedef boost::shared_ptr<CTimeSlot> TimeSlotPtr;
typedef std::vector<TimeSlotPtr> TimeSlotVec;

class CTimerWheelThread :public TimerThread
{
public:
	CTimerWheelThread(long maxUnit, CThreadPoolUtils &pool);
	virtual ~CTimerWheelThread() throw();
	virtual void Stop();
	void Join();
	virtual void Start();
	virtual bool PushData(COperationHandlerSPtr &pData, long delaySecs);
protected:
	void Loop();
	void PushWheel();
protected:
	bool m_bStop;
	thread_ptr m_thread;
	TimeSlotVec m_queueVecWheel;
	boost::atomic_ulong m_uIndex;
	long m_nMaxUnit;
	CThreadPoolUtils &m_ThreadPool;
};
}

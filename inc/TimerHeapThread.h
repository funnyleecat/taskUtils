#pragma once
#include "TimerThread.hpp"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/atomic.hpp>
#include <vector>

class COperation;
using namespace std;
namespace TaskUtils {

class CTimerTask
{
	public:
		static boost::shared_ptr<CTimerTask> Create();
		COperationHandlerSPtr GetTask();
		const boost::posix_time::ptime &GetTime();
		void ResetOperation();
		void SetTask(COperationHandlerSPtr &pHandler, boost::posix_time::ptime &sTime);
		bool operator > (const CTimerTask &ref);
		~CTimerTask()throw();
	protected:
		CTimerTask();		
	protected:
		COperationHandlerSPtr m_pHandler;
		boost::posix_time::ptime m_system_time;
};

typedef boost::shared_ptr<CTimerTask> CTimerTaskPtr;

typedef std::vector<CTimerTaskPtr> CTimerTaskPtrVec;

class CTimerHeapThread :public TimerThread
{
	public:
		CTimerHeapThread(int nMaxTask, CThreadPoolUtils &threadPool);
		virtual ~CTimerHeapThread() throw();
		virtual void Start();
		virtual void Stop();
		virtual void Join();
		virtual bool PushData(COperationHandlerSPtr &pHandler, long nDelay);
	protected:
		void Expand();
		void Loop();
		void ShiftUp();
		void ShiftDown(unsigned int nStart, CTimerTaskPtr tPtr);
		void Swap(CTimerTaskPtr &t1, CTimerTaskPtr &t2);
		boost::shared_ptr<CTimerTask> PopTask();
		boost::shared_ptr<CTimerTask>& TopTask();
	protected:
		boost::mutex m_heapMutex;
		CTimerTaskPtrVec m_TaskHeap;
		RWMutex m_taskMutex;
		boost::condition_variable_any m_dataCond;
		thread_ptr m_Pthread;
		CThreadPoolUtils &m_ThreadPool;
		boost::atomics::atomic_ulong m_nMaxTask;
		boost::atomics::atomic_ulong m_nEndPos;
		boost::atomics::atomic_bool m_bStop;
		
};
}


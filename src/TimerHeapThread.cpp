#include "TimerHeapThread.h"
#include "ThreadPoolUtils.hpp"
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread_time.hpp>
#include "ThreadPoolFacade.hpp"

typedef boost::mutex::scoped_lock Scopedlock;
using namespace boost;

namespace TaskUtils {
	boost::shared_ptr<CTimerTask> CTimerTask::Create()
	{
		return boost::shared_ptr<CTimerTask>(new CTimerTask);
	}

	void CTimerTask::SetTask(COperationHandlerSPtr &pHandler, boost::posix_time::ptime &sTime)
	{
		m_pHandler = pHandler;
		m_system_time = sTime;
	}

	COperationHandlerSPtr CTimerTask::GetTask()
	{
		return m_pHandler;
	}

	const boost::posix_time::ptime &CTimerTask::GetTime()
	{
		return m_system_time;
	}

	CTimerTask::CTimerTask()
	{

	}

	CTimerTask::~CTimerTask()throw()
	{
	}

	void CTimerTask::ResetOperation()
	{
		//m_Operation = NULL;
	}

	bool CTimerTask::operator> (const CTimerTask &ref)
	{
		if (m_system_time > ref.m_system_time)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	CTimerHeapThread::CTimerHeapThread(int nMaxTask, CThreadPoolUtils &threadPool)
	:m_nMaxTask(nMaxTask)
	,m_nEndPos(0)
	,m_ThreadPool(threadPool)
	,m_bStop(true)
	{
		m_TaskHeap.reserve(nMaxTask);
		for (int index = 0; index < nMaxTask; ++index)
		{
			CTimerTaskPtr ptr = CTimerTask::Create();
			m_TaskHeap.push_back(ptr);
		}
	}
	
	CTimerHeapThread::~CTimerHeapThread()throw()
	{

	}

	void CTimerHeapThread::Start()
	{
		m_bStop = false;
		m_Pthread = thread_ptr(new boost::thread(boost::bind(&CTimerHeapThread::Loop, this)));
	}

	void CTimerHeapThread::Join()
	{
		m_Pthread->join();
	}

	void CTimerHeapThread::Loop()
	{
		try{
			do
			{
				ReadLock condLock(m_taskMutex);
				//定时堆中如果没有任务则一直阻塞
				while(0 == m_nEndPos)
				{
					//ReadLock condLock(m_taskMutex);
					m_dataCond.wait(condLock);
				}

				Tasklabel:boost::shared_ptr<CTimerTask> task = TopTask();
				if (task.get())
				{
					if (m_dataCond.timed_wait(condLock, task->GetTime()))
					{
						//又有新的超时任务到达,需要重新从最小堆中取出定时任务
						goto Tasklabel;
					}
					else
					{
						//超时取出堆顶任务，然后执行超时操作。
						static int num = 0;
						boost::shared_ptr<CTimerTask> taskOp = PopTask();
						if (taskOp.get())
						{
							COperationHandlerSPtr pHandler = taskOp->GetTask();
							reinterpret_cast<CThreadPoolFacade&>(m_ThreadPool).InsertHandler(pHandler);
							//m_ThreadPool.InsertTask(opPtr, 0);
							
						}
					}	
				}
			} while (!m_bStop);
		}
		catch (...)
		{

		}
	}

	bool CTimerHeapThread::PushData(COperationHandlerSPtr &pHandler, long nDelay)
	{
		ReadLock condLock(m_taskMutex);
		Scopedlock lock(m_heapMutex);
		if (m_nEndPos >= m_nMaxTask)
		{
			Expand();
		}		

		boost::posix_time::ptime systemTime = get_system_time() + posix_time::seconds(nDelay);
		m_TaskHeap[m_nEndPos]->SetTask(pHandler, systemTime);
		ShiftUp();
		m_nEndPos++;
		m_dataCond.notify_one();
		return true;
		//}
	}

	void CTimerHeapThread::Expand()
	{
		unsigned long nExPand = m_nMaxTask * 2 - m_nMaxTask;
		for (unsigned long index = 0; index < nExPand; ++index)
		{
			CTimerTaskPtr ptr = CTimerTask::Create();
			m_TaskHeap.push_back(ptr);
		}

	}

	boost::shared_ptr<CTimerTask> CTimerHeapThread::PopTask()
	{
		if (!m_nEndPos)
		{
			CTimerTask *p = NULL;
			return boost::shared_ptr<CTimerTask>(p);
		}
		else
		{
			Scopedlock lock(m_heapMutex);
			boost::shared_ptr<CTimerTask> tmp = m_TaskHeap[0];
			//Swap(m_TaskHeap[0],m_TaskHeap[m_nEndPos - 1]);
			--m_nEndPos;
			ShiftDown(0, m_TaskHeap[m_nEndPos]);
			//ShiftDown(0, m_nEndPos);
			return tmp;
		}
	}

	boost::shared_ptr<CTimerTask>& CTimerHeapThread::TopTask()
	{
		Scopedlock lock(m_heapMutex);
		return m_TaskHeap[0];
	}

	void CTimerHeapThread::Stop()
	{
		m_bStop = true;
		m_Pthread->interrupt();
	}


	void CTimerHeapThread::Swap(CTimerTaskPtr &t1, CTimerTaskPtr &t2)
	{
		CTimerTaskPtr tmp = t1;
		t1 = t2;
		t2 = tmp;
	}

	void CTimerHeapThread::ShiftUp()
	{
		int nPos = m_nEndPos;
		while (nPos > 0)
		{
			int nParent = (nPos % 2 == 0 ? nPos / 2 - 1 : nPos / 2);
			if (*m_TaskHeap[nParent] > *m_TaskHeap[nPos])
			{
				Swap(m_TaskHeap[nParent], m_TaskHeap[nPos]);
				nPos = nParent;
			}
			else
			{
				--nPos;
			}
		}
	}


	void CTimerHeapThread::ShiftDown(unsigned int nStart, CTimerTaskPtr tPtr)
	{
		unsigned int half = m_nEndPos / 2;
		while (nStart < half){
			unsigned int child = 2 * nStart + 1;
			unsigned int right = child + 1;

			CTimerTaskPtr &ref = m_TaskHeap[child];
			CTimerTask &ojb = *ref;

			if (right < m_nEndPos && *m_TaskHeap[child] > *m_TaskHeap[right]){
				child = right;
			}
			if (*m_TaskHeap[child] > *tPtr){
				break;
			}
			m_TaskHeap[nStart] = m_TaskHeap[child];
			nStart = child;
		}
		m_TaskHeap[nStart] = tPtr;
	}

}

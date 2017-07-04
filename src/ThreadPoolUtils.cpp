#include "ThreadPoolFacade.hpp"
#include <vector>
#include <boost/lockfree/queue.hpp>  
#include <boost/bind.hpp>  
#include <boost/thread/mutex.hpp> 
#include <boost/thread.hpp>
#include <boost/atomic.hpp>
#include "ThreadUtils.hpp"
#include "TimerThread.hpp"
#include "TimerWheelThread.h"
#include "TimerHeapThread.h"
#include "OperationHandlerBridge.h"

namespace TaskUtils {
	using namespace boost;
	using namespace boost::lockfree;

	//typedef std::vector<COperationPtr> COperationPtrVec;

	template<class T>
	class BaseExe
	{
		typedef void(T::*Func)();
	public:
		void Execute(boost::shared_ptr<T> ptr, Func fun)
		{
			boost::bind(fun, ptr.get())();
		}

	};

	class CThreadPoolUtilsImpl : public CThreadPoolFacade
	{
	public:
		CThreadPoolUtilsImpl(unsigned int unThreadNum, long lMaxSlot, TimerType eType)
			:m_ulIndex(0)
			, m_nThreadNum(unThreadNum)
		{
			for (unsigned int index = 0; index < unThreadNum; ++index)
			{
				m_threadPool.push_back(CThreadUtils::Create());
			}
			if (TIMER_WHEEL == eType || TIMER_HEAP == eType)
			{
				m_tPtr = TimerThread::Create(lMaxSlot, *this, eType);
			}
		}

		virtual void InsertHandler(COperationHandlerSPtr &pHandler)
		{
			(*m_threadPool[(m_ulIndex++) % m_nThreadNum]).PushData(pHandler);
		}

		virtual boost::shared_ptr<COperationHandler> InsertTask(boost::shared_ptr<COperation> &pData, long lDelaySecs)
		{
			boost::shared_ptr<COperationHandler> nullHandler;
			if (lDelaySecs)
			{
				COperationHandlerSPtr pHandler(new COperationHandlerBridge(pData));
				if (m_tPtr.get())
				{
					m_tPtr->PushData(pHandler, lDelaySecs);
					return pHandler;
				}

			}
			else
			{
				if (m_threadPool.size())
				{
					COperationHandlerSPtr pHandler(new COperationHandlerBridge(pData));
					(*m_threadPool[(m_ulIndex++) % m_nThreadNum]).PushData(pHandler);
					return pHandler;
				}							
			}
			return nullHandler;
		}

		virtual void Run()
		{
			std::for_each(m_threadPool.begin(), m_threadPool.end(), boost::bind(&CThreadUtils::Run,_1));
			if (m_tPtr.get())
			{
				m_tPtr->Start();
			}
		}

		virtual void Stop()
		{
			std::for_each(m_threadPool.begin(), m_threadPool.end(), boost::bind(&CThreadUtils::Stop, _1));
			std::for_each(m_threadPool.begin(), m_threadPool.end(), boost::bind(&CThreadUtils::Join, _1));
			if (m_tPtr.get())
			{
				m_tPtr->Stop();
				m_tPtr->Join();
			}
		}

		~CThreadPoolUtilsImpl() throw()
		{

		}
	private:
		boost::atomics::atomic_ulong m_ulIndex;
		unsigned int m_nThreadNum;
		CThreadPtrVec m_threadPool;
		boost::shared_ptr<TimerThread> m_tPtr;
	};

	boost::shared_ptr<CThreadPoolUtils> CThreadPoolUtils::Create(unsigned int unThreadNum, long lMaxSlot, TimerType eType)
	{
		return 	boost::shared_ptr<CThreadPoolUtils>(new CThreadPoolUtilsImpl(unThreadNum, lMaxSlot, eType));
	}
}

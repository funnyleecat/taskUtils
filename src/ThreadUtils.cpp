#include "ThreadUtils.hpp"
#include "Operation.hpp"
#include "ThreadPoolUtils.hpp"
#include <boost/atomic.hpp>
#include "OperationHandlerBridge.h"
#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include <boost/ref.hpp>

namespace TaskUtils {
	class CThreadUtilsImpl : public CThreadUtils
	{
	public:
		CThreadUtilsImpl()
			:m_bStop(false)
			, m_taskNum(0)
		{


		}

		~CThreadUtilsImpl() throw()
		{
			m_pThread->interrupt();
			m_pThread->join();
		}


		void Stop()
		{
			m_pThread->interrupt();
			m_bStop = true;
		}

		virtual void Run()
		{
			m_pThread = thread_ptr(new boost::thread(boost::bind(&CThreadUtilsImpl::Execute, this)));
		}

		void Join()
		{
			m_pThread->join();
		}

		void PushData(COperationHandlerSPtr &data)
		{
			LockGuard lock(m_mutex);
			m_dataQueue.push_back(data);
			++m_taskNum;
			m_dataCond.notify_one();
		}

		void SwapData(COperationHandlerSPtrVec &dataVec)
		{
			LockGuard lock(m_mutex);
			if (!m_taskNum)
			{
				m_dataCond.wait(m_mutex);
			}
			dataVec.swap(m_dataQueue);

			m_taskNum = 0;
		}

		void Execute()
		{
			try{
				while (!m_bStop)
				{
					COperationHandlerSPtrVec tmpVec;
					SwapData(tmpVec);
					if (tmpVec.empty())
						continue;

					boost::reference_wrapper<CThreadUtilsImpl> cWrp(*this);
					std::for_each(tmpVec.begin(), tmpVec.end(), boost::bind(&CThreadUtilsImpl::RunTask, cWrp, _1));
				}
			}
			catch (...)
			{
				//Ö±½ÓÍË³ö
			}
		}

		void RunTask(COperationHandlerSPtr &pHandler)
		{
			COperationHandlerBridge *pBri = reinterpret_cast<COperationHandlerBridge *>(pHandler.get());
			pBri->Run();
		}
	private:
		bool m_bStop;
		thread_ptr m_pThread;
		COperationHandlerSPtrVec m_dataQueue;
		boost::atomic_ulong m_taskNum;
		boost::condition_variable_any m_dataCond;
		boost::mutex m_mutex;
	};

	boost::shared_ptr<CThreadUtils> CThreadUtils::Create()
	{
		return boost::shared_ptr<CThreadUtils>(new CThreadUtilsImpl());
	}

}

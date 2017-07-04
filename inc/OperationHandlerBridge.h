#pragma once
#include "OperationHandler.hpp"
#include <boost/atomic.hpp>
#include <boost/thread/thread_time.hpp>
#include <ThreadComDef.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>
namespace TaskUtils {

	class COperation;

	class COperationHandlerBridge : public COperationHandler
	{
	public:
		void Run();
		boost::shared_ptr<COperation> GetOperation();
		virtual ~COperationHandlerBridge()throw();
		COperationHandlerBridge(COperationSPtr pOperation);
		virtual unsigned long WaitResult(long lTimeout);
		virtual void Cancel();
	private:
		COperationSPtr m_pOperation;
		boost::atomics::atomic_ulong  m_taskStatus;
		boost::interprocess::interprocess_semaphore m_taskSem;
	};

}

#include "OperationHandlerBridge.h"
#include "Operation.hpp"
#include <boost/thread/thread_time.hpp>
namespace TaskUtils {
	COperationHandlerBridge::COperationHandlerBridge(COperationSPtr pOperation)
: m_taskStatus(TASK_WAITING)
, m_pOperation(pOperation)
, m_taskSem(0)
{

}

COperationHandlerBridge::~COperationHandlerBridge()throw()
{

}

boost::shared_ptr<COperation> COperationHandlerBridge::GetOperation()
{
	return m_pOperation;
}

void COperationHandlerBridge::Run()
{
	//调度任务取消或者，或者任务不存在。
	if (!m_pOperation.get() || TASK_CANCEL == m_taskStatus)
	{
		return;
	}
	if (m_pOperation->Run())
	{
		m_taskStatus = TASK_SUCCESS;
	}
	else
	{
		m_taskStatus = TASK_CANCEL;
	}
	m_taskSem.post();
	//m_dataCond.notify_all();
}

unsigned long COperationHandlerBridge::WaitResult(long lTimeout)
{	
	//ReadLock guard(m_taskMutex);
	if (TASK_WAITING != m_taskStatus)
	{
		return m_taskStatus;
	}
	else
	{	
		if (lTimeout != 0)
		{
			m_taskSem.timed_wait(boost::get_system_time() + boost::posix_time::seconds(lTimeout));
		}
		else
		{
			m_taskSem.wait();
		}
		return m_taskStatus;
	}
}
		
void COperationHandlerBridge::Cancel()
{
	m_taskStatus = TASK_CANCEL;
	m_taskSem.post();
}
}

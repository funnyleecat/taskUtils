#include "TimerWheelThread.h"
#include "ThreadPoolUtils.hpp"
#include "ThreadPoolFacade.hpp"
namespace TaskUtils {

	CTimeSlot::CTimeSlot()
	{

	}
	void CTimeSlot::Swap(COperationHandlerSPtrVec &tmp)
	{
		WriteLock lock(m_mutex);
		tmp.swap(m_SlotInfo);
	}

	void CTimeSlot::Push_back(COperationHandlerSPtr pData)
	{
		WriteLock lock(m_mutex);
		m_SlotInfo.push_back(pData);
	}

	CTimerWheelThread::CTimerWheelThread(long maxUnit, CThreadPoolUtils &pool)
		:m_nMaxUnit(maxUnit)
		, m_ThreadPool(pool)
		, m_bStop(true)
		, m_uIndex(0)
	{
		m_queueVecWheel.reserve(m_nMaxUnit);
		for (long i = 0; i < m_nMaxUnit; ++i)
		{
			TimeSlotPtr ptr(new CTimeSlot());
			m_queueVecWheel.push_back(ptr);
		}
	}

	CTimerWheelThread::~CTimerWheelThread() throw()
	{

	}

	void CTimerWheelThread::Stop()
	{
		m_bStop = true;
		m_thread->interrupt();
	}

	void CTimerWheelThread::Join()
	{
		(*m_thread).join();
	}

	void CTimerWheelThread::Start()
	{
		m_bStop = false;
		m_thread = thread_ptr(new boost::thread(boost::bind(&CTimerWheelThread::Loop, this)));
	}

	bool CTimerWheelThread::PushData(COperationHandlerSPtr &pData, long delaySecs)
	{
		if (delaySecs >= m_nMaxUnit)
		{
			return false;
		}
		else
		{
			long pos = (m_uIndex + delaySecs) % m_nMaxUnit;
			TimeSlotPtr &ref = m_queueVecWheel[pos];
			ref->Push_back(pData);
			return true;
		}
	}

	void CTimerWheelThread::Loop()
	{
		try
		{
			do
			{
				boost::this_thread::sleep(boost::posix_time::seconds(1));
				PushWheel();
			} while (!m_bStop);
		}
		catch (std::exception&)
		{
			//当线程被interrupt之后，将会产生异常
			//这里只捕获了异常但不做任何处理，只是为了正常停止线程
		}
	}

	void CTimerWheelThread::PushWheel()
	{
		if (m_uIndex == m_nMaxUnit - 1)
		{
			m_uIndex = 0;
		}
		else
		{
			++m_uIndex;
		}

		COperationHandlerSPtrVec temp;
		m_queueVecWheel[m_uIndex]->Swap(temp);
		COperationHandlerSPtrVec::iterator iter = temp.begin();
		while (iter != temp.end())
		{
			reinterpret_cast<CThreadPoolFacade&>(m_ThreadPool).InsertHandler(*iter);
			++iter;
		}
	}

}

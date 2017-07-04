#include "TimerThread.hpp"
#include <boost/atomic.hpp>
#include "ThreadPoolUtils.hpp"
#include "TimerWheelThread.h"
#include "TimerHeapThread.h"
//#include "ThreadPoolUtils.hpp"
//#include <expection.h>

namespace TaskUtils {



boost::shared_ptr<TimerThread> TimerThread::Create(long maxUnit, CThreadPoolUtils &pool,TimerType eType)
{

	if (TIMER_WHEEL == eType)
	{
		return   boost::shared_ptr<TimerThread>(new CTimerWheelThread(maxUnit, pool));
	}
	else if (TIMER_HEAP == eType)
	{
		return boost::shared_ptr<TimerThread>(new CTimerHeapThread(maxUnit, pool));
	}
	else
	{
		TimerThread *p = NULL;
		return boost::shared_ptr<TimerThread>(p);
	}
}

}


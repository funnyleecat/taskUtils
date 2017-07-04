#ifndef _TIMER_THREAD_UTILS_HPP_
#define _TIMER_THREAD_UTILS_HPP_
#include "ThreadComDef.hpp"
#include "export.h"

namespace TaskUtils {
	class CThreadPoolUtils;
	class TimerThread
	{
	public:
		virtual ~TimerThread() throw(){}
		virtual void Stop() = 0;
		virtual void Join() = 0;
		virtual void Start() = 0;
		virtual bool PushData(COperationHandlerSPtr &pData, long delaySecs) = 0;
		static boost::shared_ptr<TimerThread> Create(long maxUnit, CThreadPoolUtils &pool, TimerType eType);
	};
}
#endif


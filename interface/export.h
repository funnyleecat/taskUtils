#pragma once


#ifdef WIN32
#   ifdef TASKUTILS_EXPORTS
#       define TASKUTILT_API __declspec(dllexport)
#   else
#       define TASKUTILT_API __declspec(dllimport)
#   endif //UTILS_EXPORTS
#else
#   define TASKUTILT_API
#endif //WIN32

const unsigned long TASK_SUCCESS = 1;
const unsigned long TASK_FAILED = 2;
const unsigned long TASK_CANCEL = 3;
const unsigned long TASK_WAITING = 4;

typedef enum TimerType
{
	TIMER_WHEEL,
	TIMER_HEAP,
	TIMER_UNKNOWN
}TimerType;

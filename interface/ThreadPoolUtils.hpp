/******************************************************************************
模块名  ： Utils
文件名  ： CThreadPoolUtils.hpp
相关文件：
文件实现功能：线程池调度相关接口
作者    ： lifeng
版本    ： 1.0.0.0
******************************************************************************/
#pragma once
#include <boost/shared_ptr.hpp>
#include "Operation.hpp"
//#include "ThreadComDef.hpp"
#include "export.h"
#include "OperationHandler.hpp"

namespace TaskUtils {


	//typedef boost::shared_ptr<COperation> COperationPtr;

	class TASKUTILT_API CThreadPoolUtils
	{
	public:
		virtual ~CThreadPoolUtils() throw(){}
		/*
			函数功能：插入一个执行任务
			入参：（1）data 的执行指针
			      （2）delaySecs 任务的延时秒数
			返回：返回一个控制COperation的句柄指针COperationHandler,如果在初始化任务池时候传入TIMER_UNKNOWN，那么当插入定时任务时候，就可能为COperationHandler 指向NULL。
		*/
		virtual boost::shared_ptr<COperationHandler> InsertTask(boost::shared_ptr<COperation> &pData, long lDelaySecs = 0) = 0;

		/*
			函数功能：启动任务池
		*/
		virtual void Run() = 0;

		/*
			函数功能：停止任务池
		*/
		virtual void Stop() = 0;

		/*
			函数功能：创建一个工作线程池
				入参：（1）threadNum 线程的数量
					  （2）maxSlot 为槽位数 当eType 为 TIMER_WHEEL 类型，代表定时器最大超时秒数，当eType为TIMER_HEAP代表最大定时任务数量
					  （3）eType 位定时器类型
		*/           
		static boost::shared_ptr<CThreadPoolUtils> Create(unsigned int unThreadNum, long lMaxSlot = 360, TimerType eType = TIMER_UNKNOWN);
	};
}


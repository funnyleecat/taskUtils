/******************************************************************************
模块名  ： TaskUtils
文件名  ： COperationHandler.hpp
相关文件：
文件实现功能：
作者    ： lifeng
版本    ： 1.0.0.0
******************************************************************************/
#pragma once
#include "export.h"
#include <boost/shared_ptr.hpp>
#include "Operation.hpp"

namespace TaskUtils {

	class  TASKUTILT_API COperationHandler
	{
		public:
			virtual ~COperationHandler()throw(){}
			/*
				函数功能：获取原始执行任务智能指针
				返回：COperation的智能指针
			*/
			virtual boost::shared_ptr<COperation> GetOperation() = 0;
			
			/*
				函数功能：等待用户一个异步任务的返回
				参数：（1）lTimeout 等待超时时间，如果lTimeout为0则一直等待
				返回：任务状态，其定义详见epxort.h	   
			*/
			virtual unsigned long WaitResult(long lTimeout = 0) =0;
			/*
				函数功能：取消一个任务的调度
			*/
			virtual void Cancel() = 0;
	};

}
 

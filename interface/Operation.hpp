/******************************************************************************
模块名  ： Utils
文件名  ： COperation.hpp
相关文件：
文件实现功能：任务抽象接口，子类继承COperation
作者    ： lifeng
版本    ： 1.0.0.0
******************************************************************************/
#pragma once
//#ifndef _OPERATION_UTILS_HPP_
//#define _OPERATION_UTILS_HPP_
//#include "COperationFacade.hpp"
//#include <boost/shared_ptr.hpp>
#include "export.h"

namespace TaskUtils {
	
	class  TASKUTILT_API COperation //: public COperationFacade
	{
	public:
		virtual ~COperation() throw(){}
		/*
			函数功能：任务执行的纯虚接口，子类实现
			返回：执行任务的结果
		*/
		virtual bool Run() = 0;
	};
}

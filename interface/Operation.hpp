/******************************************************************************
ģ����  �� Utils
�ļ���  �� COperation.hpp
����ļ���
�ļ�ʵ�ֹ��ܣ��������ӿڣ�����̳�COperation
����    �� lifeng
�汾    �� 1.0.0.0
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
			�������ܣ�����ִ�еĴ���ӿڣ�����ʵ��
			���أ�ִ������Ľ��
		*/
		virtual bool Run() = 0;
	};
}

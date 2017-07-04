/******************************************************************************
ģ����  �� TaskUtils
�ļ���  �� COperationHandler.hpp
����ļ���
�ļ�ʵ�ֹ��ܣ�
����    �� lifeng
�汾    �� 1.0.0.0
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
				�������ܣ���ȡԭʼִ����������ָ��
				���أ�COperation������ָ��
			*/
			virtual boost::shared_ptr<COperation> GetOperation() = 0;
			
			/*
				�������ܣ��ȴ��û�һ���첽����ķ���
				��������1��lTimeout �ȴ���ʱʱ�䣬���lTimeoutΪ0��һֱ�ȴ�
				���أ�����״̬���䶨�����epxort.h	   
			*/
			virtual unsigned long WaitResult(long lTimeout = 0) =0;
			/*
				�������ܣ�ȡ��һ������ĵ���
			*/
			virtual void Cancel() = 0;
	};

}
 

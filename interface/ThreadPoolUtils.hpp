/******************************************************************************
ģ����  �� Utils
�ļ���  �� CThreadPoolUtils.hpp
����ļ���
�ļ�ʵ�ֹ��ܣ��̳߳ص�����ؽӿ�
����    �� lifeng
�汾    �� 1.0.0.0
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
			�������ܣ�����һ��ִ������
			��Σ���1��data ��ִ��ָ��
			      ��2��delaySecs �������ʱ����
			���أ�����һ������COperation�ľ��ָ��COperationHandler,����ڳ�ʼ�������ʱ����TIMER_UNKNOWN����ô�����붨ʱ����ʱ�򣬾Ϳ���ΪCOperationHandler ָ��NULL��
		*/
		virtual boost::shared_ptr<COperationHandler> InsertTask(boost::shared_ptr<COperation> &pData, long lDelaySecs = 0) = 0;

		/*
			�������ܣ����������
		*/
		virtual void Run() = 0;

		/*
			�������ܣ�ֹͣ�����
		*/
		virtual void Stop() = 0;

		/*
			�������ܣ�����һ�������̳߳�
				��Σ���1��threadNum �̵߳�����
					  ��2��maxSlot Ϊ��λ�� ��eType Ϊ TIMER_WHEEL ���ͣ�����ʱ�����ʱ��������eTypeΪTIMER_HEAP�������ʱ��������
					  ��3��eType λ��ʱ������
		*/           
		static boost::shared_ptr<CThreadPoolUtils> Create(unsigned int unThreadNum, long lMaxSlot = 360, TimerType eType = TIMER_UNKNOWN);
	};
}


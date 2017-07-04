// taskUtilsTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "OperationHandler.hpp"
#include "Operation.hpp"
#include "ThreadPoolUtils.hpp"
#include <iostream>
#include<boost/shared_ptr.hpp>
#include <vector>
#ifdef WIN32
#include <windows.h>
#else 
#include <signal.h>
#include <unistd.h>
#endif

using namespace std;
using namespace TaskUtils;

class TestOperation : public COperation
{
	public:
		TestOperation(long num) :num_(num)
		{

		}
		bool Run()
		{
			std::cout << "Runing " << num_<<endl;
			return true;
		}
	protected:
		long num_;
};

typedef std::vector<boost::shared_ptr<COperationHandler>> HandlerVec;



void TestTimer(TimerType type)
{
	boost::shared_ptr<CThreadPoolUtils> ptr = CThreadPoolUtils::Create(3, 80, type);
	ptr->Run();
	HandlerVec handleVec;
	int TestNum = 60;
	for (long i = 1; i <= TestNum; ++i)
	{
		boost::shared_ptr<COperation> pTest(new TestOperation(i));
		boost::shared_ptr<COperationHandler> pH = ptr->InsertTask(pTest, i);
		handleVec.push_back(pH);
	}

	for (long i = 0; i < TestNum; ++i)
	{
		boost::shared_ptr<COperationHandler> pH = handleVec[i];
		pH->WaitResult();
	}
	ptr->Stop();
}

void TestTimerWithoutWait(TimerType type)
{
	boost::shared_ptr<CThreadPoolUtils> ptr = CThreadPoolUtils::Create(3, 20, type);
	ptr->Run();
	int TestNum = 60;
	for (long i = 1; i <= TestNum; ++i)
	{
		boost::shared_ptr<COperation> pTest(new TestOperation(i));
		boost::shared_ptr<COperationHandler> pH = ptr->InsertTask(pTest, i);
	}
	long index = 0;
	while (index < 90)
	{
#ifdef WIN32
		Sleep(1000);
#else
		sleep(1);
#endif
		++index;
	}
	ptr->Stop();
}

int _tmain(int argc, _TCHAR* argv[])
{
	TestTimer(TIMER_WHEEL);
	//TestTimer(TIMER_HEAP);
	//TestTimerWithoutWait(TIMER_HEAP);
	return 0;
}


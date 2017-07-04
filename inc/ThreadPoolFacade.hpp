#pragma once
#include "ThreadComDef.hpp"
#include "ThreadPoolUtils.hpp"

namespace TaskUtils{
class COperationHandler;
class CThreadPoolUtils;

class CThreadPoolFacade: public CThreadPoolUtils
{
	public:
		~CThreadPoolFacade() throw(){ ; }
		virtual void InsertHandler(COperationHandlerSPtr &pHandler) = 0;
};
}


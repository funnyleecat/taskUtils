#pragma once
#include "ThreadComDef.hpp"

namespace TaskUtils {
	
	class COperationHandler;
	class CThreadUtils : public boost::noncopyable
	{
	public:
		virtual ~CThreadUtils() throw(){}
		virtual void Stop() = 0;
		virtual void Run() = 0;
		virtual void Join() = 0;
		virtual void PushData(COperationHandlerSPtr &pData) = 0;
		static boost::shared_ptr<CThreadUtils> Create();

	};

	typedef boost::shared_ptr<CThreadUtils> CThreadUtilsPtr;
	typedef std::vector<CThreadUtilsPtr> CThreadPtrVec;
}

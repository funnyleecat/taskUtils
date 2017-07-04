#pragma once
#include <boost/shared_ptr.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/thread.hpp>
#include <vector>

namespace TaskUtils {
	class COperation;
	typedef boost::shared_ptr<boost::thread>  thread_ptr;
	
	typedef boost::shared_ptr<COperation> COperationSPtr;
	typedef boost::weak_ptr<COperation> COperationWPtr;
	typedef std::vector<COperationWPtr> COperationWPtrVec;

	//typedef std::vector<COperation*> COperationPtrVec;
	
	class COperationHandler;
	//class COperationHandlerBridge;

	typedef boost::shared_ptr<COperationHandler> COperationHandlerSPtr;
	typedef std::vector<COperationHandlerSPtr> COperationHandlerSPtrVec;
	//typedef std::vector<COperationHandler*> COperationHandlerPtrVec;
	//typedef boost::lockfree::queue<COperationHandler*> FreeLockQueue;

	typedef boost::shared_mutex RWMutex;
	typedef boost::shared_lock<RWMutex> ReadLock;
	typedef boost::unique_lock<RWMutex> WriteLock;
	typedef boost::lock_guard<boost::mutex> LockGuard;

	class CThreadPoolUtils;

}

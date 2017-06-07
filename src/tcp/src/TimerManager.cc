#include "TimerManager.h"
#include "EventLoop.h"
#include "TimerHandler.h"
#include "Log.h"

TimerManager::TimerManager(EventLoop* loop_ptr)
:loop_ptr_(loop_ptr)
{
}


void TimerManager::runAfter(std::function<void()> cb, const double interval)
{
	//LOG4CPLUS_DEBUG(_logger, "runAfter");
	registerTimer(cb, interval);
}

void TimerManager::runEvery(std::function<void()> cb, const double interval)
{
	//LOG4CPLUS_DEBUG(_logger, "runEvery");
	std::function<void()> cb1 = [=]()
	{
		cb();
		runEvery(cb, interval);
	};
	runAfter(cb1, interval);
}

void TimerManager::runAt(std::function<void()> cb, const TrantorTimestamp timestamp)
{
	long long usec = timestamp - TrantorTimestamp::now();
	runAfter(cb, usec/1000000.0);
}

void TimerManager::registerTimer(std::function<void()> cb, double interval)
{
	//LOG4CPLUS_DEBUG(_logger, "registerTimer");
	
	TimerHandler* timer_handler = new TimerHandler(loop_ptr_, cb, interval); //delete in timer cb
}


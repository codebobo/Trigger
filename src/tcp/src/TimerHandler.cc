#include "TimerHandler.h"
#include "EventLoop.h"
#include "Log.h"

TimerHandler::TimerHandler(EventLoop* loop_ptr, std::function<void()> cb, const double interval)
:EventHandlerAbstract(loop_ptr, interval), cb_(cb)
{
	event_wrap_.setEventsHandler(std::bind(&TimerHandler::handleEvents, this, std::placeholders::_1, std::placeholders::_2, this));
	event_wrap_.registerEvent();
}

TimerHandler::~TimerHandler()
{
}

void TimerHandler::handleEvents(const int fd, const short events, void* arg)
{
	LOG4CPLUS_DEBUG(_logger, "timer callback!");
	TimerHandler* handler = (TimerHandler*)arg;
	if(handler && handler->cb_)
	{
		if(handler->cb_)
		{
			(handler->cb_)();
		}
		delete handler;
	}
}








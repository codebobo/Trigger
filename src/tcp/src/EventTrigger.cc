#include "EventTrigger.h"
#include <sys/eventfd.h>
#include "unistd.h"
#include "Log.h"
#include "EventLoop.h"

EventTrigger::EventTrigger(EventLoop* loop_ptr, std::function<void()> trig_func)
:eventfd_(createEventfd()),
trig_func_(trig_func),
event_handler_(loop_ptr, eventfd_)
{
	event_handler_.enableRead();
	event_handler_.setReadCallback(std::bind(&EventTrigger::handleEvents, this));
	//event_wrap_.registerEvent();
}

uint32_t EventTrigger::createEventfd()
{
	uint64_t evtfd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
	if (evtfd < 0)
	{
		abort();
	}
	return evtfd;
}

void EventTrigger::trigEvent()
{
	//LOG4CPLUS_DEBUG(_logger, "trig event!");
	uint64_t one = 1;
	uint64_t n = write(eventfd_, &one, sizeof one);
}

void EventTrigger::handleEvents()
{
	LOG4CPLUS_DEBUG(_logger, "trig callback!");
	uint64_t one = 1;
	uint64_t n = read(eventfd_, &one, sizeof one);
	if (n != sizeof one)
	{
		LOG4CPLUS_DEBUG(_logger, "EventLoop::handleRead() reads " << n << " bytes instead of 8");
	}
	if(trig_func_)
	{
		trig_func_();
	}
}



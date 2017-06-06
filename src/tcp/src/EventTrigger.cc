#include "EventTrigger.h"
#include <sys/eventfd.h>
#include "unistd.h"
#include "Log.h"
#include "EventLoop.h"

EventTrigger::EventTrigger(EventLoop* loop_ptr, std::function<void()> trig_func)
:EventHandlerAbstract(loop_ptr, (int)createEventfd()),
trig_func_(trig_func)
{
	eventfd_ = event_wrap_.getEventFd();
	event_wrap_.enableRead();
	event_wrap_.setEventsHandler(std::bind(&EventTrigger::handleEvents, this, std::placeholders::_1, std::placeholders::_2, this));
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
	LOG4CPLUS_DEBUG(_logger, "trig event!");
	uint64_t one = 1;
	uint64_t n = write(eventfd_, &one, sizeof one);
}

void EventTrigger::handleEvents(const int fd, const short events, void* arg)
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



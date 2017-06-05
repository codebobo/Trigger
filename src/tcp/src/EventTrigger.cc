#include "EventTrigger.h"
#include <sys/eventfd.h>
#include "EventHandler.h"
#include "unistd.h"

EventTrigger::EventTrigger(std::shared_ptr<EventLoop> loop_ptr)
:eventfd_(createEventfd()),
loop_ptr_(loop_ptr)
{
	if(eventfd_ > 0)
	{
		event_handler_ptr_ = std::make_shared<EventHandler>(loop_ptr_, eventfd_);
		if(event_handler_ptr_)
		{
			event_handler_ptr_->setReadCallback(std::bind(&EventTrigger::handleRead, this));
			event_handler_ptr_->enableRead();
			event_handler_ptr_->registerEvent();
		}
		else
		{
			abort();
		}
	}
	else
	{
		//LOG_FATAL<<"eventfd create failed";
	}
}

uint32_t EventTrigger::createEventfd()
{
	uint64_t evtfd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
	if (evtfd < 0)
	{
		//LOG_SYSERR << "Failed in eventfd";
		abort();
	}
	return evtfd;
}

void EventTrigger::trigEvent()
{
	uint64_t one = 1;
	uint64_t n = write(eventfd_, &one, sizeof one);
	if (n != sizeof one)
	{
		//LOG_ERROR << "EventLoop::wakeup() writes " << n << " bytes instead of 8";
	}
}

void EventTrigger::handleRead()
{
	uint64_t one = 1;
	uint64_t n = read(eventfd_, &one, sizeof one);
	if (n != sizeof one)
	{
		//LOG_ERROR << "EventLoop::handleRead() reads " << n << " bytes instead of 8";
	}
}



#include "EventWrap.h"
#include "EventLoop.h"
#include "Log.h"

void handleEventsCb(int fd, short events, void* arg)
{
	EventWrap* wrap = (EventWrap*)arg;

	short events_self = 0;
	if(events & EV_READ)
	{
		events_self ^= EVENT_READ;
	}
	if(events & EV_WRITE)
	{
		events_self ^= EVENT_WRITE;
	}
	wrap->handleEvents(fd, events_self);
}

EventWrap::EventWrap(EventLoop* loop_ptr, const int fd)
:loop_ptr_(loop_ptr),type_(EventType::COMMON_EVENT), state_(EventState::UNREGISTER), events_(0)
{
	ev_ = event_new(loop_ptr_->getEventBase(), fd, 0, handleEventsCb, this);
}

EventWrap::EventWrap(EventLoop* loop_ptr, const double interval)
:loop_ptr_(loop_ptr),type_(EventType::TIMER_EVENT), state_(EventState::UNREGISTER), events_(0), interval_(interval)
{
	ev_ = evtimer_new(loop_ptr_->getEventBase(), handleEventsCb, this);
}


EventWrap::~EventWrap()
{
	//event_del(ev_);
	event_free(ev_);
}

void EventWrap::enableRead()
{
	if(events_ & EVENT_READ)
	{
		return;
	}
	events_ ^= EVENT_READ;
	updateEvents();
}

void EventWrap::disableRead()
{
	if(!(events_ & EVENT_READ))
	{
		return;
	}
	events_ &= (~EVENT_READ);
	updateEvents();
}

void EventWrap::enableWrite()
{
	if(events_ & EVENT_WRITE)
	{
		return;
	}
	events_ ^= EVENT_WRITE;
	updateEvents();
}

void EventWrap::disableWrite()
{
	if(!(events_ & EVENT_WRITE))
	{
		return;
	}
	events_ &= (~EVENT_WRITE);
	updateEvents();
}

void EventWrap::registerEvent()
{
	if(state_ == EventState::REGISTERED)
	{
		LOG4CPLUS_WARN(_logger, "event has been registered, do not register again!");
		return;
	}
	if(loop_ptr_)
	{
		loop_ptr_->addEvent(this);
		state_ = EventState::REGISTERED;
	}
}

void EventWrap::unregisterEvent()
{
	if(state_ == EventState::UNREGISTER)
	{
		LOG4CPLUS_WARN(_logger, "event has been unregistered, do not unregister again!");
		return;
	}
	if(loop_ptr_)
	{
		//LOG4CPLUS_DEBUG(_logger, "remove event!");
		loop_ptr_->removeEvent(this);
		state_ = EventState::UNREGISTER;
	}
}

void EventWrap::handleEvents(const int fd, const short events)
{
	if(handler_ && state_ == EventState::REGISTERED)
	{
		handler_(fd, events);
	}
}

void EventWrap::updateEvents()
{
	if(state_ == EventState::REGISTERED)
	{
		loop_ptr_->runInLoop(std::bind(&EventWrap::reRegisterEvents, this));
	}
	else
	{
		assignEvents();
	}
}

void EventWrap::assignEvents()
{
	short evs_ = EV_PERSIST;
	if(events_ & EVENT_READ)
	{
		evs_ ^= EV_READ;
	}
	if(events_ & EVENT_WRITE)
	{
		evs_ ^= EV_WRITE;
	}
	event_assign(ev_, loop_ptr_->getEventBase(), getEventFd(), evs_, handleEventsCb, this);
}

void EventWrap::reRegisterEvents()
{
	unregisterEvent();
	assignEvents();
	registerEvent();
}


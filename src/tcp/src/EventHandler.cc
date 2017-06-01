#include "EventHandler.h"
#include "Log.h"

void handleEventsCb(int fd, short events, void* arg)
{
	LOG4CPLUS_DEBUG(_logger, "event callback!");
	EventHandler* handler = (EventHandler*)arg;
	if(handler)
	{
		handler->handleEvents(events);
	}
}

EventHandler::EventHandler(std::shared_ptr<EventLoop>& loop_ptr, const int fd)
:loop_ptr_(loop_ptr),events_(0),fd_(fd),evs_(EV_PERSIST),state_(EventState::UNREGISTER)
{
	ev_ = event_new(loop_ptr_->getEventBase(), fd_, evs_, NULL, NULL);
}

EventHandler::~EventHandler()
{
	if(ev_)
	{
		event_free(ev_);
	}
}

void EventHandler::enableRead()
{
	if(events_ & EVENT_READ)
	{
		return;
	}
	events_ ^= EVENT_READ;
	updateEvents();
}

void EventHandler::disableRead()
{
	if(!(events_ & EVENT_READ))
	{
		return;
	}
	events_ &= (~EVENT_READ);
	updateEvents();
}

void EventHandler::enableWrite()
{
	if(events_ & EVENT_WRITE)
	{
		return;
	}
	events_ ^= EVENT_WRITE;
	updateEvents();
}

void EventHandler::disableWrite()
{
	if(!(events_ & EVENT_WRITE))
	{
		return;
	}
	events_ &= (~EVENT_WRITE);
	updateEvents();
}

void EventHandler::registerEvent()
{
	if(state_ == EventState::REGISTERED)
	{
		LOG4CPLUS_WARN(_logger, "event has been registered, do not register again!");
		return;
	}
	if(loop_ptr_)
	{
		LOG4CPLUS_DEBUG(_logger, "register event: " <<evs_<<" fd: "<<fd_);
		loop_ptr_->addEvent(shared_from_this());
		state_ = EventState::REGISTERED;
	}
}

void EventHandler::unregisterEvent()
{
	if(state_ == EventState::REGISTERED)
	{
		LOG4CPLUS_WARN(_logger, "event has been unregistered, do not unregister again!");
		return;
	}
	if(loop_ptr_)
	{
		loop_ptr_->removeEvent(shared_from_this());
		state_ == EventState::UNREGISTER;
	}
}

void EventHandler::handleEvents(const short events)
{
	if(events & EV_READ)
	{
		if(readCallback_)
		{
			readCallback_();
		}
	}
	if(events & EV_WRITE)
	{
		if(writeCallback_)
		{
			writeCallback_();
		}
	}
}

void EventHandler::updateEvents()
{
	if(state_ == EventState::REGISTERED)
	{
		reRegisterEvents();
	}
	else
	{
		assignEvents();
	}
}

void EventHandler::assignEvents()
{
	evs_ = EV_PERSIST;
	if(events_ & EVENT_READ)
	{
		evs_ ^= EV_READ;
	}
	if(events_ & EVENT_WRITE)
	{
		evs_ ^= EV_WRITE;
	}
	
	event_assign(ev_, loop_ptr_->getEventBase(), fd_, evs_, handleEventsCb, this);
}

void EventHandler::reRegisterEvents()
{
	unregisterEvent();
	assignEvents();
	registerEvent();
}
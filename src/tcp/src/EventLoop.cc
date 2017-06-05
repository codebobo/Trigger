#include "EventLoop.h"
#include "EventHandler.h"
#include "EventTrigger.h"
#include "Log.h"

EventLoop::EventLoop()
{
	base_ = event_base_new();
	//event_trigger_ = new EventTrigger(shared_from_this());
}

EventLoop::~EventLoop()
{
	event_base_loopbreak(base_);
	event_base_free(base_);
}


void EventLoop::loop()
{
	if(base_)
	{
		event_base_dispatch(base_);
	}
	else
	{
		LOG4CPLUS_ERROR(_logger, "thiere is no valid event base!");
	}
}

void EventLoop::addEvent(std::shared_ptr<EventHandler> eventhandler_ptr)
{
	if(eventhandler_ptr)
	{
		LOG4CPLUS_DEBUG(_logger, "event add!");
		event_add(eventhandler_ptr->ev_, NULL);
	}	
}

void EventLoop::removeEvent(std::shared_ptr<EventHandler> eventhandler_ptr)
{
	if(eventhandler_ptr)
	{
		event_del(eventhandler_ptr->ev_);
	}	
}




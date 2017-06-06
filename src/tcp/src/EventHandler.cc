#include "EventHandler.h"
#include "Log.h"

EventHandler::EventHandler(EventLoop* loop_ptr, const int fd)
:EventHandlerAbstract(loop_ptr, fd)
{
	event_wrap_.setEventsHandler(std::bind(&EventHandler::handleEvents, this, std::placeholders::_1, std::placeholders::_2, this));
}

EventHandler::~EventHandler()
{
}

void EventHandler::enableRead()
{
	event_wrap_.enableRead();
}

void EventHandler::disableRead()
{
	event_wrap_.disableRead();
}

void EventHandler::enableWrite()
{
	event_wrap_.enableWrite();
}

void EventHandler::disableWrite()
{
	event_wrap_.disableWrite();
}

void EventHandler::handleEvents(const int fd, const short events, void * arg)
{
	if(events & EVENT_READ)
	{
		if(readCallback_)
		{
			readCallback_();
		}
	}
	if(events & EVENT_WRITE)
	{
		if(writeCallback_)
		{
			writeCallback_();
		}
	}
}
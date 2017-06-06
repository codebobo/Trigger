#ifndef EVENTHANDLERABSTRACT_H_
#define EVENTHANDLERABSTRACT_H_

#include <memory>
#include <iostream>
#include "event2/event.h"
#include "EventWrap.h"

class EventLoop;
class EventHandlerAbstract
{
	friend class EventLoop;
	public:
		EventHandlerAbstract(EventLoop* loop_ptr, const int fd):loop_ptr_(loop_ptr), event_wrap_(loop_ptr, fd){}
		EventHandlerAbstract(EventLoop* loop_ptr, const double interval):loop_ptr_(loop_ptr), event_wrap_(loop_ptr, interval){}
		void registerEvent()
		{
			event_wrap_.registerEvent();
		}
		void unregisterEvent()
		{
			event_wrap_.unregisterEvent();
		}
		virtual void handleEvents(const int fd, const short events, void* arg) = 0;

		EventLoop* loop_ptr_;
		EventWrap event_wrap_;
};

#endif


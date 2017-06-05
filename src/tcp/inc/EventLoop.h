#ifndef EVENTLOOP_H_
#define EVENTLOOP_H_

#include <memory>
#include <iostream>
#include "event2/event.h"
//#include "EventHandler.h"

class EventHandler;
class EventTrigger;
class EventLoop: public std::enable_shared_from_this<EventLoop>
{
	public:
		EventLoop();
		~EventLoop();
		void loop();
		void addEvent(std::shared_ptr<EventHandler> eventhandler_ptr);
		void removeEvent(std::shared_ptr<EventHandler> eventhandler_ptr);
		struct event_base* getEventBase()
		{
			if(base_ != NULL)
			{
				return base_;
			}
			else
			{
				std::cout<<"base null"<<std::endl;
			}
			
		}

	private:
		struct event_base* base_;
		EventTrigger* event_trigger_;
};

#endif
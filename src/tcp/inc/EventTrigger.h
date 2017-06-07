#ifndef EVENT_TRIGGER_H_
#define EVENT_TRIGGER_H_

#include <stdint.h>
#include <memory>
#include "EventHandler.h"
#include "Log.h"
#include "EventHandlerAbstract.h"

class EventLoop;
class EventTrigger
{
public:
	friend class EventLoop;
	 EventTrigger(EventLoop* loop_ptr, std::function<void()> trig_func);
	void trigEvent();
	void registerEvent()
	{
		event_handler_.registerEvent();
	}

private:
	uint32_t createEventfd();
	uint32_t eventfd_;
	std::function<void()> trig_func_;
	EventHandler event_handler_;

	void handleEvents();
};

#endif

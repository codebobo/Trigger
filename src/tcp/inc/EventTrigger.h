#ifndef EVENT_TRIGGER_H_
#define EVENT_TRIGGER_H_

#include <stdint.h>
#include <memory>
//#include "EventHandler.h"

class EventHandler;
class EventLoop;
class EventTrigger
{
public:
	 EventTrigger(std::shared_ptr<EventLoop> loop_ptr);
	void trigEvent();
	void handleRead();
	//shared_ptr<TrantorPipe> getEventPipe() const{return event_pipe_sp_;}
private:
	uint32_t createEventfd();
	uint32_t eventfd_;
	std::shared_ptr<EventHandler> event_handler_ptr_;
	std::shared_ptr<EventLoop> loop_ptr_;
};

#endif

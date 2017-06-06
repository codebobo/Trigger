#ifndef EVENT_TRIGGER_H_
#define EVENT_TRIGGER_H_

#include <stdint.h>
#include <memory>
#include "EventWrap.h"
#include "Log.h"
#include "EventHandlerAbstract.h"

class EventLoop;
class EventTrigger : public EventHandlerAbstract
{
public:
	friend class EventLoop;
	 EventTrigger(EventLoop* loop_ptr, std::function<void()> trig_func);
	void trigEvent();

private:
	uint32_t createEventfd();
	uint32_t eventfd_;
	std::function<void()> trig_func_;

	virtual void handleEvents(const int fd, const short events, void* arg);
};

#endif

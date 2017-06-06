#ifndef TIMERHANDLER_H_
#define TIMERHANDLER_H_

#include <memory>
#include <iostream>
#include "event2/event.h"
#include "EventHandlerAbstract.h"


class EventLoop;
class TimerHandler : public EventHandlerAbstract
{
	friend class EventLoop;
	public:
		TimerHandler(EventLoop* loop_ptr, std::function<void()> cb, const double interval);
		~TimerHandler();


	private:
		virtual void handleEvents(int fd, short events, void* arg);
		std::function<void()> cb_;
};

#endif


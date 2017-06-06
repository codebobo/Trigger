#ifndef TIMERMANAGER_H_
#define TIMERMANAGER_H_

#include <memory>
#include <iostream>
#include "event2/event.h"
#include "TrantorTimestamp.h"


class EventLoop;
class TimerManager
{
	friend class EventLoop;
	public:
		TimerManager(EventLoop* loop_ptr);
		~TimerManager();
		void runAfter(std::function<void()> cb, const double interval);
		void runEvery(std::function<void()> cb, const double interval);
		void runAt(std::function<void()> cb, const TrantorTimestamp timestamp);

	private:
		void registerTimer(std::function<void()> cb, double interval);
		EventLoop* loop_ptr_;
};

#endif


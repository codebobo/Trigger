#ifndef TRANTOR_TIMER_LOOP_H_
#define TRANTOR_TIMER_LOOP_H_

#include "EventLoop.h"
#include "EventLoopThread.h"
#include <functional>

class TrantorTimerLoop
{
public:
	typedef std::function<void()> TimerCallback;
	static TrantorTimerLoop& Instance()
	{
		std::call_once(once_, &TrantorTimerLoop::init);
		return *timer_loop_ptr_;
	}
	void runAt(const TrantorTimestamp& time, const TimerCallback& cb)
	{
		loop_ptr_->runAt(cb, time);
	}
	void runAfter(const double interval, const TimerCallback& cb)
	{
		loop_ptr_->runAfter(cb, interval);
	}
	void runEvery(const double interval, const TimerCallback& cb)
	{
		loop_ptr_->runEvery(cb, interval);
	}
private:

	static std::once_flag once_;
	static TrantorTimerLoop* timer_loop_ptr_;
	static void init()
	{
		if(!timer_loop_ptr_)
		{
			timer_loop_ptr_ = new TrantorTimerLoop;
		}
	}

	explicit TrantorTimerLoop()
	{
		loop_ptr_ = loop_thread_.getLoop();
		loop_thread_.start();
	}
	EventLoopThread loop_thread_;
	EventLoop* loop_ptr_;
};

#endif



#ifndef EVENTLOOP_H_
#define EVENTLOOP_H_

#include <thread>
#include <vector>
#include <mutex>
#include <memory>
#include <iostream>
#include "event2/event.h"
#include "TrantorTimestamp.h"
#include "EventWrap.h"
#include "TimerManager.h"

class EventTrigger;
class EventLoop
{
	public:
		EventLoop();
		~EventLoop();
		void loop();
		bool isInLoopThread() const 		
		{ 			
			return loop_thread_id_ == std::this_thread::get_id(); 		
		}
		void addEvent(EventWrap* eventwrap_ptr);
		void removeEvent(EventWrap* eventwrap_ptr);

		void runInLoop(std::function<void()> func);
		void runAfter(std::function<void()> cb, const double interval)
		{
			timer_manager_->runAfter(cb,interval);
		}
		void runEvery(std::function<void()> cb, const double interval)
		{
			timer_manager_->runEvery(cb,interval);
		}
		void runAt(std::function<void()> cb, const TrantorTimestamp timestamp)
		{
			timer_manager_->runAt(cb,timestamp);
		}
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
		TimerManager* timer_manager_;
		std::thread::id loop_thread_id_;
		bool need_wakeup_;
		std::mutex mtx_;
		std::vector<std::function<void ()> > loop_funcs_list_;

		void runLoopFuncs();
		void queueInLoop(std::function<void()> func);	
		void wakeup();
		void addEventInLoop(EventWrap* eventwrap_ptr);
		void removeEventInLoop(EventWrap* eventwrap_ptr);
};

#endif
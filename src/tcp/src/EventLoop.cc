#include "EventLoop.h"
#include "EventHandler.h"
#include "EventTrigger.h"
#include "Log.h"

EventLoop::EventLoop()
:need_wakeup_(false)
{
	base_ = event_base_new();
	event_trigger_ = new EventTrigger(this, std::bind(&EventLoop::runLoopFuncs, this));
	timer_manager_ = new TimerManager(this);
}

EventLoop::~EventLoop()
{
	event_base_loopbreak(base_);
	event_base_free(base_);
}


void EventLoop::loop()
{
	LOG4CPLUS_DEBUG(_logger, "begin loop!");
	loop_thread_id_ = std::this_thread::get_id();
	event_trigger_->registerEvent();
	if(base_)
	{
		event_base_dispatch(base_);
	}
	else
	{
		LOG4CPLUS_ERROR(_logger, "thiere is no valid event base!");
	}
}


void EventLoop::addEvent(EventWrap* eventwrap_ptr)
{
	runInLoop(std::bind(&EventLoop::addEventInLoop, this, eventwrap_ptr));
}

void EventLoop::removeEvent(EventWrap* eventwrap_ptr)
{
	runInLoop(std::bind(&EventLoop::removeEventInLoop, this, eventwrap_ptr));	
}

void EventLoop::addEventInLoop(EventWrap* eventwrap_ptr)
{
	if(eventwrap_ptr)
	{
		if(eventwrap_ptr->type_ == EventType::COMMON_EVENT)
		{
			LOG4CPLUS_DEBUG(_logger, "add event in loop!");
			event_add(eventwrap_ptr->ev_, NULL);
		}
		if(eventwrap_ptr->type_ == EventType::TIMER_EVENT)
		{
			LOG4CPLUS_DEBUG(_logger, "add timer in loop!");
			TrantorTimestamp interval(eventwrap_ptr->interval_ * 1000000);
			auto st = interval.getTimeval();
			event_add(eventwrap_ptr->ev_, &st);
		}
	}	
}

void EventLoop::removeEventInLoop(EventWrap* eventwrap_ptr)
{
	if(eventwrap_ptr)
	{
		event_del(eventwrap_ptr->ev_);
	}	
}

void EventLoop::runLoopFuncs()
{
	need_wakeup_ = true;		
	std::vector<std::function<void ()> > funcs;		
	{			
		std::lock_guard<std::mutex> lock(mtx_);			
		funcs.swap(loop_funcs_list_);		
	}		
	for(int i = 0; i < funcs.size(); i++)		
	{			
		if(funcs[i])			
		{				
		funcs[i]();			
		}		
	}		
	need_wakeup_ = false;	
}


void EventLoop::runInLoop(std::function<void()> func)	
{		
	if(func)		
	{			
		if(isInLoopThread())			
		{				
			func();			
		}			
		else			
		{				
			queueInLoop(func);			
		}		
	}	
}

void EventLoop::queueInLoop(std::function<void()> func)	
{			
	{			
		std::lock_guard<std::mutex> lock(mtx_);			
		loop_funcs_list_.push_back(func);		
	}		
	if(!isInLoopThread() || need_wakeup_)		
	{					
		wakeup();		
	}	
}	

void EventLoop::wakeup()	
{		
	event_trigger_->trigEvent();	
}





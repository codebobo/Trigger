#ifndef EVENTLOOPTHREADPOOL_H_
#define EVENTLOOPTHREADPOOL_H_

#include <thread>
#include "EventLoopThread.h"

class EventLoopThreadPool
{
	public:
		EventLoopThreadPool(const int thread_num);
		EventLoop* getNextLoop();
		
		
	private:
		std::vector<EventLoopThread*> loop_pool_;
		int current_index_;
};

#endif


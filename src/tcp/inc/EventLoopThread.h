#ifndef EVENTLOOPTHREAD_H_
#define EVENTLOOPTHREAD_H_

#include <thread>
#include "EventLoop.h"

class EventLoopThread
{
	public:
		void start();
		void threadFunc();
		EventLoop* getLoop();
		
	private:
		EventLoop loop_;
		std::thread* loop_thread_;
};

#endif
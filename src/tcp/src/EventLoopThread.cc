#include "EventLoopThread.h"


EventLoop* EventLoopThread::getLoop()
{
	return &loop_;
}

void EventLoopThread::start()
{
	loop_thread_ = new std::thread(std::bind(&EventLoopThread::threadFunc, this)); 
}

void EventLoopThread::threadFunc()
{
	loop_.loop();
}
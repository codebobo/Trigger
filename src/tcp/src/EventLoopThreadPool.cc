#include "EventLoopThreadPool.h"

EventLoopThreadPool::EventLoopThreadPool(const int thread_num)
{
	for(int i = 0; i < thread_num; i++)
	{
		EventLoopThread* loop_thread = new EventLoopThread;
		loop_thread->start();
		loop_pool_.push_back(loop_thread);
	}
	current_index_ = 0;
}

EventLoop* EventLoopThreadPool::getNextLoop()
{
	current_index_ = (current_index_ + 1) % loop_pool_.size();
	return loop_pool_[current_index_]->getLoop();
}



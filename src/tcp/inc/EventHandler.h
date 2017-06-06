#ifndef EVENTHANDLER_H_
#define EVENTHANDLER_H_

#include <functional>
#include "event2/event.h"
#include "EventHandlerAbstract.h"

class EventLoop;
class EventHandler : public EventHandlerAbstract
{
	friend class EventLoop;
	
	public:
		EventHandler(EventLoop* loop_ptr, const int fd);
		~EventHandler();
		void enableWrite();
		void enableRead();
		void disableWrite();
		void disableRead();
		void setReadCallback(const std::function<void()>& cb)
		{
			readCallback_ = cb;
		}
		void setWriteCallback(const std::function<void()>& cb)
		{
			writeCallback_ = cb;
		}
		void setCloseCallback(const std::function<void()>& cb)
		{
			closeCallback_ = cb;
		}
		void handleEvents(const int fd, const short events, void* arg);

	private:
		std::function<void()> readCallback_;
		std::function<void()> writeCallback_;
		std::function<void()> closeCallback_;
};

#endif

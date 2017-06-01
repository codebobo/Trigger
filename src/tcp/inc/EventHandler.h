#ifndef EVENTHANDLER_H_
#define EVENTHANDLER_H_

#include <functional>
#include "event2/event.h"
#include "EventLoop.h"

#define EVENT_READ 0x01
#define EVENT_WRITE 0x02

//TODO add registered and unregistered state
class EventHandler:public std::enable_shared_from_this<EventHandler>
{
	friend class EventLoop;
	enum class EventState{REGISTERED, UNREGISTER};
	public:
		EventHandler(std::shared_ptr<EventLoop>& loop_ptr, const int fd);
		~EventHandler();
		void enableWrite();
		void enableRead();
		void disableWrite();
		void disableRead();
		void registerEvent();
		void unregisterEvent();
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
		void handleEvents(const short events);

	private:
		std::shared_ptr<EventLoop> loop_ptr_;
		struct event* ev_;
		short events_;
		short evs_;
		int fd_;
		std::function<void()> readCallback_;
		std::function<void()> writeCallback_;
		std::function<void()> closeCallback_;
		EventState state_;

		void reRegisterEvents();
		void updateEvents();
		void assignEvents();
		
};

#endif

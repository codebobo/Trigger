#ifndef EVENTWRAP_H_
#define EVENTWRAP_H_

#include <functional>
#include "event2/event.h"

#define EVENT_READ 0x01
#define EVENT_WRITE 0x02

enum class EventType{TIMER_EVENT, COMMON_EVENT, SIGNAL_EVENT};
enum class EventState{REGISTERED, UNREGISTER};
class EventLoop;

class EventWrap
{
	friend class EventLoop;
	
	public:
		EventWrap(EventLoop* loop_ptr, const int fd);
		EventWrap(EventLoop* loop_ptr, const double interval);
		~EventWrap();
		void enableWrite();
		void enableRead();
		void disableWrite();
		void disableRead();
		void registerEvent();
		void unregisterEvent();
		void setEventsHandler(std::function<void(const int fd, const short events)> handler)
		{
			handler_ = handler;
		}
		int getEventFd()
		{
			if(type_ == EventType::COMMON_EVENT)
			{
				return event_get_fd(ev_);
			}
		}
		short getEvents()
		{
			return event_get_events(ev_);
		}
		void handleEvents(const int fd, const short events);


	private:
		EventLoop* loop_ptr_;
		struct event* ev_;
		short events_;
		EventType type_;
		EventState state_;
		double interval_;
		std::function<void(const int fd, const short events)> handler_;

		void reRegisterEvents();
		void updateEvents();
		void assignEvents();
};

#endif


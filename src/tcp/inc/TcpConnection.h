#ifndef TCPCONNECTION_H_
#define TCPCONNECTION_H_

#include <functional>
#include "SocketOperator.h"
#include "EventHandler.h"
#include "StringBuffer.h"


class TcpConnection: public std::enable_shared_from_this<TcpConnection>
{
	public:
		TcpConnection(std::shared_ptr<EventLoop> loop_ptr, const int fd);
		~TcpConnection();
		void write(const char* addr, const long len);
		void enableRead()
		{
			event_handler_ptr_->enableRead();
		}
		void setReadCallback(std::function<void(std::shared_ptr<TcpConnection>)> cb)
		{
			readCallback_ = cb;
		}
		
		void unregisterFromLoop()
		{
			event_handler_ptr_->unregisterEvent();
		}
		void registerIntoLoop()
		{
			event_handler_ptr_->registerEvent();
		}
		int getFd(){return fd_;}
		std::shared_ptr<StringBuffer> getReadBufferPtr(){return read_buffer_ptr_;}

		void shutdownWrite();
		void forceClose();

	private:
		int fd_;
		SocketOperator socket_operator_;
		std::shared_ptr<EventHandler> event_handler_ptr_;
		std::shared_ptr<StringBuffer> read_buffer_ptr_;
		std::shared_ptr<StringBuffer> write_buffer_ptr_;
		std::function<void (std::shared_ptr<TcpConnection>) > readCallback_;
		std::function<void (std::shared_ptr<TcpConnection>) > closedCallback_;
		std::function<void ()> shutdown_write_callback_;
		std::shared_ptr<EventLoop> loop_ptr_;

		void closedByPeerCallback();
		void writeCallback();
		void readCallback();
};

#endif

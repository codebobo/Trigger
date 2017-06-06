#ifndef TCPCONNECTION_H_
#define TCPCONNECTION_H_

#include <functional>
#include "SocketOperator.h"
#include "EventHandler.h"
#include "StringBuffer.h"
#include "TrantorTimestamp.h"


class TcpConnection: public std::enable_shared_from_this<TcpConnection>
{
	public:
		TcpConnection(EventLoop* loop_ptr, const int fd, const std::string connection_id);
		~TcpConnection();
		void write(const char* addr, const long len);
		void enableRead()
		{
			event_handler_ptr_->enableRead();
		}
		void setReadCallback(std::function<void(std::shared_ptr<TcpConnection>, const TrantorTimestamp)> cb)
		{
			readCallback_ = cb;
		}
		void setClosedCallback(std::function<void(std::shared_ptr<TcpConnection>)> cb)
		{
			closedCallback_ = cb;
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
		const std::string getConnId(){return connection_id_;}
		std::shared_ptr<StringBuffer> getReadBufferPtr(){return read_buffer_ptr_;}

		void shutdownWrite();
		void forceClose();

	private:
		int fd_;
		SocketOperator socket_operator_;
		std::shared_ptr<EventHandler> event_handler_ptr_;
		std::shared_ptr<StringBuffer> read_buffer_ptr_;
		std::shared_ptr<StringBuffer> write_buffer_ptr_;
		std::function<void (std::shared_ptr<TcpConnection>, const TrantorTimestamp) > readCallback_;
		std::function<void (std::shared_ptr<TcpConnection>) > closedCallback_;
		std::function<void ()> shutdown_write_callback_;
		EventLoop* loop_ptr_;
		std::string connection_id_;

		void closedByPeerCallback();
		void writeCallback();
		void readCallback();
};

#endif

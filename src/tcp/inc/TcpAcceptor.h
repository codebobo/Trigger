#ifndef TCPACCEPTOR_H_
#define TCPACCEPTOR_H_

#include <functional>
#include <memory>
#include "SocketOperator.h"
#include "EventHandler.h"
#include "StringBuffer.h"
#include "EventLoop.h"
#include "TcpConnection.h"
#include "EventLoopThreadPool.h"
#include "Log.h"


class TcpAcceptor:public std::enable_shared_from_this<TcpAcceptor>
{
	typedef std::function<void(std::shared_ptr<TcpConnection>&)> ReadCallback;
	public:
		TcpAcceptor(EventLoop* main_loop_ptr);
		int init(const std::string& sever_addr, const int server_port);
		void setReadCallback(ReadCallback cb)
		{
			readCallback_ = cb;
		}
		void enableRead()
		{
			event_handler_ptr_->enableRead();
		}
		
		void unregisterFromLoop()
		{
			event_handler_ptr_->unregisterEvent();
		}
		void registerIntoLoop()
		{
			event_handler_ptr_->registerEvent();
		}
		void setWorkLoopPool(EventLoopThreadPool* work_thread_pool)
		{
			
			work_thread_pool_ = work_thread_pool;
		}

	private:
		SocketOperator socket_operator_;
		std::shared_ptr<EventHandler> event_handler_ptr_;
		ReadCallback readCallback_;
		EventLoop* main_loop_ptr_;
		EventLoopThreadPool* work_thread_pool_;
		int listener_;

		void readSocket();
};

#endif


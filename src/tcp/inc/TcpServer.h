#ifndef TCPSERVER_H_
#define TCPSERVER_H_

#include <map>
#include "EventLoop.h"
#include "TcpAcceptor.h"
#include "TcpConnection.h"
#include "TrantorTimestamp.h"
#include "EventLoopThreadPool.h"


class TcpServer
{
	public:
		TcpServer(EventLoop* loop_ptr);
		TcpServer();
		int init(const std::string& server_addr, const int server_port);
		void start();
		void setNewConnectionCallback(const std::function<void(std::shared_ptr<TcpConnection>)> cb)
		{
			newConnectionCallback_ = cb;
		}
		void setNewMessageCallback(const std::function<void(std::shared_ptr<TcpConnection>, const TrantorTimestamp)> cb)
		{
			newMessageCallback_ = cb;
		}
		void setWorkThreadNum(const int thread_num)
		{
			work_loop_pool_ = new EventLoopThreadPool(thread_num);
		}
		
	private:
		EventLoop* main_loop_ptr_;
		EventLoopThreadPool* work_loop_pool_;
		std::shared_ptr<TcpAcceptor> tcp_acceptor_ptr_;
		std::map<const std::string, std::shared_ptr<TcpConnection> > tcp_connection_map_;
		std::function<void(std::shared_ptr<TcpConnection>)> newConnectionCallback_;
		std::function<void(std::shared_ptr<TcpConnection>, const TrantorTimestamp)> newMessageCallback_;
		std::function<void(std::shared_ptr<TcpConnection>)> closeCallback_;
		void newConnectionCallback(std::shared_ptr<TcpConnection>&);
		void newMessageCallback(std::shared_ptr<TcpConnection>, const TrantorTimestamp);
		void closeCallback(std::shared_ptr<TcpConnection>);
};
		

#endif

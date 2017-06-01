#ifndef TCPSERVER_H_
#define TCPSERVER_H_

#include <map>
#include "EventLoop.h"
#include "TcpAcceptor.h"
#include "TcpConnection.h"


class TcpServer
{
	public:
		TcpServer();
		int init(const std::string& server_addr, const int server_port);
		void start();
		void setNewConnectionCallback(const std::function<void(std::shared_ptr<TcpConnection>)> cb)
		{
			newConnectionCallback_ = cb;
		}
		void setNewMessageCallback(const std::function<void(std::shared_ptr<TcpConnection>)> cb)
		{
			newMessageCallback_ = cb;
		}
		
	private:
		std::shared_ptr<EventLoop> loop_ptr_;
		std::shared_ptr<TcpAcceptor> tcp_acceptor_ptr_;
		std::map<int, std::shared_ptr<TcpConnection> > tcp_connection_map_;
		std::function<void(std::shared_ptr<TcpConnection>)> newConnectionCallback_;
		std::function<void(std::shared_ptr<TcpConnection>)> newMessageCallback_;
		void newConnection(std::shared_ptr<TcpConnection>&);
		void newMessage(std::shared_ptr<TcpConnection>);
};
		

#endif

#include <arpa/inet.h>
#include "TcpAcceptor.h"
#include "Log.h"

TcpAcceptor::TcpAcceptor(std::shared_ptr<EventLoop> loop_ptr)
:loop_ptr_(loop_ptr)
{	
}

int TcpAcceptor::init(const std::string& sever_addr, const int server_port)
{
	listener_ = socket_operator_.createSocket();
	if(listener_ < 0)
	{
		return -1;
	}
	if(socket_operator_.bind(listener_, sever_addr, server_port) < 0)
	{
		return -1;
	}
	if(socket_operator_.listen(listener_, 100) < 0)
	{
		return -1;
	}
	event_handler_ptr_ = std::make_shared<EventHandler>(loop_ptr_, listener_);
	if(event_handler_ptr_)
	{
		event_handler_ptr_->setReadCallback(std::bind(&TcpAcceptor::readSocket, this));
	}
}

void TcpAcceptor::readSocket()
{
	int sockfd = socket_operator_.accept(listener_);
	if(sockfd > 0)
	{
		LOG4CPLUS_INFO(_logger, "accept new tcp connection");
		std::shared_ptr<TcpConnection> tcp_connection_ptr = std::make_shared<TcpConnection>(loop_ptr_, sockfd);
		if(tcp_connection_ptr && readCallback_)
		{
			readCallback_(tcp_connection_ptr);
		}
	}
	else
	{
		LOG4CPLUS_ERROR(_logger, "accept new tcp connection failed!");
	}
}



#include <arpa/inet.h>
#include "TcpAcceptor.h"
#include "Funcs.h"

TcpAcceptor::TcpAcceptor(EventLoop* main_loop_ptr)
:main_loop_ptr_(main_loop_ptr), work_thread_pool_(NULL)
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
	event_handler_ptr_ = std::make_shared<EventHandler>(main_loop_ptr_, listener_);
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
		std::shared_ptr<TcpConnection> tcp_connection_ptr;
		if(work_thread_pool_ != NULL)
		{
			EventLoop* work_loop_ptr = work_thread_pool_->getNextLoop();
			tcp_connection_ptr = std::make_shared<TcpConnection>(work_loop_ptr, sockfd, getuuid());
		}
		else
		{
			tcp_connection_ptr = std::make_shared<TcpConnection>(main_loop_ptr_, sockfd, getuuid());
		}
		
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



#include "TcpServer.h"
#include "Log.h"

TcpServer::TcpServer()
{
	main_loop_ptr_ = NULL;
	work_loop_pool_ = NULL;
}

TcpServer::TcpServer(EventLoop* main_loop_ptr)
{
	main_loop_ptr_ = main_loop_ptr;
	work_loop_pool_ = NULL;
}

int TcpServer::init(const std::string& server_addr, const int server_port)
{
	if(!main_loop_ptr_)
	{
		main_loop_ptr_ = new EventLoop;
	}
	
	if(main_loop_ptr_)
	{
		tcp_acceptor_ptr_ = std::make_shared<TcpAcceptor>(main_loop_ptr_);
		if(tcp_acceptor_ptr_)
		{
			if(tcp_acceptor_ptr_->init(server_addr, server_port) < 0)
			{
				LOG4CPLUS_INFO(_logger, "tcp server init failed");
				return -1;
			}
			tcp_acceptor_ptr_->setReadCallback(std::bind(&TcpServer::newConnectionCallback, this, std::placeholders::_1));
			tcp_acceptor_ptr_->enableRead();
			tcp_acceptor_ptr_->registerIntoLoop();
		}
		else
		{
			LOG4CPLUS_INFO(_logger, "tcp server init failed");
			return -1;
		}
	}
	else
	{
		LOG4CPLUS_INFO(_logger, "tcp server init failed");
		return -1;
	}
	LOG4CPLUS_INFO(_logger, "tcp server init successfully");
	return 0;
}

void TcpServer::start()
{
	if(main_loop_ptr_)
	{
		tcp_acceptor_ptr_->setWorkLoopPool(work_loop_pool_);
		LOG4CPLUS_INFO(_logger, "start tcp server") ;
		main_loop_ptr_->loop();
	}
	else
	{
		LOG4CPLUS_WARN(_logger, "event loop ptr null! please create a event loop first!") ;
	}
}

void TcpServer::newConnectionCallback(std::shared_ptr<TcpConnection>& tcp_connection_ptr)
{
	if(tcp_connection_ptr)
	{
		LOG4CPLUS_INFO(_logger, "accept new tcp connection in main loop");
		if(newConnectionCallback_)
		{
			newConnectionCallback_(tcp_connection_ptr);
		}
		
		tcp_connection_ptr->setReadCallback(std::bind(&TcpServer::newMessageCallback, this, std::placeholders::_1, std::placeholders::_2));
		tcp_connection_ptr->setClosedCallback(std::bind(&TcpServer::closeCallback, this, std::placeholders::_1));
		tcp_connection_ptr->enableRead();
		tcp_connection_map_[tcp_connection_ptr->getConnId()] = tcp_connection_ptr;

		tcp_connection_ptr->registerIntoLoop();
	}
}

void TcpServer::newMessageCallback(std::shared_ptr<TcpConnection> tcp_connection_ptr, const TrantorTimestamp timestamp)
{
	LOG4CPLUS_DEBUG(_logger, "new message received: ") ;
	//LOG4CPLUS_DEBUG(_logger, "new message received: "<<tcp_connection_ptr->getReadBufferPtr()->retrieveAllAsString()) ;
	//tcp_connection_ptr->write("Hello!", 6);
	//tcp_connection_ptr->forceClose();
	if(tcp_connection_ptr && newMessageCallback_)
	{
		newMessageCallback_(tcp_connection_ptr, timestamp);
	}
}

void TcpServer::closeCallback(std::shared_ptr<TcpConnection> tcp_connection_ptr)
{
	LOG4CPLUS_DEBUG(_logger, "erase connection: "<<tcp_connection_ptr->getConnId()) ;
	main_loop_ptr_->runInLoop(std::bind(&TcpServer::closeCallbackInMainLoop, this, tcp_connection_ptr));
}

void TcpServer::closeCallbackInMainLoop(std::shared_ptr<TcpConnection> tcp_connection_ptr)
{
	LOG4CPLUS_DEBUG(_logger, "erase connection in main loop: "<<tcp_connection_ptr->getConnId()) ;
	tcp_connection_map_.erase(tcp_connection_ptr->getConnId());
	if(closeCallback_)
	{
		closeCallback_(tcp_connection_ptr);
	}
}
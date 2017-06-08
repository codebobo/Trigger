#include "TcpConnection.h"
#include "EventLoop.h"
#include "Log.h"
#include "TrantorTimestamp.h"

TcpConnection::TcpConnection(EventLoop* loop_ptr, const int fd, const std::string connection_id)
:loop_ptr_(loop_ptr), fd_(fd),connection_id_(connection_id)
{
	event_handler_ptr_ = std::make_shared<EventHandler>(loop_ptr, fd);
	read_buffer_ptr_ = std::make_shared<StringBuffer>();
	write_buffer_ptr_ = std::make_shared<StringBuffer>();
	if(event_handler_ptr_ && read_buffer_ptr_ && write_buffer_ptr_)
	{
		event_handler_ptr_->setReadCallback(std::bind(&TcpConnection::readCallback, this));
		event_handler_ptr_->setWriteCallback(std::bind(&TcpConnection::writeCallback, this));
	}
	else
	{
		LOG4CPLUS_ERROR(_logger, "TcpConnection init failed") ;
	}
}

TcpConnection::~TcpConnection()
{
	if(fd_ > 0)
	{
		LOG4CPLUS_INFO(_logger, "close fd") ;
		socket_operator_.close(fd_);
	}
}


void TcpConnection::write(const char* addr, const long len)
{
	write_buffer_ptr_->writeBuffer(addr,len);
	event_handler_ptr_->enableWrite();
}

void TcpConnection::readCallback()
{
	int length = socket_operator_.read(fd_, read_buffer_ptr_);
	LOG4CPLUS_INFO(_logger, "read bytes: "<<length) ;
	if(length > 0)
	{
		if(readCallback_)
		{
			readCallback_(shared_from_this(), TrantorTimestamp::now());
		}
	}
	else
	{
		LOG4CPLUS_INFO(_logger, "TcpConnection closed by peer") ;
		closedByPeerCallback();
	}
}

void TcpConnection::writeCallback()
{
	socket_operator_.write(fd_, write_buffer_ptr_);
	if(write_buffer_ptr_->getReadableBytes() <= 0)
	{
		event_handler_ptr_->disableWrite();
		if(shutdown_write_callback_)
		{
			shutdown_write_callback_();
			shutdown_write_callback_ = NULL;
		}
	}
}

//closed by peer, unregister event and close fd in dtor
void TcpConnection::closedByPeerCallback()
{
	event_handler_ptr_->unregisterEvent();
	if(closedCallback_)
	{
		closedCallback_(shared_from_this());
	}
}

void TcpConnection::shutdownWrite()
{
	loop_ptr_->runInLoop(std::bind(&TcpConnection::shutdownWriteInLoop, this));
}

void TcpConnection::shutdownWriteInLoop()
{
	if(write_buffer_ptr_->getReadableBytes() > 0)
	{
		shutdown_write_callback_ = [=]()
		{
			socket_operator_.shutdownWrite(fd_);
		};
	}
	else
	{
		socket_operator_.shutdownWrite(fd_);
	}
}

void TcpConnection::forceClose()
{
	loop_ptr_->runInLoop(std::bind(&TcpConnection::forceCloseInLoop, this));
}

void TcpConnection::forceCloseInLoop()
{
	if(write_buffer_ptr_->getReadableBytes() > 0)
	{
		shutdown_write_callback_ = [=]()
		{
			if(closedCallback_)
			{
				closedCallback_(shared_from_this());
			}
		};
	}
	else
	{
		if(closedCallback_)
		{
			closedCallback_(shared_from_this());
		}
	}
}






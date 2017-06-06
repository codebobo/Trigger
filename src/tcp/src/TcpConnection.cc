#include "TcpConnection.h"
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
		event_handler_ptr_->setCloseCallback(std::bind(&TcpConnection::closedByPeerCallback, this));
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
		if(closedCallback_)
		{
			closedCallback_(shared_from_this());
		}
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

void TcpConnection::closedByPeerCallback()
{
	socket_operator_.shutdownWrite(fd_);
	if(closedCallback_)
	{
		closedCallback_(shared_from_this());
	}
}

void TcpConnection::shutdownWrite()
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
	if(write_buffer_ptr_->getReadableBytes() > 0)
	{
		shutdown_write_callback_ = [=]()
		{
			socket_operator_.close(fd_);
			fd_ = -1;
			if(closedCallback_)
			{
				closedCallback_(shared_from_this());
			}
		};
	}
	else
	{
		socket_operator_.close(fd_);
		fd_ = -1;
		if(closedCallback_)
		{
			closedCallback_(shared_from_this());
		}
	}
}






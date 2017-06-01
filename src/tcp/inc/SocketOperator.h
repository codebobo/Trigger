#ifndef SOCKETOPERATOR_H_
#define SOCKETOPERATOR_H_

#include <memory>
#include "unistd.h"
#include "StringBuffer.h"

class SocketOperator
{
	public:
		int createSocket();
		int bind(const int fd, const std::string& sever_addr, const int server_port);
		int listen(const int fd, const int queue_length);
		int accept(const int fd);
		void write(const int fd, std::shared_ptr<StringBuffer>& buffer);
		int read(const int fd, std::shared_ptr<StringBuffer>& buffer);
		void shutdownWrite(const int fd);
		void close(const int fd);
};

#endif
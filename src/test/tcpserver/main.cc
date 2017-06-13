#include "TcpServer.h"
#include "Log.h"

int main()
{
	initLog();
	
	TcpServer tcp_server;
	if(tcp_server.init("192.168.12.223", 2222) >= 0)
	{
		tcp_server.setNewMessageCallback([](std :: shared_ptr < TcpConnection > conn_ptr, const TrantorTimestamp timestamp)
											{
												conn_ptr->write("Hello! Welcome to tcp server!");
											}
										);
		tcp_server.setWorkThreadNum(10);
		tcp_server.start();
	}
	/*
	EventLoop loop;
	loop.runEvery([](){LOG4CPLUS_ERROR(_logger, "timer every test 1!");},1.0);
	loop.runEvery([](){LOG4CPLUS_ERROR(_logger, "timer every test 2!");},2.0);
	loop.runAfter([](){LOG4CPLUS_ERROR(_logger, "timer After test 3!");},3.0);
	loop.loop();
	*/
}

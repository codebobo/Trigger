#include "TcpServer.h"
#include "Log.h"

int main()
{
	initLog();
	TcpServer tcp_server;
	if(tcp_server.init("192.168.12.223", 2222) >= 0)
	{
		tcp_server.start();
	}
}

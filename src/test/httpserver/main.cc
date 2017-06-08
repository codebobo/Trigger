#include "HttpFramework.h"
#include "Log.h"

int main()
{
	initLog();
	
	HttpFramework httpServer("192.168.12.223", 2222);	
	httpServer.initWithConfigFile("../http.cfg");	
	httpServer.start();	
}


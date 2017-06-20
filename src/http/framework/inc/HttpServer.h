#ifndef HTTPSERVER_H_
#define HTTPSERVER_H_

#include <TcpServer.h>
#include <functional>
#include <string>
#include "TrantorTimestamp.h"

using std::string;

class HttpRequest;
class HttpResponse;


class HttpServer
{
 public:
  typedef std::function< void (const HttpRequest&,std::function<void (HttpResponse &)>)> HttpAsyncCallback;
  typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
  HttpServer(EventLoop* loop, const std::string& server_addr, const int server_port);

  ~HttpServer();  // force out-line dtor, for scoped_ptr members.

  EventLoop* getLoop() { return server_.getMainLoop(); }


  void setHttpAsyncCallback(const HttpAsyncCallback& cb)
	{
		  httpAsyncCallback_= cb;
	}

	
  void setThreadNum(int numThreads)
  {
    server_.setWorkThreadNum(numThreads);
  }

  void start();

 private:
  void onConnection(const TcpConnectionPtr& conn);
  void onMessage(const TcpConnectionPtr& conn, const TrantorTimestamp receiveTime);
  void onRequest(const TcpConnectionPtr&, const HttpRequest&);

  TcpServer server_;
  HttpAsyncCallback httpAsyncCallback_;
};



#endif  

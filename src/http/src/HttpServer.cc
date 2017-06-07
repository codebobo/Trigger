
#include "HttpServer.h"

#include <Log.h>
#include "HttpContext.h"
#include "HttpRequest.h"
#include "HttpResponse.h"


static void defaultHttpAsyncCallback(const HttpRequest&, std::function<void( HttpResponse& resp)>callback)
{
    HttpResponse resp(close);
    resp.setStatusCode(HttpResponse::k404NotFound);
    resp.setCloseConnection(true);
    callback(resp);
}




HttpServer::HttpServer(EventLoop* loop, const std::string& server_addr, const int server_port)
        : server_(loop),
          httpAsyncCallback_(defaultHttpAsyncCallback)
{
	server_.init(server_addr, server_port);
    server_.setNewConnectionCallback(
            std::bind(&HttpServer::onConnection, this, std::placeholders::_1));
    server_.setNewMessageCallback(
            std::bind(&HttpServer::onMessage, this, std::placeholders::_1, std::placeholders::_2));
}

HttpServer::~HttpServer()
{
}

void HttpServer::start()
{
    server_.start();
}

void HttpServer::onConnection(const TcpConnectionPtr& conn)
{
    conn->setContext(HttpContext());
}

void HttpServer::onMessage(const TcpConnectionPtr& conn, const TrantorTimestamp receiveTime)
{
    HttpContext* context = trantorAnyCast<HttpContext*>(conn->getMutableContext());

    StringBuffer* buf = conn->getReadBufferPtr().get();
    if (!context->parseRequest(buf, receiveTime)) {
        conn->send("HTTP/1.1 400 Bad Request\r\n\r\n");
        conn->shutdown();
    }

    if (context->gotAll()) {
        context->request().parsePremeter();
        onRequest(conn, context->request());
        context->reset();
    }
}

void HttpServer::onRequest(const TcpConnectionPtr& conn, const HttpRequest& req)
{
    const std::string& connection = req.getHeader("Connection");
    bool close = connection == "close" ||
                 (req.getVersion() == HttpRequest::kHttp10 && connection != "Keep-Alive");



    httpAsyncCallback_(req, [ = ](HttpResponse & response) {
        StringBuffer buf;
        response.setCloseConnection(close);
        response.appendToBuffer(&buf);
        conn->send(&buf);
        if (response.closeConnection()) {
            conn->shutdown();
        }
    });


}


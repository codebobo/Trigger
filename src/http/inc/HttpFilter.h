#ifndef __HTTPFILTER_H__
#define __HTTPFILTER_H__
#include "TRObject.h"
#include "HttpController.h"
class HttpFilter:public TRObject
{
public:
	HttpFilter(){};
	virtual ~HttpFilter(){};
	DECLARE_TROBJECT_CLASS(HttpFilter);
	virtual void doFilter(const HttpRequest & req, std::function < void(HttpResponse &)> callback){};
	void setSession(const std::shared_ptr< Session > &session){session_=session;};
protected:
	std::shared_ptr< Session > session_;
};
#endif

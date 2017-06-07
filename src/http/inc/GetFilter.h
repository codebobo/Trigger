#ifndef __GETFILTER_H__
#define __GETFILTER_H__
#include "HttpFilter.h"
class GetFilter:public HttpFilter
{
public:
	GetFilter(){};
	virtual ~GetFilter(){};
	DECLARE_TROBJECT_CLASS(GetFilter);
	virtual void doFilter(const HttpRequest & req, std::function < void(HttpResponse &)> callback);
};
#endif

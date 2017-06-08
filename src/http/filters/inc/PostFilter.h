#ifndef __POSTFILTER_H__
#define __POSTFILTER_H__
#include "HttpFilter.h"
class PostFilter:public HttpFilter
{
public:
	PostFilter(){};
	virtual ~PostFilter(){};
	DECLARE_TROBJECT_CLASS(PostFilter);
	virtual void doFilter(const HttpRequest & req, std::function < void(HttpResponse &)> callback);
};
#endif

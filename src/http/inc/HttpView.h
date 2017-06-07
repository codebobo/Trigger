#ifndef __HTTPVIEW_H__
#define __HTTPVIEW_H__

#include "TRObject.h"
#include "HttpResponse.h"
#include "HttpViewData.h"
#include <map>
#include <string>


class HttpView:public TRObject
{
public:

    static HttpResponse genHttpResponse(std::string viewName,const HttpViewData &data);
    static HttpResponse notFoundResponse();
    static HttpResponse locationRedirectResponse(std::string path);
    virtual ~HttpView(){};
    HttpView(){};
protected:

    virtual HttpResponse genHttpResponse(const HttpViewData&)=0;
};
#endif
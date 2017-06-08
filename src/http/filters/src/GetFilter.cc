#include "GetFilter.h"
#include "HttpView.h"
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <set>
IMPL_TROBJECT_CLASS(GetFilter);
void GetFilter::doFilter(const HttpRequest & req, std::function < void(HttpResponse &)> callback)
{
    if(req.method() != HttpRequest::kGet) {
        HttpResponse resp=HttpView::notFoundResponse();
        callback(resp);
        return;
    }
}

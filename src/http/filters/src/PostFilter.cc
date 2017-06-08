#include "PostFilter.h"
#include "HttpView.h"
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <set>
IMPL_TROBJECT_CLASS(PostFilter);
void PostFilter::doFilter(const HttpRequest & req, std::function < void(HttpResponse &)> callback)
{
    if(req.method() != HttpRequest::kPost) {
        HttpResponse resp=HttpView::notFoundResponse();
        callback(resp);
        return;
    }
}

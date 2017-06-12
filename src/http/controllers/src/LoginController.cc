#include "LoginController.h"
#include "Funcs.h"
#include <json/json.h>
#include "HttpView.h"
#include "Log.h"
IMPL_TROBJECT_CLASS(LoginController);


void LoginController::handleHttpAsyncRequest(const HttpRequest & req, std::function < void(HttpResponse &)> callback)
{
    std::map<std::string,std::string> premeter=req.getPremeter();
    std::string userName=premeter["username"];
    std::string passwd=premeter["password"];
	LOG4CPLUS_DEBUG(_logger, "login controller username: "<<userName<<" passwd: "<<passwd) ;
    if(userName == "httptest" && passwd == "http123")
    {        
		Json::Value result;        
		result["result"]="success";        
		result["message"]="µÇÂ¼³É¹¦";        
		HttpViewData data;        
		data.insert("json",result);        
		HttpResponse resp=HttpView::genHttpResponse("JsonView",data);        
		callback(resp);         
	}
	else
	{
		LOG4CPLUS_DEBUG(_logger, "login failed") ;
		Json::Value result;        
		result["result"]="fail";        
		result["message"]="µÇÂ¼Ê§°Ü";        
		HttpViewData data;        
		data.insert("json",result);        
		HttpResponse resp=HttpView::genHttpResponse("JsonView",data);        
		callback(resp); 
	}
}

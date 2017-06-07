#include "HttpController.h"
#include "HttpView.h"
#include <fstream>
#include <iostream>

IMPL_TROBJECT_CLASS(HttpController);
HttpController::HttpController()
{
}
//void HttpController::handleHttpRequest(const HttpRequest& req, HttpResponse* resp)
//{
//	setNotFound(resp);
//}
void HttpController::handleHttpAsyncRequest(const HttpRequest& req,std::function<void (HttpResponse &)>callback)
{
	HttpResponse resp=HttpView::notFoundResponse();
	callback(resp);
}

HttpController::~HttpController()
{
}


//void HttpController::setNotFound(HttpResponse* resp)
//{
//	resp->setStatusCode(HttpResponse::k404NotFound);
//	resp->setStatusMessage("Not Found");
//	resp->setCloseConnection(true);
//}
//void HttpController::setHtmlBody(HttpResponse* resp,std::string body)
//{
//	resp->setStatusCode(HttpResponse::k200Ok);
//    resp->setStatusMessage("OK");
//    resp->setContentType(web_con);
//    resp->setBody(body.c_str());
//
//}
//
//std::map<std::string,std::string> HttpController::parseHttpPostOrGetPremeter(const std::string& input)
//{
//	std::map<std::string,std::string> ret;
//	std::string::size_type pos=0;
//	while((input[pos]=='?'||isspace(input[pos]))&&pos<input.length())
//	{
//		pos++;
//	}
//	std::string value=input.substr(pos);
//	while((pos = value.find("&")) != std::string::npos) {
//		std::string coo = value.substr(0, pos);
//		auto epos = coo.find("=");
//		if(epos != std::string::npos) {
//			std::string key = coo.substr(0, epos);
//			std::string::size_type cpos=0;
//			while(isspace(key[cpos])&&cpos<key.length())
//				cpos++;
//			key=key.substr(cpos);
//			std::string pvalue = coo.substr(epos + 1);
//			std::string pdecode=pvalue;
//			std::string keydecode=key;
//			urldecode((char *)key.c_str(),(char *)keydecode.c_str());
//			urldecode((char *)pvalue.c_str(),(char *)pdecode.c_str());
//			ret[keydecode] = pdecode;
//		}
//		value=value.substr(pos+1);
//	}
//	if(value.length()>0)
//	{
//		std::string &coo = value;
//		auto epos = coo.find("=");
//		if(epos != std::string::npos) {
//			std::string key = coo.substr(0, epos);
//			std::string::size_type cpos=0;
//			while(isspace(key[cpos])&&cpos<key.length())
//				cpos++;
//			key=key.substr(cpos);
//			std::string pvalue = coo.substr(epos + 1);
//			std::string pdecode=pvalue;
//			urldecode((char *)pvalue.c_str(),(char *)pdecode.c_str());
//			ret[key] = pdecode;
//		}
//	}
//	return ret;
//}

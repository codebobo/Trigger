#ifndef __LOGIN_CONTROLLER_H___
#define __LOGIN_CONTROLLER_H___
#include "HttpController.h"
class LoginController:public HttpController
{
	public:
		//virtual void handleHttpRequest(const HttpRequest & req,HttpResponse * resp);
		virtual void handleHttpAsyncRequest(const HttpRequest & req, std::function < void(HttpResponse &)> callback);
	DECLARE_TROBJECT_CLASS(LoginController);
};
#endif

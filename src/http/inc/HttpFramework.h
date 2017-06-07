#ifndef __HTTP_FRAMEWORK_H__
#define __HTTP_FRAMEWORK_H__


#include "HttpServer.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpConfig.h"
#include "HttpController.h"
#include "CacheMap.h"
#include "TrantorAny.h"

#include <memory>
#include <string>
#include <map>
#include <set>
#include <functional>


using std::map;
using std::shared_ptr;
class HttpFramework:public FrameworkEnvironment
{
	public:
		HttpFramework(EventLoop* loop, const std::string& server_addr, const int server_port);
		void start();
		void initWithConfigFile(const std::string & filePath);
		virtual const std::string getUploadPath() const ;
		virtual const std::string getDocumentRoot() const;
		virtual const std::string getHost() const;
		virtual const std::string getTitle() const;
		virtual TrantorAny* getObjectByName(const std::string &) ;
		virtual void setObjectByName(const std::string &, TrantorAny&);
	protected:
	    std::unique_ptr <HttpServer> _httpServer;
		std::unique_ptr <HttpConfig> _httpConfig;
		bool benchmark = false;
		//void onRequest(const HttpRequest& req, HttpResponse* resp);
		void onAsyncRequest(const HttpRequest& req,std::function<void ( HttpResponse &)>callback);
		void readSendFile(const std::string& filePath,const HttpRequest& req,HttpResponse* resp);
		std::string _rootPath;

	private:
		shared_ptr< map<std::string,std::string> > _controllerMap;
		shared_ptr< map<std::string,std::vector<std::string> > >  _filterMap;
		int _sessionTimeout;
		std::set<std::string> _fileTypeSet;
		//map < std::string,std::shared_ptr<Session> > _sessionMap;
		CacheMap<std::string,std::shared_ptr<Session> > _sessionMap;
		std::map<std::string,TrantorAny*>_objectMap;
};
#endif

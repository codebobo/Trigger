#include "HttpFramework.h"
#include "HttpConfig.h"
#include "TRClassMap.h"
#include "HttpController.h"
#include "HttpFilter.h"
#include "Funcs.h"

#include <fstream>
#include <functional>
#include <iostream>
#include <algorithm>
#include <string>
#include <sys/stat.h>

/*
HTTP/1.1 302 Found
Expires: Thu, 01 Jan 1970 00:00:00 GMT
Set-Cookie: JSESSIONID=gly5j038afxo;Path=/
Location: http://113.59.110.65:9090/login.jsp?url=%2Findex.jsp
Content-Length: 0

*/

using namespace std::placeholders;
void HttpFramework::onAsyncRequest(const HttpRequest& req,std::function<void (HttpResponse &)>callback)
{
    //std::cout << "Headers " << req.methodString() << " " << req.path() << std::endl;
	
	if (!benchmark) {
        const std::map<std::string, std::string>& headers = req.headers();
        for (std::map<std::string, std::string>::const_iterator it = headers.begin();
             it != headers.end();
             ++it) {
            std::cout << it->first << ": " << it->second << std::endl;
        }

		//std::cout<<"cookies:"<<std::endl;
		auto cookies = req.cookies();
		for(auto it=cookies.begin();it!=cookies.end();++it)
		{
			//std::cout<<it->first<<"="<<it->second<<std::endl;
		}
    }


    //LOG_INFO << "http path=" << req.path();
    //LOG_INFO << "query: " << req.query() ;

    std::string session_id=req.getCookie("JSESSIONID");
	bool needSetJsessionid=false;
    if(_sessionTimeout>0)
    {
        if(session_id=="")
        {
            session_id=getuuid().c_str();
            //resp.addCookie("JSESSIONID",session_id);
            needSetJsessionid=true;
            _sessionMap.insert(session_id,std::make_shared< Session >(),_sessionTimeout);
        }
        else
        {
            if(_sessionMap.find(session_id)==false)
            {
                _sessionMap.insert(session_id,std::make_shared< Session >(),_sessionTimeout);
            }
        }
    }

    std::string path = req.path().c_str();
    auto pos = path.rfind(".");
    if(pos != std::string::npos) {
        std::string filetype = path.substr(pos + 1, path.length());
        transform(filetype.begin(), filetype.end(), filetype.begin(), tolower);
        if(_fileTypeSet.find(filetype) != _fileTypeSet.end()) {
            //LOG_INFO << "file query!";
            std::string filePath = _rootPath + path;
			HttpResponse resp(close);
			if(needSetJsessionid)
			resp.addCookie("JSESSIONID",session_id);
            // pick a Content-Type for the file
            if(filetype=="html")	resp.setContentTypeCode(CT_TEXT_HTML);
            else if(filetype=="js")  resp.setContentTypeCode(CT_APPLICATION_X_JAVASCRIPT);
            else if(filetype=="css")  resp.setContentTypeCode(CT_TEXT_CSS);
            else if(filetype=="xml")  resp.setContentTypeCode(CT_TEXT_XML);
            else if(filetype=="xsl") resp.setContentTypeCode(CT_TEXT_XSL);
            else if(filetype=="txt")  resp.setContentTypeCode(CT_TEXT_PLAIN);
            else if(filetype=="svg")  resp.setContentTypeCode(CT_IMAGE_SVG_XML);
            else if(filetype=="ttf")  resp.setContentTypeCode(CT_APPLICATION_X_FONT_TRUETYPE);
            else if(filetype=="otf")  resp.setContentTypeCode(CT_APPLICATION_X_FONT_OPENTYPE);
            else if(filetype=="woff2")resp.setContentTypeCode(CT_APPLICATION_FONT_WOFF2);
            else if(filetype=="woff") resp.setContentTypeCode(CT_APPLICATION_FONT_WOFF);
            else if(filetype=="eot")  resp.setContentTypeCode(CT_APPLICATION_VND_MS_FONTOBJ);
            else if(filetype=="png")  resp.setContentTypeCode(CT_IMAGE_PNG);
            else if(filetype=="jpg")  resp.setContentTypeCode(CT_IMAGE_JPG);
            else if(filetype=="jpeg") resp.setContentTypeCode(CT_IMAGE_JPG);
            else if(filetype=="gif")  resp.setContentTypeCode(CT_IMAGE_GIF);
            else if(filetype=="bmp")  resp.setContentTypeCode(CT_IMAGE_BMP);
            else if(filetype=="ico")  resp.setContentTypeCode(CT_IMAGE_XICON);
            else if(filetype=="icns") resp.setContentTypeCode(CT_IMAGE_ICNS);
            else resp.setContentTypeCode(CT_APPLICATION_OCTET_STREAM);

            readSendFile(filePath,req, &resp);
			callback(resp);
            return;
        }

    }
    std::vector<std::string> filters=(*_filterMap)[req.path().c_str()];
    for(std::string filterClassName:filters)
    {
        TRObject *_obj=TRClassMap::newObject(filterClassName);
        HttpFilter *filter= dynamic_cast<HttpFilter*>(_obj);
        int filter_flag=0;
        if(filter)
        {
            //LOG_INFO<<filterClassName<<".doFilter()";
            filter->setSession(_sessionMap[session_id]);
            filter->doFilter(req,[=,&filter_flag]( HttpResponse &resp){
                callback(resp);
                filter_flag=1;
            });
            if(filter_flag)
                return;
        }
        else{}
            //LOG_INFO<<"can't find filter "<<filterClassName;
    }
    std::string ctrlName = (*_controllerMap)[req.path().c_str()];

    TRObject *_object = TRClassMap::newObject(ctrlName);

    HttpController *controller = dynamic_cast<HttpController*>(_object);

    if(controller) {
		controller->setSession(_sessionMap[session_id]);
        controller->setEnvironment(this);
        controller->handleHttpAsyncRequest(req, [=](HttpResponse& resp){
			if(needSetJsessionid)
			resp.addCookie("JSESSIONID",session_id);
			callback(resp);
		});
        delete controller;
    } else {
        //LOG_ERROR<<"can't find controller "<<ctrlName;
    	HttpResponse resp(close);
    
        resp.setStatusCode(HttpResponse::k404NotFound);
        resp.setCloseConnection(true);
		if(needSetJsessionid)
			resp.addCookie("JSESSIONID",session_id);
		callback(resp);
    }

}



HttpFramework::HttpFramework(EventLoop* loop, const std::string& server_addr, const int server_port)
    : _httpServer(std::unique_ptr<HttpServer>(new HttpServer(loop, server_addr, server_port))),
      _sessionMap(10,1800)
{
}
void HttpFramework::start()
{
    //_httpServer->setHttpCallback(std::bind(&HttpFramework::onRequest, this, _1, _2));
	_httpServer->setHttpAsyncCallback(std::bind(&HttpFramework::onAsyncRequest, this, _1, _2));
    _httpServer->start();
}
void HttpFramework::readSendFile(const std::string& filePath,const HttpRequest& req, HttpResponse* resp)
{
//If-Modified-Since: Wed Jun 15 08:57:30 2016 GMT
    std::ifstream infile(filePath, std::ifstream::binary);
    //LOG_INFO << "send http file:" << filePath;
    if(!infile) {

        resp->setStatusCode(HttpResponse::k404NotFound);
        resp->setCloseConnection(true);
        return;
    }

    if(_httpConfig->enableLastModify())
    {
        struct stat fileStat;
        //LOG_DEBUG<<"enabled LastModify";
        if(stat(filePath.c_str(),&fileStat)>=0)
        {
            //std::cout<<"last modify time:"<<fileStat.st_mtime<<std::endl;
            struct tm tm1;
            gmtime_r(&fileStat.st_mtime,&tm1);
            char timeBuf[64];
            asctime_r(&tm1,timeBuf);
            std::string timeStr(timeBuf);
            std::string::size_type len=timeStr.length();
            std::string lastModified =timeStr.substr(0,len-1)+" GMT";

            std::string modiStr=req.getHeader("If-Modified-Since");
            if(modiStr!=""&&modiStr==lastModified)
            {
                //LOG_DEBUG<<"not Modified!";
                resp->setStatusCode(HttpResponse::k304NotModified);
                return;
            }
            resp->addHeader("Last-Modified",lastModified);

            resp->addHeader("Expires","Thu, 01 Jan 1970 00:00:00 GMT");
        }
    }
    
	std::streambuf * pbuf = infile.rdbuf();
    std::streamsize size = pbuf->pubseekoff(0,infile.end);
    pbuf->pubseekoff(0,infile.beg);       // rewind
    char *contents = new char [size];
    pbuf->sgetn (contents,size);
    infile.close();
    std::string str(contents,size);




    resp->setStatusCode(HttpResponse::k200Ok);
    //LOG_INFO << "file len:" << str.length();
    resp->setBody(str);
	delete contents;
}

void HttpFramework::initWithConfigFile(const std::string & filePath)
{
    _httpConfig = std::unique_ptr<HttpConfig>(new HttpConfig);
    if(_httpConfig->init(filePath) != 0) {
        //LOG_ERROR << "http config error!";
    } else {
        _rootPath = _httpConfig->getDocRoot();
        _controllerMap = _httpConfig->getControllers();
        _filterMap=_httpConfig->getFilters();
        _sessionTimeout=_httpConfig->getSessionTimeout();
        int num = _httpConfig->getThreadNum();
        if(num > 0) {
            _httpServer->setThreadNum(num);
        }
        _fileTypeSet = _httpConfig->getFileTypes();
    }
}
const std::string HttpFramework::getUploadPath() const
{
    return _httpConfig->getUploadPath();
}
const std::string HttpFramework::getDocumentRoot() const
{
    return _httpConfig->getDocRoot();
}
const std::string HttpFramework::getHost() const
{
    return _httpConfig->getHost();
}
const std::string HttpFramework::getTitle() const
{
    return _httpConfig->getTitle();
}
TrantorAny* HttpFramework::getObjectByName(const std::string &key)
{
    if(_objectMap.find(key)==_objectMap.end())
        return NULL;
    return _objectMap[key];
}
void HttpFramework::setObjectByName(const std::string &key,TrantorAny& obj)
{
    _objectMap[key]=&obj;
}

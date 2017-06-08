#ifndef __HTTP_CONTROLLER_H__
#define __HTTP_CONTROLLER_H__
#include "TRObject.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include <map>
#include <string>
#include <memory>
#include <functional>
#include <mutex>
#include "TrantorAny.h"


typedef struct EmpInfo
{
public:
	uint64_t emp_uuid;
	int emp_type;

	bool operator < (const EmpInfo& r)
	{
		return this->emp_uuid < r.emp_uuid;
	}
	bool operator > (const EmpInfo& r)
	{
		return this->emp_uuid > r.emp_uuid;
	}
	bool operator == (const EmpInfo& r)
	{
		return this->emp_uuid == r.emp_uuid;
	}
} EmpInfo;

typedef std::map<std::string,TrantorAny> SessionMap;
class Session
{
public:
	template <typename T> T get(const std::string &key) const{
		std::lock_guard<std::mutex> lck(mutex_);
		auto it=sessionMap_.find(key);
		if(it!=sessionMap_.end())
		{
			return trantorAnyCast<T>(it->second);
		}
		T tmp;
		return tmp;
	};
	TrantorAny &operator[](const std::string &key){
		std::lock_guard<std::mutex> lck(mutex_);
		return sessionMap_[key];
	};
	void insert(const std::string& key,const TrantorAny &obj)
	{
		std::lock_guard<std::mutex> lck(mutex_);
		sessionMap_[key]=obj;
	};
	void erase(const std::string& key)
	{
		std::lock_guard<std::mutex> lck(mutex_);
		sessionMap_.erase(key);
	}
	bool find(const std::string& key)
	{
		std::lock_guard<std::mutex> lck(mutex_);
		if(sessionMap_.find(key) == sessionMap_.end())
		{
			return false;
		}
		return true;
	}
	void clear()
	{
		std::lock_guard<std::mutex> lck(mutex_);
		sessionMap_.clear();
	}
protected:
	SessionMap sessionMap_;
	int timeoutInterval_;
	mutable std::mutex mutex_; 
};
class FrameworkEnvironment
{
public:
	virtual const std::string getUploadPath() const =0;
	virtual const std::string getDocumentRoot() const =0;
	virtual const std::string getHost() const =0;
	virtual const std::string getTitle() const =0;
	virtual TrantorAny* getObjectByName(const std::string &) =0;
	virtual void setObjectByName(const std::string &,TrantorAny&) =0;
};
class HttpController:public TRObject
{
	public:
		HttpController();
		virtual ~HttpController();
		//virtual void handleHttpRequest(const HttpRequest& req, HttpResponse* resp);
		virtual void handleHttpAsyncRequest(const HttpRequest& req,std::function<void (HttpResponse &)>callback);
		DECLARE_TROBJECT_CLASS(HttpController);
		void setSession(const std::shared_ptr< Session > &session){session_=session;};
		void setEnvironment(FrameworkEnvironment* env){environment_=env;};
	protected:
		std::shared_ptr< Session > session_;
		FrameworkEnvironment* environment_;
//		void setNotFound(HttpResponse* resp);
//		void setHtmlBody(HttpResponse* resp,std::string body);

		//std::map<std::string,std::string> parseHttpPostOrGetPremeter(const std::string&);
};
#endif

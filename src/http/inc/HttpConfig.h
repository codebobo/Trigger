#ifndef _HTTP_CONFIG_H__
#define _HTTP_CONFIG_H__

#include <libconfig.h++>
#include <map>
#include <memory>
#include <set>
#include <vector>
class HttpConfig
{
    public:
        
        int init(const std::string &filename);
        
        int getThreadNum();
		std::shared_ptr< std::map<std::string,std::string> > getControllers();
		std::shared_ptr< std::map<std::string,std::vector<std::string> > >getFilters();
		std::set<std::string> getFileTypes();
		std::string getDocRoot();
		std::string getUploadPath();
		std::string getHost();
		std::string getTitle();
	  	int getSessionTimeout();
		int enableLastModify();
		HttpConfig(){};
        ~HttpConfig(){};
     private:
       

        libconfig::Config config_;
};
#endif
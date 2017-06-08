#include "HttpConfig.h"

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>

int HttpConfig::init(const std::string &filename)
{

    try
      {
        config_.readFile(filename.c_str());
      }
      catch(const libconfig::FileIOException &fioex)
      {
        //LOG_INFO << "I/O error while reading file." ;
        return -1;
      }
      catch(const libconfig::ParseException &pex)
      {
        //LOG_ERROR << "Parse error at " << pex.getFile() << ":" << pex.getLine()
                 // << " - " << pex.getError() ;
        exit(-1);
      }
    return 0;
}
 
int HttpConfig::getThreadNum()
{
    int num;
    if(config_.lookupValue("http.thread_num",num))
        return num;
    return 1;
}

std::shared_ptr< std::map<std::string,std::string> > HttpConfig::getControllers()
{
	const libconfig::Setting& root = config_.getRoot();
 std::shared_ptr< std::map<std::string,std::string> > ctrlMap = std::shared_ptr< std::map<std::string,std::string> >(new std::map<std::string,std::string>);
   try
   {
     const libconfig::Setting &controllers = root["http"]["controllers"];
     int count = controllers.getLength();
	 for(int i = 0; i < count; ++i)
     {
       const libconfig::Setting &controller = controllers[i];
       std::string path_str, controller_str;

       if(!(controller.lookupValue("path", path_str)
            && controller.lookupValue("controller", controller_str)))
         continue;
	   ctrlMap->insert(std::make_pair(path_str,controller_str));
       }
   }
   catch(const libconfig::SettingNotFoundException &nfex)
   {
     return nullptr;
   }
   return  ctrlMap;
}

std::shared_ptr< std::map<std::string,std::vector<std::string> > > HttpConfig::getFilters()
{
    const libconfig::Setting& root = config_.getRoot();
    //LOG_INFO<<"getFilters";
    std::shared_ptr< std::map<std::string,std::vector<std::string> > > ctrlMap = std::shared_ptr< std::map<std::string,std::vector<std::string> > >(new std::map<std::string,std::vector<std::string> >);
    try
    {
        const libconfig::Setting &controllers = root["http"]["controllers"];
        int count = controllers.getLength();
        for(int i = 0; i < count; ++i)
        {
            const libconfig::Setting &controller = controllers[i];
            std::string path_str;
            try {
                libconfig::Setting &filter = controller["filters"];
                if(!controller.lookupValue("path", path_str))
                    continue;

                std::vector<std::string>filters;
                int filter_count=filter.getLength();
                for(int j = 0; j < filter_count; ++j)
                {
                    const std::string &filter_str = filter[j];
                    filters.push_back(filter_str);
                    //LOG_INFO<<"filter:"<<filter_str;
                }
                ctrlMap->insert(std::make_pair(path_str,filters));
            }
            catch(const libconfig::SettingNotFoundException &nfex)
            {
                continue;
            }



        }
    }
    catch(const libconfig::SettingNotFoundException &nfex)
    {
        return nullptr;
    }
    return  ctrlMap;
}
std::set<std::string> HttpConfig::getFileTypes()
{
	std::set<std::string> files;
	const libconfig::Setting& root = config_.getRoot();
   try
   {
     const libconfig::Setting &cfg_files = root["http"]["files"];
     int count = cfg_files.getLength();
	 for(int i = 0; i < count; ++i)
     {
       const std::string &filetType = cfg_files[i];
       std::string path_str, controller_str;

       files.insert(filetType);
       }
   }
   catch(const libconfig::SettingNotFoundException &nfex)
   {
     return files;
   }
   return  files;
}

std::string HttpConfig::getDocRoot()
{
	std::string root;
	config_.lookupValue("http.document_root",root);
    return root;
}
std::string  HttpConfig::getUploadPath()
{
    std::string root;
    config_.lookupValue("http.upload_path",root);
    return root;
}

std::string  HttpConfig::getHost()
{
    std::string root;
    config_.lookupValue("http.host",root);
    return root;
}
std::string HttpConfig::getTitle()
{
    std::string title;
    config_.lookupValue("http.title",title);
    return title;
}
int HttpConfig::enableLastModify()
{
    int ret;
    if(config_.lookupValue("http.enable_last_modify",ret))
        return ret;
    return 0;
}
int HttpConfig::getSessionTimeout()
{
    int ret;
    if(config_.lookupValue("http.session_timeout",ret))
        return ret;
    return 0;
}


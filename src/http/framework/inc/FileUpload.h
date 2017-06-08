#ifndef __FILE_UP_LOAD_H__
#define __FILE_UP_LOAD_H__

#include "HttpRequest.h"
#include <string>
#include <map>
#include <vector>
class HttpFile
{
public:
    const std::string& getFileName() const {return fileName_;};
    void setFileName(const std::string& filename){fileName_=filename;};
    void setFile(const std::string& file){fileContent_=file;};
    int save(const std::string& path);
    int saveAs(const std::string& filename);
    int64_t fileLength() const {return fileContent_.length();};
    const std::string getMd5()const;
protected:
    std::string fileName_;
    std::string fileContent_;
};
class FileUpload
{
public:
    FileUpload(){};
    ~FileUpload(){};
    const std::vector<HttpFile> getFiles();
    const std::map<std::string,std::string>& getPremeter() const;
    int parse(const HttpRequest& req);

protected:
    std::vector<HttpFile> files_;
    std::map<std::string,std::string> premeter_;
    int parseEntity(const std::string& str);
};
#endif
#ifndef __HTTPVIEWDATA_H__
#define __HTTPVIEWDATA_H__
#include <map>
#include <string>
#include "TrantorAny.h"
typedef std::map<std::string,TrantorAny> ViewData;
class HttpViewData
{
public:
    template <typename T>
    const T get(const std::string &key) const
    {
        auto it=viewData_.find(key);
        if(it!=viewData_.end())
        {
            return trantorAnyCast<T>(it->second);
        }
        T tmp;
        return tmp;
    }
//   template <typename T>
//    const T operator[](const std::string &key) const {
//        auto it=viewData_.find(key);
//        if(it!=viewData_.end())
//        {
//            return boost::any_cast<T>(it->second);
//        }
//        T tmp;
//        return tmp;
//    }
    void insert(const std::string& key,const TrantorAny &obj);
protected:
    ViewData viewData_;
};
#endif
#include "HttpViewData.h"
void HttpViewData::insert(const std::string& key,const TrantorAny &obj)
{
    viewData_[key]=obj;
}

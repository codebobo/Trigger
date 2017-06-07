

#include "TRClassMap.h"
#include <iostream>
std::map<std::string,TRClass*> * TRClassMap::classMap=nullptr;
std::once_flag TRClassMap::flag;
void TRClassMap::registerClass(const std::string className,TRClass* classPtr)
{
   // std::map<std::string,TRClass*> _map;
    std::call_once(flag, [](){classMap=new std::map<std::string,TRClass*>;});
    //std::cout<<"register class:"<<className<<"("<<classPtr<<")"<<std::endl;
    classMap->insert(std::make_pair(className, classPtr));
}
TRObject* TRClassMap::newObject(const std::string className)
{
    auto iter=classMap->find(className);
    if(iter!=classMap->end())
    {
        return iter->second->newObject();
    }
    else
        return nullptr;
}
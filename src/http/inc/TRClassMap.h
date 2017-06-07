

#ifndef TRClassMap_hpp
#define TRClassMap_hpp
#include "TRClass.h"


#include <stdio.h>
#include <map>
#include <memory>
#include <functional>

#include <thread>
#include <mutex>


class TRClassMap
{
public:
    static void registerClass(const std::string className,TRClass *pClass);
    static TRObject* newObject(const std::string className);
protected:
    static std::map < std::string,TRClass* > *classMap;
    static std::once_flag flag;
};
#endif /* TRClassMap_hpp */

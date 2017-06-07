

#ifndef TRObject_hpp
#define TRObject_hpp

#include <stdio.h>
#include <string>
#include "TRClassMap.h"
#include "TRClass.h"
#define DECLARE_TROBJECT_CLASS(classname)  \
DECLARE_TRCLASS(classname);\
static const TRClass##classname _classOf##classname;\
static const TRClass * getClass(){return &_classOf##classname;};\
virtual std::string getClassName(){return _classOf##classname.getClassName();};

#define IMPL_TROBJECT_CLASS(classname) \
const classname::TRClass##classname classname::_classOf##classname;
class TRObject
{
public:
    TRObject();
    virtual ~TRObject(){};
    DECLARE_TROBJECT_CLASS(TRObject);
protected:
    std::string className;
};

#endif /* TRObject_hpp */


#include <typeinfo>
#include "TRObject.h"
using std::type_info;
IMPL_TROBJECT_CLASS(TRObject);
TRObject::TRObject()
{
    //className=typeid(this).name();
    //TRCLASS_DEFINE(TRObject);
    //TRClassMap::registerClass("TRObject", [](){return new TRObject;});
}

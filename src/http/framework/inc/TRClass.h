//
//  TrantorClass.hpp
//  antTest
//
//  Created by antao on 16/6/23.
//  Copyright © 2016年 ant. All rights reserved.
//

#ifndef TrantorClass_hpp
#define TrantorClass_hpp
#include "TRClassMap.h"
//
#include <stdio.h>
#include <string>
#include <memory>
#define DECLARE_TRCLASS(classname) \
class TRClass##classname:public TRClass \
{ \
public: \
    TRClass##classname(){TRClassMap::registerClass(#classname,this);};\
    std::string getClassName() const {return #classname;}; \
    TRObject *newObject() const {return new classname;}; \
};

class TRObject;
class TRClass
{
public:
    TRClass(){};
    ~TRClass(){};
    virtual std::string getClassName() const=0;
    virtual TRObject *newObject() const=0;
    
private:
};



#endif /* TrantorClass_hpp */

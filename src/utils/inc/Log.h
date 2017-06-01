#ifndef CPLUS_LOG_H_
#define CPLUS_LOG_H_

#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <iostream>
#include <log4cplus/consoleappender.h>
#include <log4cplus/layout.h>
#include <log4cplus/helpers/loglog.h>
#include  <log4cplus/loggingmacros.h> 

void initLog();
extern log4cplus::Logger _logger; 

#endif
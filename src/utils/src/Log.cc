#include "Log.h"
using namespace log4cplus;
using namespace log4cplus::helpers;

log4cplus::Logger _logger = Logger::getInstance("EventServer");
void initLog()
{
     SharedObjectPtr<Appender> _append (new ConsoleAppender());
	_append->setName("EventServer append");

    /* step 2: Instantiate a layout object */
    std::string pattern = "%d{%m/%d/%y %H:%M:%S}  - %m [%l]%n";
	std::auto_ptr<Layout> _layout(new PatternLayout(pattern));

    /* step 3: Attach the layout object to the appender */
	_append->setLayout( _layout );

    /* step 4: Instantiate a logger object */
	//_logger = Logger::getInstance("EventServer");

    /* step 5: Attach the appender object to the logger  */
	_logger.addAppender(_append);

	/* step 6: Set a priority for the logger  */
	_logger.setLogLevel(ALL_LOG_LEVEL);
}
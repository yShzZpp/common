#ifndef _CTI_ROBOT_SAVE_CRASH_LOG_
#define _CTI_ROBOT_SAVE_CRASH_LOG_

#include <stdio.h>
#include <string>
#include <functional>

namespace cti
{
    namespace robot
    {
        typedef std::function <void(std::string)> ShutdownFunction;
        void registerSignalHandler(std::string robot_num, std::string process_name, ShutdownFunction process_shutdown);
    }
}

#endif //_CTI_ROBOT_SAVE_CRASH_LOG_
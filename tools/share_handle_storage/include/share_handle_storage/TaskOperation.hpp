#ifndef _CTI_ROBOT_TOOL_TASK_TO_STRING_HPP_
#define _CTI_ROBOT_TOOL_TASK_TO_STRING_HPP_

#include <string>

namespace cti_robot
{
    namespace tool
    {
        namespace task
        {
            std::string requestActionToString(unsigned int action);

            std::string taskTypeToString(unsigned int task_type);
        }
    }
}

#endif //_CTI_ROBOT_TOOL_TASK_TO_STRING_HPP_
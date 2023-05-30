
#ifndef _CTI_ROBOT_UPLOAD_CRASH_LOG_
#define _CTI_ROBOT_UPLOAD_CRASH_LOG_

#include <cpprest/json.h>

// using namespace std;

namespace cti
{
    namespace robot
    {
        namespace uploadlog
        {
            bool UploadLog(std::string mode, std::string file);
            bool UploadLog(std::string mode, std::string robot_num , std::string node, web::json::value json);
        }
    }
}

#endif //_CTI_ROBOT_UPLOAD_CRASH_LOG_

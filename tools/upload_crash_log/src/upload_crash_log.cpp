#include <cstdlib>
#include <ctime>
#include "upload_crash_log.hpp"
#include "spdlog/spdlog.h"

using namespace std;
using namespace std::literals;

namespace cti
{
    namespace robot
    {
        namespace uploadlog
        {
            string GetTimeString()
            {
                time_t rawtime;
                struct tm * timeinfo;
                char buffer [100];

                time(&rawtime);
                timeinfo = std::localtime(&rawtime);
                strftime(buffer, 99,"%Y_%m_%d_%H_%M_%S", timeinfo);
                string t(buffer);

                return t;
            }

            bool upload(std::string file)
            {
                auto commandLine = "curl -X POST -k -H \'Content-Type: application/json\' "s
                           + "-i \'https://qyapi.weixin.qq.com/cgi-bin/webhook/send?key=617d9be0-33d7-444d-a643-c9969bd84df2\'"
                           + " --data \"@" + file + "\"";

                return std::system(commandLine.c_str());
            }

            bool UploadLog(std::string mode, std::string file)
            {
                return upload(file);
            }

            bool UploadLog(std::string mode, std::string robot_num , std::string node, web::json::value json)
            {
                std::string file = "/tmp/" + mode + "_" + robot_num + "_"+ node + "_" + GetTimeString();
                FILE* fp = fopen(file.c_str(), "w+");
                fprintf(fp, "%s", json.serialize().c_str());
                fclose(fp);

                return upload(file);
            }
        }
    }
}

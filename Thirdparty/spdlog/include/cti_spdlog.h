/*
 * @Author: your name
 * @Date: 2020-01-09 11:39:06
 * @LastEditTime: 2020-04-07 09:22:50
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /src/map/map_manager/include/map_manager/log.h
 */
#ifndef _CTI_BUILDINGROBOT_LOG_SPDLOG_
#define _CTI_BUILDINGROBOT_LOG_SPDLOG_

#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"

#include <boost/filesystem.hpp>

namespace cti
{
    namespace buildingrobot
    {
        namespace log
        {
            class SpdLog
            {
            public:

                SpdLog(std::string mode_name, unsigned int size, unsigned num)
                {
                    std::string log_path = std::string(::getenv("HOME")) + "/.ros/" + mode_name;

                    try
                    {
                        boost::filesystem::path path(log_path);
                        if (!boost::filesystem::exists(path))
                        {
                            boost::filesystem::create_directories(path);
                        }
                    }
                    catch(std::exception& ex)
                    {
                        std::string error_message("create main directories failed - ");
                        throw std::runtime_error(error_message + ex.what());
                    }
                    
                    mFileLogger = spdlog::rotating_logger_mt(mode_name, log_path + "/" + mode_name + ".log", size, num);
                    mFileLogger->set_pattern("[%Y-%m-%d %H:%M:%S %f] [%l] %v ");
                    spdlog::flush_on(spdlog::level::err);
                    spdlog::flush_every(std::chrono::seconds(1));

                    spdlog::set_default_logger(mFileLogger);
                }

                virtual ~ SpdLog()
                {
                    spdlog::drop_all();
                }

            private:
                std::shared_ptr<spdlog::logger> mFileLogger;
            };
        }
    }
}


#endif //_CTI_BUILDINGROBOT_LOG_SPDLOG_

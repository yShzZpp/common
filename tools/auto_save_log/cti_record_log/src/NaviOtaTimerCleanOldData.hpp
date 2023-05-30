#ifndef _CTI_ROBOT_FILE_MONITOR_NAVI_OTA_TIMER_CLEAN_OLD_DATA_HPP_
#define _CTI_ROBOT_FILE_MONITOR_NAVI_OTA_TIMER_CLEAN_OLD_DATA_HPP_

#include "TimerCleanOldData.hpp"

namespace cti_robot
{
namespace file_monitor
{
    class NaviOtaTimerCleanOldData : public TimerCleanOldData
    {
        public:

            NaviOtaTimerCleanOldData()
            {
                mVersion = read_navi_version();
            }
        
        private:

            std::string mVersion;

        protected:

            bool needDelete(boost::filesystem::directory_iterator it) override
            {
                namespace bf = boost::filesystem;

                try
                {
                    bf::path p(it->path());

                    if (bf::last_write_time(p) < mExpirationTime)
                    {
                        if (bf::is_directory(it->status()) 
                            && mCleanDir)
                        {
                            return true;
                        }
                        else if (mCleanFile)
                        {
                            std::string name = p.filename().string();
                            if (name == "upgrade_info_current"
                             || name == "APP.tar"
                             || name.find(mVersion) != std::string::npos)
                            {
                                return false;
                            }
                            return true;
                        }
                    }
                }
                catch (std::exception & e)
                {
                    SPDLOG_ERROR("[{}]判断是否能删除时捕获std错误:{}", mDir, e.what());
                }

                return false;
            }
        
        private:

            std::string read_navi_version()
            {
                FILE *fstream = NULL;      
                char buff[64];    
                memset(buff, 0, sizeof(buff));   

                std::string version = "no unknow";
                if(NULL == (fstream = popen("dpkg -s br-robot-release | grep Version | cut -d \' \' -f 2", "r")))   
                {     
                    SPDLOG_ERROR("获取版本号异常: {}", strerror(errno));
                }
                else
                {
                    char tmp[50] = {0};
                    auto unused = fscanf(fstream, "%s", tmp);
                    version = tmp;
                    pclose(fstream);
                }
                SPDLOG_ERROR("获取版本号: {}", version);
                return version;
            }
    };
}
}

#endif //_CTI_ROBOT_FILE_MONITOR_NAVI_OTA_TIMER_CLEAN_OLD_DATA_HPP_
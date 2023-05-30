#ifndef _CTI_ROBOT_FILE_MONITOR_TIMER_CLEAN_OLD_DATA_HPP_
#define _CTI_ROBOT_FILE_MONITOR_TIMER_CLEAN_OLD_DATA_HPP_

#include "ros/ros.h"
#include "spdlog/spdlog.h"
#include "boost/filesystem.hpp"
#include <ctime>

namespace cti_robot
{
namespace file_monitor
{
    class TimerCleanOldData
    {
        protected:

            ros::Timer mTimer;
            std::string mDir;
            unsigned int mPeriod;
            unsigned int mExpirationTime;
            bool mCleanFile;
            bool mCleanDir;

        public:

            TimerCleanOldData(){}

            void Init(std::string dir,
                      unsigned int period,
                      unsigned int expiration_time,
                      bool clean_file,
                      bool clean_dir,
                      bool first_do_clean)
            {
                mDir = dir;
                mPeriod = period;
                mExpirationTime = expiration_time;
                mCleanFile = clean_file;
                mCleanDir = clean_dir;

                if (first_do_clean)
                {
                    cleanFunction();
                }
                return;
            }

            void Start()
            {
                ros::NodeHandle n;
                mTimer = n.createTimer(ros::Duration(mPeriod), &TimerCleanOldData::cbTimer, this);
                return;
            }

            void Stop()
            {
                mTimer.stop();
                return;
            }
        
        protected:

            void cbTimer(const ros::TimerEvent &) 
            {
                cleanFunction();
                return;
            }

            virtual void cleanFunction()
            {
                SPDLOG_INFO("清理文件夹: {}", mDir);

                namespace bf = boost::filesystem;

                try
                {
                    if (bf::exists(mDir))
                    {
                        bf::directory_iterator end_it;
                        for (bf::directory_iterator dir_it(mDir); dir_it != end_it; dir_it++)
                        {
                            bf::path p(dir_it->path());
                            if (needDelete(dir_it))
                            {
                                SPDLOG_INFO("删除文件: {}", dir_it->path().string());
                                bf::remove_all(p);
                            }
                        }
                    }
                    else
                    {
                        SPDLOG_ERROR("[{}]文件夹不存在", mDir);
                    }
                }
                // catch (boost::exception & ex)
                // {
                //     SPDLOG_ERROR("[{}]定时清理捕获boost错误:{}", mDir, ex.what());
                // }
                catch (std::exception & e)
                {
                    SPDLOG_ERROR("[{}]定时清理捕获std错误:{}", mDir, e.what());
                }
                return;
            }

            virtual bool needDelete(boost::filesystem::directory_iterator it)
            {
                namespace bf = boost::filesystem;

                auto expiration_time = ros::Time::now().toSec() - mExpirationTime; 

                try
                {
                    bf::path p(it->path());

                    if (bf::last_write_time(p) < expiration_time)
                    {
                        if (bf::is_directory(it->status()) 
                         && mCleanDir)
                        {
                            return true;
                        }
                        else if (not bf::is_directory(it->status()) 
                              && mCleanFile)
                        {
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

            unsigned long sumDirSize(std::string path)
            {
                namespace bf = boost::filesystem;

                unsigned long sum = 0;

                try
                {
                    if (bf::exists(path))
                    {
                        bf::directory_iterator end_it;
                        for (bf::directory_iterator dir_it(path); dir_it != end_it; dir_it++)
                        {
                            if (not bf::is_directory(dir_it->status()))
                            {
                                sum += bf::file_size(dir_it->path());
                            }
                        }
                    }
                    else
                    {
                        SPDLOG_ERROR("[{}]文件夹不存在", path);
                    }
                }
                catch (std::exception & e)
                {
                    SPDLOG_ERROR("[{}]获取文件夹大小时捕获std错误:{}", path, e.what());
                }

                SPDLOG_INFO("[{}]文件夹大小: {}", path, sum);
                return sum;
            }

            unsigned int getTheTimeFromFileName(std::string file_name)
            {
                unsigned int now = ros::Time::now().toSec();
                unsigned int file_time = now;
                struct tm file_name_time;
                if (strptime(file_name.c_str(), "_%Y-%m-%d-%H-%M-%S", &file_name_time) != NULL)
                {
                  file_time = mktime(&file_name_time);
                  // char buff[26];
                  // strftime(buff, 26, "%Y:%m:%d %H:%M:%S", &file_name_time);
                  // SPDLOG_INFO("文件名{} ,时间:{} {} 当前时间{} 差值{}",file_name, buff, file_time,now,now-file_time);
                }
                else
                {
                  SPDLOG_ERROR("文件名时间读取错误 {}",file_name);
                }
                return file_time;
            }

            bool getOldestFileInDir(std::string dir,
                                    std::string & oldest_file,
                                    std::vector<std::string> exclude_sub_strs)
            {
                namespace bf = boost::filesystem;

                unsigned int now = ros::Time::now().toSec();
                unsigned int oldest_time = now;

                try
                {
                    if (bf::exists(dir))
                    {
                        bf::directory_iterator end_it;
                        for (bf::directory_iterator dir_it(dir); dir_it != end_it; dir_it++)
                        {
                            if (not bf::is_directory(dir_it->status())
                             && getTheTimeFromFileName(dir_it->path().filename().string()) < oldest_time)
                            {
                                bool exist = false;
                                for (auto sub_str : exclude_sub_strs)
                                {
                                    if (dir_it->path().filename().string().find(sub_str) != std::string::npos)
                                    {
                                        exist = true;
                                        break;
                                    }
                                }

                                if (not exist)
                                {
                                    oldest_file = dir_it->path().string();
                                    oldest_time = getTheTimeFromFileName(dir_it->path().filename().string());
                                }
                            }
                        }
                    }
                    else
                    {
                        SPDLOG_ERROR("[{}]文件夹不存在", dir);
                    }
                }
                catch (std::exception & e)
                {
                    SPDLOG_ERROR("[{}]寻找最旧的文件std错误:{}", dir, e.what());
                }

                if (oldest_time != now)
                {
                    SPDLOG_INFO("[{}]最旧的文件: {}", dir, oldest_file);
                    return true;
                }
                else
                {
                    // SPDLOG_INFO("[{}]无最旧的文件", dir);
                    return false;
                }
            }
    };
}
}
#endif //_CTI_ROBOT_FILE_MONITOR_TIMER_CLEAN_OLD_DATA_HPP_

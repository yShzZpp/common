#ifndef _CTI_ROBOT_FILE_MONITOR_BAG_TIMER_CLEAN_OLD_DATA_HPP_
#define _CTI_ROBOT_FILE_MONITOR_BAG_TIMER_CLEAN_OLD_DATA_HPP_

#include "TimerCleanOldData.hpp"
#include "cti_msgs/BuildingRobotState.h"

using namespace std::literals;

namespace cti_robot
{
namespace file_monitor
{
    class BagTimerCleanOldData : public TimerCleanOldData
    {
        private:

            int mMaxBagsGSize {10};

            unsigned long mMaxBytes;

            bool mTarFlag {false};

            ros::Subscriber mSubRobotState;

        public:

            BagTimerCleanOldData()
            {
                ros::NodeHandle pn("~");
                pn.getParam("bags_max_size", mMaxBagsGSize);

                mMaxBytes = (long)mMaxBagsGSize * (1024*1024*1024);
                SPDLOG_INFO("{} 限制大小为: {}.", mDir, mMaxBytes);
            }
        
            void cleanFunction() override
            {
                while (sumDirSize(mDir) > mMaxBytes)
                {
                    std::string oldest_file;
                    std::vector<std::string> exclude_sub_strs;
                    if (getOldestFileInDir(mDir, oldest_file, exclude_sub_strs))
                    {
                        SPDLOG_INFO("删除文件: {}", oldest_file);
                        boost::filesystem::remove_all(oldest_file);
                    }
                    else
                    {
                        SPDLOG_ERROR("Bag目录出现问题[{}], 无法获取到最旧文件", mDir);
                        ros::Duration(10.0).sleep();
                    }
                }

                if (not mTarFlag)
                {
                    mTarFlag = true;

                    ros::NodeHandle n;
                    mSubRobotState = n.subscribe("/robot_state", 1, &BagTimerCleanOldData::cbRobotState, this);
                }
                return;
            }

            void cbRobotState(const cti_msgs::BuildingRobotState & msg)
            {
                namespace bf = boost::filesystem;

                if (msg.state == cti_msgs::BuildingRobotState::STATE_START
                 || msg.state == cti_msgs::BuildingRobotState::STATE_SELF_CHECKING_CLOUDCONNECT
                 || msg.state == cti_msgs::BuildingRobotState::STATE_WAITTING_TASK
                 || msg.state == cti_msgs::BuildingRobotState::STATE_SELF_CHECKING_SENSORS
                 || msg.state == cti_msgs::BuildingRobotState::STATE_SUBTASK_START
                 || msg.state == cti_msgs::BuildingRobotState::STATE_CHECK_LASTSTATE
                 || msg.state == cti_msgs::BuildingRobotState::STATE_WAITING_NTP
                 || msg.state == cti_msgs::BuildingRobotState::STATE_SELF_GET_POSE
                 || msg.state == cti_msgs::BuildingRobotState::STATE_MAPPING_MODE
                 || msg.state == cti_msgs::BuildingRobotState::STATE_MAP_SYNC
                 || msg.state == cti_msgs::BuildingRobotState::STATE_CHARGING
                 || msg.state == cti_msgs::BuildingRobotState::STATE_RE_LOCATIONING)
                {
                    std::string oldest_file;
                    std::vector<std::string> exclude_sub_strs;
                    exclude_sub_strs.push_back("tar"s);
                    exclude_sub_strs.push_back("active"s);
                    if (getOldestFileInDir(mDir, oldest_file, exclude_sub_strs))
                    {
                        SPDLOG_INFO("压缩: {}", oldest_file);
                        bf::path p(oldest_file);

                        // if (oldest_file.find("active") != std::string::npos
                        //  || oldest_file.find("tar") != std::string::npos)
                        // {
                        //     SPDLOG_INFO("当前最旧的文件已被压缩");
                        // }
                        // else
                        // {
                            std::string cmd = "tar -czf "s + mDir + "/" + p.stem().string() + ".tar.gz "s + oldest_file;
                            SPDLOG_INFO("命令: {}", cmd);
                            auto ret = std::system(cmd.c_str());
                            if (not ret)
                            {
                                bf::remove_all(oldest_file);
                            }
                        // }
                    }
                    else
                    {
                        // SPDLOG_INFO("当前无bag包需要压缩");
                    }
                }
                else
                {
                    SPDLOG_INFO("关闭压缩bag功能,状态 {}", msg.state);
                    mSubRobotState.shutdown();
                }
                return;
            }
    };
}
}

#endif //_CTI_ROBOT_FILE_MONITOR_BAG_TIMER_CLEAN_OLD_DATA_HPP_

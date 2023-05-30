#include "cti_save_crash_log.hpp"
#include "upload_crash_log.hpp"
#include "cti_spdlog.h"
#include "ros/ros.h"

#include "share_handle_storage/SharedHandleStorage.hpp"
#include <boost/filesystem.hpp>
#include <std_srvs/Empty.h>

#include "TimerCleanOldData.hpp"
#include "NaviOtaTimerCleanOldData.hpp"
#include "BagTimerCleanOldData.hpp"

void process_shutdown(std::string file_name)
{
    //将日志上传到服务器
    // cti::robot::uploadlog::UploadLog("crash", file_name);
    spdlog::drop_all();
    ros::shutdown();
    return;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "bag_monitor");

    ros::NodeHandle n;
    cti::buildingrobot::log::SpdLog log("bag_monitor", 1*1024*1024, 60);

    std::string robot_num;
	n.getParam("/robot_attribute/number", robot_num);
    cti::robot::registerSignalHandler(robot_num, "bag_monitor", process_shutdown);

    ros::ServiceClient ntp_service = n.serviceClient<std_srvs::Empty>("/is_ntp_over");
    std_srvs::Empty srv;
    while (ros::ok() && not ntp_service.call(srv))
    {
        SPDLOG_ERROR("NTP尚未完成.");
        ros::Duration(2.0).sleep();
    }

    if (ros::ok())
    {
        SPDLOG_INFO("bag_monitor 启动...sleep 2s 等待时间同步...");

        ros::Duration(2.0).sleep();

        ros::NodeHandle pn("~");

        std::string dir;
        int interval;
        pn.getParam("ros_dir_path", dir);
        pn.getParam("ros_dir_interval", interval);
        cti_robot::file_monitor::TimerCleanOldData ros_dir_timer;
        ros_dir_timer.Init(dir,
                    86400, //1天
                    interval,
                    true,
                    false,
                    true);
        ros_dir_timer.Start();

        pn.getParam("ros_dir_log_path", dir);
        pn.getParam("ros_dir_log_interval", interval);
        cti_robot::file_monitor::TimerCleanOldData ros_dir_log_timer;
        ros_dir_log_timer.Init(dir,
                    86400, //1天
                    interval,
                    true,
                    true,
                    true);
        ros_dir_log_timer.Start();

        pn.getParam("cloud_ota_path", dir);
        pn.getParam("cloud_ota_interval", interval);
        cti_robot::file_monitor::TimerCleanOldData cloud_ota_timer;
        cloud_ota_timer.Init(dir,
                    86400, //1天
                    interval,
                    true,
                    true,
                    true);
        cloud_ota_timer.Start();

        pn.getParam("navi_ota_path", dir);
        pn.getParam("navi_ota_interval", interval);
        cti_robot::file_monitor::NaviOtaTimerCleanOldData navi_ota_timer;
        navi_ota_timer.Init(dir,
                    86400, //1天
                    interval,
                    true,
                    true,
                    true);
        navi_ota_timer.Start();

        pn.getParam("bag_path", dir);
        pn.getParam("bag_interval", interval);
        cti_robot::file_monitor::BagTimerCleanOldData bag_timer;
        bag_timer.Init(dir,
                    interval,
                    0,  //未使用
                    false, //未使用
                    false, //未使用
                    true);
        bag_timer.Start();

        ros::spin();

        ros_dir_timer.Stop();
        ros_dir_log_timer.Stop();
        cloud_ota_timer.Stop();
        navi_ota_timer.Stop();
        bag_timer.Stop();
    }
    else
    {
        SPDLOG_ERROR("bag_monitor 未启动就被退出...");
    }

    return 0;
}

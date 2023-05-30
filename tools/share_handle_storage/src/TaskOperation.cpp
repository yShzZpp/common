#include "share_handle_storage/TaskOperation.hpp"
#include "cti_msgs/CloudTaskRequest.h"
#include "cti_msgs/Task.h"

using namespace std::literals;

namespace cti_robot
{
    namespace tool
    {
        namespace task
        {
            std::string requestActionToString(unsigned int action)
            {
                switch (action)
                {
                    case cti_msgs::CloudTaskRequest::ACTION_EXEC: { return "执行任务"s; }
                    case cti_msgs::CloudTaskRequest::ACTION_PAUSE: { return "暂停任务"s; }
                    case cti_msgs::CloudTaskRequest::ACTION_RESUME: { return "继续(恢复)任务"s; }
                    case cti_msgs::CloudTaskRequest::ACTION_ABORT_ALL: { return "中止任务"s; }
                    default: {return "未知命令"s;}
                }
            }

            std::string taskTypeToString(unsigned int task_type)
            {
                switch (task_type)
                {
                    case cti_msgs::Task::TYPE_NAVIGATION: { return "移动"s; }
                    case cti_msgs::Task::TYPE_RE_LOCATION: { return "重定位"s; }
                    case cti_msgs::Task::TYPE_MOUNT_BOX: { return "装箱"s; }
                    case cti_msgs::Task::TYPE_UNMOUNT_BOX: { return "卸箱"s; }
                    case cti_msgs::Task::TYPE_CHARGE: { return "充电"s; }
                    case cti_msgs::Task::TYPE_WAIT_IN_SUIT: { return "原地等待"s; }
                    case cti_msgs::Task::TYPE_SELF_LOCATION: { return "自主定位"s; }
                    case cti_msgs::Task::TYPE_MAP_SYNC: { return "同步地图"s; }
                    case cti_msgs::Task::TYPE_WASH_FLOOR: { return "清洗"s; }
                    case cti_msgs::Task::TYPE_DINFECT_FLOOR: { return "消毒"s; }
                    case cti_msgs::Task::TYPE_PUSH_MAPS: { return "推送地图"s; }
                    case cti_msgs::Task::TYPE_MAPPING: { return "启动建图"s; }
                    case cti_msgs::Task::TYPE_FIND_ALL_QR: { return "寻找所有二维码"s; }

                    case cti_msgs::Task::TYPE_DIRECT_LIFTDOWN_BOX: { return "直接抬升"s; }
                    case cti_msgs::Task::TYPE_REMOTE_CONTROL: { return "远程控制"s; }
                    case cti_msgs::Task::TYPE_REBOOT_MACHINE: { return "重启机器"s; }
                    case cti_msgs::Task::TYPE_SWITCH_NAVIGATION_MODE: { return "切换成工作模式"s; }
                    case cti_msgs::Task::TYPE_SWITCH_MAPPING_MODE: { return "切换成建图模式"s; }
                    case cti_msgs::Task::TYPE_SWITCH_TESTMAP_MODE: { return "切换成测试模式"s; }
                    case cti_msgs::Task::TYPE_OPEN_WIFI_HOTSPOT: { return "打开wifi热点"s; }
                    case cti_msgs::Task::TYPE_CLOSE_WIFI_HOTSPOT: { return "关闭wifi热点"s; }

                    case cti_msgs::Task::TYPE_SCHEDULE_DODGE: { return "躲避机器"s; }
                    case cti_msgs::Task::TYPE_SCHEDULE_WAIT: { return "避让机器"s; }

                    case cti_msgs::Task::TYPE_FLATLAYER_NAVIGATION: { return "平层导航"s; }
                    case cti_msgs::Task::TYPE_UP_DOWN_ELEVATOR: { return "乘坐电梯"s; }
                    case cti_msgs::Task::TYPE_NAVIGATION_ELEVATOR: { return "移动到电梯前"s; }
                    case cti_msgs::Task::TYPE_BOOT_UP_SELF_CHECK: { return "开机自检"s; }
                    case cti_msgs::Task::TYPE_CHECKSTATE_BE_POWEROFF: { return "检查机器断电前状态"s; }
                    case cti_msgs::Task::TYPE_SELF_CHECK_SENSORS: { return "自检传感器"s; }
                    case cti_msgs::Task::TYPE_FIND_LOCATION_USING_QR: { return "用二维码重定位"s; }
                    case cti_msgs::Task::TYPE_CHECK_NTP: { return "自检网络时间同步"s; }
                    case cti_msgs::Task::TYPE_CHECK_CLOUD_CONNECT: { return "自检连接云服务器"s; }
                    case cti_msgs::Task::TYPE_SELF_GET_POSE: { return "自动寻找位置"s; }
                    case cti_msgs::Task::TYPE_BOOT_UP_SELF_LOCATION: { return "开机自动定位"s; }
                    default: { return "未知任务"s; }
                }
            }
        }
    }
}
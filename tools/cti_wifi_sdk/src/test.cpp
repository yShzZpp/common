
#include "cti_wifi_sdk/wifi.h"
#include "cti_spdlog.h"
#include "ros/ros.h"

//wrx   添加头文件,g_main_context_pending先关的函数.
#include <glib.h>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "test_wifi");

    ros::NodeHandle n;

    cti::buildingrobot::log::SpdLog log("test_wifi", 1*1024*1024, 10);

    SPDLOG_INFO("---test start----.");


    while(ros::ok())
    {
        char message[512];
        ros::Duration(5.0).sleep();
        SPDLOG_INFO("-------.");
        SPDLOG_INFO("Start to connect wifi.");

        printf("Start to connect wifi.\n");

        int ret;
        while ((ret = connect_wifi("wlp3s0", "tikong-00369", "tikong-4g", false, message)) != 0)  
        {
            SPDLOG_INFO("connect wifi fail sleep and try again.");

            //wrx   只要有死循环的地方都要加上这个.
            while (g_main_context_pending(NULL))
                g_main_context_iteration(NULL, TRUE);

            ros::Duration(0.5).sleep();
        }

        SPDLOG_INFO("connect wifi done, and ping wifi");
        printf("connect wifi done, and ping wifi\n");
        ros::Duration(0.5).sleep();


        unsigned int fail_times = 0;
        unsigned int disconnect_times = 0;
        while ((ret = check_connectivity("wlp3s0", "tikong-00xxx", "192.168.1.", message)) != 0)
        {
            fail_times++;
            disconnect_times++;
            SPDLOG_INFO("ping wifi failed. times: {}", fail_times);
            printf("ping wifi failed. times: %d\n", fail_times);

            ros::Duration(0.5).sleep();

            if (fail_times == 30)
            {
                SPDLOG_INFO("ping wifi failed. connect again");

                while ((ret = connect_wifi("wlp3s0", "tikong-00369", "tikong-4g", false, message)) != 0)  
                {
                    SPDLOG_INFO("ping wifi failed. connect wifi fail sleep and try again.");

                    //wrx   只要有死循环的地方都要加上这个.
                    while (g_main_context_pending(NULL))
                        g_main_context_iteration(NULL, TRUE);
                    ros::Duration(0.5).sleep();
                }
                fail_times = 0;
            }
            
            if (disconnect_times == 90)
            {
                SPDLOG_INFO("long time ping fail.");
                break;
            }
        }

        if (disconnect_times != 90)
        {
            SPDLOG_INFO("ping wifi success");
            printf("ping wifi success,sleep 30s\n");
            ros::Duration(30.0).sleep();
        }

        SPDLOG_INFO("disconnect wifi");
        disconnect_wifi("wlp3s0", message);



        ros::Duration(0.5).sleep();
        SPDLOG_INFO("remove ssid");

        remove_conn("tikong-00369", message);

        //wrx   只要有死循环的地方都要加上这个.
        while (g_main_context_pending(NULL))
    		g_main_context_iteration(NULL, TRUE);
    }

    // ros::spin();

    return 0;
}
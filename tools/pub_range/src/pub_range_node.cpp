#include <ros/ros.h>
#include <sensor_msgs/Range.h>
#include <cti_msgs/RangeArray.h>

#define ULTRASONIC_NUM 10
std::string ultr_frame_id[ULTRASONIC_NUM] = {
    "ultr_head_left",        // 0    头左
    "ultr_head_right",       // 1    头右
    "ultr_front",            // 2    脖子
    "ultr_front_left",       // 3    前左
    "ultr_front_right",      // 4    前右
    "ultr_left_side_front",  // 5    左侧前
    "ultr_left_side_back",   // 6    左侧后
    "ultr_right_side_front", // 7    右侧前
    "ultr_right_side_back",  // 8    右侧后
    "ultr_back"              // 9    车后
};

ros::Publisher PubArray[ULTRASONIC_NUM];

void ultrasonicCB(const cti_msgs::RangeArray::ConstPtr & msg_in)
{
    for(int i = 0 ; i< msg_in->ranges.size();i++)
    {
        sensor_msgs::Range msg_out;
        msg_out.header = msg_in->ranges[i].header;
        msg_out.radiation_type= msg_in->ranges[i].radiation_type;
        msg_out.min_range = msg_in->ranges[i].min_range;
        msg_out.max_range = msg_in->ranges[i].max_range;
        msg_out.field_of_view = msg_in->ranges[i].field_of_view;
        msg_out.range = msg_in->ranges[i].range;
        PubArray[i].publish(msg_out);
    }
}

int main(int argc,char **argv)
{
 double rate_;
    ros::init(argc, argv, "pub_range_node");

    ros::NodeHandle nh;
    ros::NodeHandle private_nh("~");
    private_nh.param<double>("rate", rate_, 200.0);
    ros::Rate loop_rate(rate_);


    ros::Subscriber rang_array_sub = nh.subscribe("/robot_base/ultrasonic", 11, &ultrasonicCB);

    for(int i = 0 ; i< ULTRASONIC_NUM;i++)
    {
        std::string topic_name = "c"+std::to_string(i+1);
        // PubArray[i] = nh.advertise<sensor_msgs::Range>(ultr_frame_id[i], 1);
        PubArray[i] = nh.advertise<sensor_msgs::Range>(topic_name, 1);
    }
    ros::spin();
return 0;
}

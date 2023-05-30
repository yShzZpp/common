#include <ros/package.h>
#include "robot_description/static_transform_component.h"

int main(int argc, char ** argv)
{
    // ros::init(argc, argv,"static_transform_node", ros::init_options::AnonymousName);
    ros::init(argc, argv,"static_transform_node");

    ros::NodeHandle nh_;
    ros::NodeHandle pnh_("~");

    // n.getParam("freq", freq);
    double frequency_;
	pnh_.param("frequency", frequency_, 20.0);

    std::string config_file_;
	pnh_.param("config_file", config_file_, std::string("/config/static_transform_conf.txt"));

    ros::Duration sleeper(frequency_/1000.0);

    cti::common::StaticTransformComponent static_transform_(nh_, config_file_);

    while (nh_.ok())
    {
        static_transform_.SendTransforms(ros::Time::now());
        sleeper.sleep();
    }

    return 0;
}

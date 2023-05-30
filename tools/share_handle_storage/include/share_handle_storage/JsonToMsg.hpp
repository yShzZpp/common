#ifndef _CTI_ROBOT_JSON_JSON_TO_MSG_HPP_
#define _CTI_ROBOT_JSON_JSON_TO_MSG_HPP_

#include <cpprest/json.h>
#include <geometry_msgs/Point32.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Polygon.h>
#include "cti_msgs/SegmentLine.h"

using namespace std::literals;

namespace cti_robot
{
    namespace json
    {
        bool GetIntVauleFrommJson(int & variable,
                                web::json::value params,
                                std::string key, 
                                int default_value=0);

        bool GetDoubleVauleFrommJson(double & variable,
                                    web::json::value params,
                                    std::string key, 
                                    double default_value=0.0);

        bool GetStringVauleFrommJson(std::string & variable,
                                    web::json::value params,
                                    std::string key, 
                                    std::string default_value=""s);

        bool GetBoolVauleFrommJson(bool & variable,
                                    web::json::value params,
                                    std::string key, 
                                    bool default_value=false);

        bool JsonToPosition(web::json::value json, geometry_msgs::Point & position);

        bool JsonToQuaternion(web::json::value json, geometry_msgs::Quaternion & orientation);

        bool JsonToPose(web::json::value json, geometry_msgs::Pose & pose);

        bool JsonToPose(web::json::value json, geometry_msgs::PoseStamped & pose_stamp);

        bool JsonToPoint32(web::json::value json, geometry_msgs::Point32 & point);

        bool JsonToSegmentLine(web::json::value json, cti_msgs::SegmentLine & line);

        bool JsonToPolygon(web::json::value json, geometry_msgs::Polygon & polygon);
    }
}
#endif //_CTI_ROBOT_JSON_JSON_TO_MSG_HPP_
#ifndef _CTI_ROBOT_SHARED_TOOL_MSG_TO_JSON_HPP_
#define _CTI_ROBOT_SHARED_TOOL_MSG_TO_JSON_HPP_

#include <cpprest/json.h>

#include <sensor_msgs/LaserScan.h>
#include <nav_msgs/Odometry.h>
#include <nav_msgs/Path.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/TwistStamped.h>
#include <geometry_msgs/Polygon.h>
#include "cti_msgs/PolygonArray.h"

namespace cti_robot{
namespace shared_tool{
namespace json{

    web::json::value HeaderToJson(std_msgs::Header header);

    web::json::value QuaternionToJson(geometry_msgs::Quaternion orientation);

    web::json::value PositionToJson(geometry_msgs::Point position);

    web::json::value Vector3ToJson(geometry_msgs::Vector3 vector3);

    web::json::value TwistToJson(geometry_msgs::Twist twist);

    web::json::value TwistToJson(geometry_msgs::TwistWithCovariance twist_with_covar);

    web::json::value TwistToJson(geometry_msgs::TwistStamped twist_stamp);

    web::json::value PoseToJson(geometry_msgs::Pose pose);

    web::json::value PoseToJson(geometry_msgs::PoseStamped pose_stamp);

    web::json::value PoseToJson(geometry_msgs::PoseWithCovariance pose_with_covar);

    //考虑inf和nan问题
    web::json::value OdomToJson(nav_msgs::Odometry odom);

    //考虑inf和nan问题
    web::json::value LaserScanToJson(sensor_msgs::LaserScan scan);

    web::json::value PathToJson(nav_msgs::Path path);

    web::json::value PolygonToJson(geometry_msgs::Polygon polygon);

    web::json::value SegmentedPolygonsToJson(std::vector<cti_msgs::PolygonArray> segmented_polygons);

}}}

#endif //_CTI_ROBOT_SHARED_TOOL_MSG_TO_JSON_HPP_
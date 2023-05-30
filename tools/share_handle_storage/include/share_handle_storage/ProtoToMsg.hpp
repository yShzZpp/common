#ifndef _CTI_ROBOT_SHARED_TOOL_PROTO_PROTO_TO_MSG_HPP_
#define _CTI_ROBOT_SHARED_TOOL_PROTO_PROTO_TO_MSG_HPP_

#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Polygon.h>
#include <nav_msgs/Path.h>
#include "common/proto/geometry.pb.h"

namespace cti_robot {
namespace shared_tool{
namespace proto {

    bool ProtoToPoint(cti::common::Point proto, geometry_msgs::Point & position);

    bool ProtoToPoint(cti::common::Point proto, geometry_msgs::Point32 & position);

    bool ProtoToPoint(cti::common::Pose proto, geometry_msgs::Point32 & point);

    bool ProtoToQuaternion(cti::common::Quaternion proto, geometry_msgs::Quaternion & orientation);

    bool ProtoToPose(cti::common::Pose proto, geometry_msgs::Pose & pose);

    bool ProtoToPointArray(google::protobuf::RepeatedPtrField<cti::common::Point> * points, geometry_msgs::Polygon & point_array);

    bool ProtoToPointArray(google::protobuf::RepeatedPtrField<cti::common::Pose> * poses, geometry_msgs::Polygon & point_array);

    bool ProtoToPonitArray(cti::common::Polygon polygon, geometry_msgs::Polygon & point_array);

    bool ProtoToPonitArray(cti::common::LineSegment line_segment, geometry_msgs::Polygon & point_array);

    bool ProtoToPonitArray(cti::common::Path path, geometry_msgs::Polygon & point_array);

    bool ProtoToPonitArray(cti::common::Path path, nav_msgs::Path & point_array);

}}}

#endif //_CTI_ROBOT_SHARED_TOOL_PROTO_PROTO_TO_MSG_HPP_
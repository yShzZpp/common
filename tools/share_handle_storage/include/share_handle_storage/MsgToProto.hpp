#ifndef _CTI_ROBOT_SHARED_TOOL_PROTO_MSG_TO_PROTO_HPP_
#define _CTI_ROBOT_SHARED_TOOL_PROTO_MSG_TO_PROTO_HPP_

#include <geometry_msgs/Pose.h>
#include <nav_msgs/Path.h>
#include "common/proto/geometry.pb.h"

namespace cti_robot {
namespace shared_tool{
namespace proto {

    void PointToProto(geometry_msgs::Point position, cti::common::Point * proto);

    void QuaternionToProto(geometry_msgs::Quaternion orientation, cti::common::Quaternion * proto);

    void PoseToProto(geometry_msgs::Pose pose, cti::common::Pose * proto);

    void PathToProto(nav_msgs::Path path, cti::common::Path * proto);

    void PathToProto(nav_msgs::Path path, cti::common::Polygon * proto);

}}}

#endif //_CTI_ROBOT_SHARED_TOOL_PROTO_MSG_TO_PROTO_HPP_
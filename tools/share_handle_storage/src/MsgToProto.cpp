
#include "share_handle_storage/MsgToProto.hpp"

namespace cti_robot {
namespace shared_tool{
namespace proto {

    void PointToProto(geometry_msgs::Point position, cti::common::Point * proto)
    {
        proto->set_x(position.x);
        proto->set_y(position.y);
        proto->set_z(position.z);
    }

    void QuaternionToProto(geometry_msgs::Quaternion orientation, cti::common::Quaternion * proto)
    {
        proto->set_x(orientation.x);
        proto->set_y(orientation.y);
        proto->set_z(orientation.z);
        proto->set_w(orientation.w);
    }

    void PoseToProto(geometry_msgs::Pose pose, cti::common::Pose * proto)
    {
        PointToProto(pose.position, proto->mutable_position());
        QuaternionToProto(pose.orientation, proto->mutable_orientation());
    }

    void PathToProto(nav_msgs::Path path, cti::common::Path * proto)
    {
        for (auto pose : path.poses)
        {
            PoseToProto(pose.pose, proto->add_poses());
        }
    }

    void PathToProto(nav_msgs::Path path, cti::common::Polygon * proto)
    {
        for (auto pose : path.poses)
        {
            PointToProto(pose.pose.position, proto->add_point());
        }
    }

}}}

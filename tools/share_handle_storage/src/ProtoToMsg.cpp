#include "share_handle_storage/ProtoToMsg.hpp"

namespace cti_robot {
namespace shared_tool{
namespace proto {

    bool ProtoToPoint(cti::common::Point proto, geometry_msgs::Point & position)
    {
        if (proto.has_x() && proto.has_y() && proto.has_z())
        {
            position.x = proto.x();
            position.y = proto.y();
            position.z = proto.z();
            return true;
        }
        return false;
    }

    bool ProtoToPoint(cti::common::Point proto, geometry_msgs::Point32 & position)
    {
        if (proto.has_x() && proto.has_y() && proto.has_z())
        {
            position.x = proto.x();
            position.y = proto.y();
            position.z = proto.z();
            return true;
        }
        return false;
    }

    bool ProtoToPoint(cti::common::Pose proto, geometry_msgs::Point32 & point)
    {
        return proto.has_position() 
            && ProtoToPoint(proto.position(), point);
    }

    bool ProtoToQuaternion(cti::common::Quaternion proto, geometry_msgs::Quaternion & orientation)
    {
        if (proto.has_x() && proto.has_y() && proto.has_z() && proto.has_w())
        {
            orientation.x = proto.x();
            orientation.y = proto.y();
            orientation.z = proto.z();
            orientation.w = proto.w();
            return true;
        }
        return false;
    }

    bool ProtoToPose(cti::common::Pose proto, geometry_msgs::Pose & pose)
    {
        return proto.has_position() 
            && ProtoToPoint(proto.position(), pose.position)
            && proto.has_orientation() 
            && ProtoToQuaternion(proto.orientation(), pose.orientation);
    }

    bool ProtoToPointArray(google::protobuf::RepeatedPtrField<cti::common::Point> * points, geometry_msgs::Polygon & point_array)
    {
        bool result = true;
        for (int i=0; i<points->size(); i++)
        {
            geometry_msgs::Point32 msg_point;
            if (ProtoToPoint(*points->Mutable(i), msg_point))
            {
                point_array.points.emplace_back(msg_point);
            }
            else
            {
                result = false;
                break;
            }
        }
        return result;
    }

    bool ProtoToPointArray(google::protobuf::RepeatedPtrField<cti::common::Pose> * poses, geometry_msgs::Polygon & point_array)
    {
        bool result = true;
        for (int i=0; i<poses->size(); i++)
        {
            geometry_msgs::Point32 msg_point;
            if (ProtoToPoint(*poses->Mutable(i), msg_point))
            {
                point_array.points.emplace_back(msg_point);
            }
            else
            {
                result = false;
                break;
            }
        }
        return result;
    }

    bool ProtoToPonitArray(cti::common::Polygon polygon, geometry_msgs::Polygon & point_array)
    {
        return ProtoToPointArray(polygon.mutable_point(), point_array);
    }

    bool ProtoToPonitArray(cti::common::LineSegment line_segment, geometry_msgs::Polygon & point_array)
    {
        return ProtoToPointArray(line_segment.mutable_point(), point_array);
    }

    bool ProtoToPonitArray(cti::common::Path path, geometry_msgs::Polygon & point_array)
    {
        return ProtoToPointArray(path.mutable_poses(), point_array);
    }

    bool ProtoToPonitArray(cti::common::Path path, nav_msgs::Path & point_array)
    {
        bool result = true;
        for (int i=0; i<path.poses_size(); i++)
        {
            geometry_msgs::PoseStamped msg_pose;
            if (ProtoToPose(path.poses(i), msg_pose.pose))
            {
                point_array.poses.emplace_back(msg_pose);
            }
            else
            {
                result = false;
                break;
            }
        }
        return result;
    }

}}}

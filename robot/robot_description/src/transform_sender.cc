#include "robot_description/transform_sender.h"


namespace cti {
namespace common {

TransformSender::TransformSender(const ros::NodeHandle& nh, 
                double x, double y, double z, double yaw, double pitch, double roll, 
                ros::Time time, const std::string& frame_id, const std::string& child_frame_id)
{
    nh_ = nh;
    tf::Quaternion q;
    q.setRPY(roll, pitch, yaw); 
    stamped_transform_ = tf::StampedTransform(tf::Transform(q, tf::Vector3(x,y,z)), 
                                                    time, 
                                                    frame_id, child_frame_id );
}

TransformSender::TransformSender(const ros::NodeHandle& nh, TransformEuler *transform, const ros::Time& time)
{
    nh_ = nh;
    tf::Quaternion q;
    q.setRPY(transform->mutable_rotation()->roll(), transform->mutable_rotation()->pitch(), transform->mutable_rotation()->yaw()); 
    stamped_transform_ = tf::StampedTransform(tf::Transform(q,  tf::Vector3(transform->mutable_translation()->x(),
                                                                    transform->mutable_translation()->y(),
                                                                    transform->mutable_translation()->z())),
                                                time,
                                                transform->frame_id(),
                                                transform->child_frame_id());

}

void TransformSender::send(ros::Time time)
{
    static tf::TransformBroadcaster broadcaster_;

    stamped_transform_.stamp_ = time;
    broadcaster_.sendTransform(stamped_transform_);
}

}
}
#include <cstdio>
#include <ros/ros.h>
#include "tf/transform_broadcaster.h"

#include "common/proto/geometry.pb.h"
#include "common/proto/transform.pb.h"

namespace cti {
namespace common {

class TransformSender 
{
    public:
        TransformSender(const ros::NodeHandle& nh, 
                double x, double y, double z, double yaw, double pitch, double roll, 
                ros::Time time, const std::string& frame_id, const std::string& child_frame_id);
        
        TransformSender(const ros::NodeHandle& nh, TransformEuler *transform, const ros::Time& time);

        ~TransformSender() {}

        void send(ros::Time time);

        ros::NodeHandle nh_;

    private:
        tf::StampedTransform stamped_transform_;
};

}
}
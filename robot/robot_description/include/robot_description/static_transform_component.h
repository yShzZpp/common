#include <cstdio>
#include <ros/ros.h>

#include "yaml-cpp/yaml.h"

#include "common/io/io.h"
#include "common/proto/transform.pb.h"
#include "common/proto/transform_description.pb.h"
#include "robot_description/proto/static_transform_config.pb.h"
#include "transform_sender.h"

namespace cti {
namespace common {

class StaticTransformComponent 
{
    public:
        StaticTransformComponent(const ros::NodeHandle& nh, const std::string& file_path);

        ~StaticTransformComponent() {}

        bool SendTransforms(const ros::Time& time);

    private:
        bool LoadConfigFiles();

        bool ParasFromYaml(const std::string& file_path, TransformEuler *transform);
        
        bool ShowTranform(void);

        ros::NodeHandle nh_;

        std::string file_path_;

        std::string package_path_;

        std::string machine_type_;

        std::vector<TransformEuler> tranform_sender_vec_;

};

}
}
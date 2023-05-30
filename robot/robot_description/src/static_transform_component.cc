#include "robot_description/static_transform_component.h"


namespace cti {
namespace common {

StaticTransformComponent::StaticTransformComponent(const ros::NodeHandle& nh, const std::string& file_path) : nh_(nh), file_path_(file_path)
{
    ROS_INFO("file_path:%s",file_path.c_str());
    
    package_path_ = ros::package::getPath("robot_description");
    ros::NodeHandle ph_("~");
    
	ph_.param("machine_type", machine_type_, std::string("BR204"));
    ROS_INFO("[robot_description] show machine_type: %s",machine_type_.c_str());

    LoadConfigFiles();
    ShowTranform();
}

bool StaticTransformComponent::LoadConfigFiles()
{
    StaticTransformConfig cfg_;
    if (!ReadProtoFromTextFile(file_path_ , &cfg_)) 
    {
        ROS_ERROR("Static Transform conifg file loading failed. : %s", file_path_.c_str());
        return false;
    }

    tranform_sender_vec_.clear();
    for (auto& config : cfg_.static_transform_config()) {
        if(config.enable()) 
        {
            
            std::string full_path = package_path_ + "/config/" + machine_type_ + "/" + config.file_path();
            if(full_path.find(".yaml") != std::string::npos)
            {   
                TransformEuler transform;
                if (ParasFromYaml(config.file_path(), &transform)) 
                {
                    tranform_sender_vec_.emplace_back(transform);
                }
            }
            else
            {
                StaticTransformTree tree_;
                if (!ReadProtoFromTextFile(full_path, &tree_)) 
                {
                    ROS_ERROR("Static Transform conifg file loading failed. : %s", full_path.c_str());
                    return false;
                }
                for(auto &tr:tree_.transform_description()) 
                {
                    tranform_sender_vec_.emplace_back(tr);
                }
            }
        }
    }
    // ROS_INFO("Static Transform config file loading successed.");
    return true;
}


bool StaticTransformComponent::SendTransforms(const ros::Time& time)
{
    if(tranform_sender_vec_.empty()) 
    {
        return false;
    }

    for (auto &tr : tranform_sender_vec_)
    {
        TransformSender transform_sender(nh_, &tr, time);
        transform_sender.send(time);
    }

    return true;
}

bool StaticTransformComponent::ParasFromYaml(const std::string& file_path, TransformEuler *transform)
{
    if(! cti::common::PathExists(file_path)) {
        ROS_INFO("Static Transform conifg file path. : %s", file_path.c_str());
        return false;
    }
            
    YAML::Node tf = YAML::LoadFile(file_path);

    try {
        transform->set_frame_id(tf["frame_id"].as<std::string>());
        transform->set_child_frame_id(tf["child_frame_id"].as<std::string>());

        transform->mutable_translation()->set_x(tf["translation"]["x"].as<double>());
        transform->mutable_translation()->set_y(tf["translation"]["y"].as<double>());
        transform->mutable_translation()->set_z(tf["translation"]["z"].as<double>());
    
        transform->mutable_rotation()->set_roll(tf["rotation"]["roll"].as<double>());
        transform->mutable_rotation()->set_pitch(tf["rotation"]["pitch"].as<double>());
        transform->mutable_rotation()->set_yaw(tf["rotation"]["yaw"].as<double>());

    } catch(...) {
        ROS_ERROR("Yaml file parse failed : %s", file_path.c_str());
        return false;
    }

    return true;
}

bool StaticTransformComponent::ShowTranform(void)
{
    for(auto &tr:tranform_sender_vec_)
    {
        ROS_INFO("\nframe %s ---> %s\n\tTransform(%f %f %f) \n\tRotation(%f %f %f)\n",
        tr.frame_id().c_str(),tr.child_frame_id().c_str(),
        tr.mutable_translation()->x(),tr.mutable_translation()->y(),tr.mutable_translation()->z(),
        tr.mutable_rotation()->roll(),tr.mutable_rotation()->pitch(),tr.mutable_rotation()->yaw());
    }
}

}
}
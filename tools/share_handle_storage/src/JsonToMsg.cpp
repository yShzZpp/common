#include "share_handle_storage/JsonToMsg.hpp"
// #include "spdlog/spdlog.h"

using namespace std::literals;

namespace cti_robot
{
    namespace json
    {
        bool GetIntVauleFrommJson(int & variable,
                                web::json::value params,
                                std::string key, 
                                int default_value)
        {
            if (params.has_field(key) && params[key].is_integer())
            {
                variable = params[key].as_integer();
                // SPDLOG_INFO("Json param {} : {}", key, variable);
                return true;
            }
            else
            {
                variable = default_value;
                // SPDLOG_ERROR("Config file don't has key {}", key);
                return false;
            }
        }

        bool GetDoubleVauleFrommJson(double & variable,
                                    web::json::value params,
                                    std::string key, 
                                    double default_value)
        {
            if (params.has_field(key))
            {
                if (params[key].is_double())
                {
                    variable = params[key].as_double();
                    // SPDLOG_INFO("Json param {} : {}", key, variable);
                    return true;
                }
                else if (params[key].is_integer())
                {
                    variable = (double)(params[key].as_integer());
                    // SPDLOG_INFO("Json param {} : {}", key, variable);
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                variable = default_value;
                // SPDLOG_ERROR("Config file don't has key {}", key);
                return false;
            }
        }

        bool GetStringVauleFrommJson(std::string & variable,
                                    web::json::value params,
                                    std::string key, 
                                    std::string default_value)
        {
            if (params.has_field(key) && params[key].is_string())
            {
                variable = params[key].as_string();
                // SPDLOG_INFO("Json param {} : {}", key, variable);
                return true;
            }
            else
            {
                variable = default_value;
                // SPDLOG_ERROR("Config file don't has key {}", key);
                return false;
            }
        }

        bool GetBoolVauleFrommJson(bool & variable,
                                    web::json::value params,
                                    std::string key, 
                                    bool default_value)
        {
            if (params.has_field(key) && params[key].is_boolean())
            {
                variable = params[key].as_bool();
                // SPDLOG_INFO("Json param {} : {}", key, variable);
                return true;
            }
            else
            {
                variable = default_value;
                // SPDLOG_ERROR("Config file don't has key {}", key);
                return false;
            }
        }

        bool JsonToPosition(web::json::value json, geometry_msgs::Point & position)
        {
            return GetDoubleVauleFrommJson(position.x, json, "x"s)
                && GetDoubleVauleFrommJson(position.y, json, "y"s)
                && GetDoubleVauleFrommJson(position.z, json, "z"s);
        }

        bool JsonToQuaternion(web::json::value json, geometry_msgs::Quaternion & orientation)
        {
            return GetDoubleVauleFrommJson(orientation.x, json, "x"s)
                && GetDoubleVauleFrommJson(orientation.y, json, "y"s)
                && GetDoubleVauleFrommJson(orientation.z, json, "z"s)
                && GetDoubleVauleFrommJson(orientation.w, json, "w"s);
        }

        bool JsonToPose(web::json::value json, geometry_msgs::Pose & pose)
        {
            if (json.has_field("position") && json.has_field("orientation"))
            {
                return JsonToPosition(json["position"], pose.position)
                    && JsonToQuaternion(json["orientation"], pose.orientation);
            }
            return false;
        }

        bool JsonToPose(web::json::value json, geometry_msgs::PoseStamped & pose_stamp)
        {
            return JsonToPose(json, pose_stamp.pose);
        }

        bool JsonToPoint32(web::json::value json, geometry_msgs::Point32 & point)
        {
            // SPDLOG_INFO("JsonToPoint32 : {}", json.serialize());
            double x, y, z;
            if (GetDoubleVauleFrommJson(x, json, "x"s)
             && GetDoubleVauleFrommJson(y, json, "y"s)
             && GetDoubleVauleFrommJson(z, json, "z"s))
            {
                point.x = x;
                point.y = y;
                point.z = z;
                return true;
            }
            else
            {
                return false;
            } 
        }
    
        bool JsonToSegmentLine(web::json::value json, cti_msgs::SegmentLine & line)
        {
            bool result = false;
            if (json.has_field("start_point") && json.has_field("end_point"))
            {
                if (JsonToPoint32(json["start_point"], line.start_point)
                 && JsonToPoint32(json["end_point"],   line.end_point))
                {
                    result = true;
                }
            }
            return result;
        }

        bool JsonToPolygon(web::json::value json, geometry_msgs::Polygon & polygon)
        {
            bool result = false;
            if (json.has_field("points") && json["points"].is_array())
            {
                for (auto json_point : json["points"].as_array())
                {
                    geometry_msgs::Point32 point32;
                    if (JsonToPoint32(json_point, point32))
                    {
                        polygon.points.push_back(point32);
                    }
                    else
                    {
                        return false;
                    }
                }
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}

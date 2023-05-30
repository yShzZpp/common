#include "share_handle_storage/MsgToJson.hpp"

namespace cti_robot{
namespace shared_tool{
namespace json{

    web::json::value HeaderToJson(std_msgs::Header header)
    {
        return web::json::value::object({
            {"seq", web::json::value(header.seq)},
            {"stamp", web::json::value(header.stamp.toSec())},
            {"frame_id", web::json::value(header.frame_id)},
        });
    }

    web::json::value QuaternionToJson(geometry_msgs::Quaternion orientation)
    {
        return web::json::value::object({
            {"x", web::json::value(orientation.x)},
            {"y", web::json::value(orientation.y)},
            {"z", web::json::value(orientation.z)},
            {"w", web::json::value(orientation.w)}
        });
    }

    web::json::value PositionToJson(geometry_msgs::Point position)
    {
        return web::json::value::object({
            {"x", web::json::value(position.x)},
            {"y", web::json::value(position.y)},
            {"z", web::json::value(position.z)}
        });
    }

    web::json::value Vector3ToJson(geometry_msgs::Vector3 vector3)
    {
        return web::json::value::object({
            {"x", web::json::value(vector3.x)},
            {"y", web::json::value(vector3.y)},
            {"z", web::json::value(vector3.z)}
        });
    }

    web::json::value TwistToJson(geometry_msgs::Twist twist)
    {
        return web::json::value::object({
            {"linear", web::json::value(Vector3ToJson(twist.linear))},
            {"angular", web::json::value(Vector3ToJson(twist.angular))}
        });
    }

    web::json::value TwistToJson(geometry_msgs::TwistWithCovariance twist_with_covar)
    {
        return TwistToJson(twist_with_covar.twist);
    }

    web::json::value TwistToJson(geometry_msgs::TwistStamped twist_stamp)
    {
        return TwistToJson(twist_stamp.twist);
    }

    web::json::value PoseToJson(geometry_msgs::Pose pose)
    {
        return web::json::value::object({
            {"position", web::json::value(PositionToJson(pose.position))},
            {"orientation", web::json::value(QuaternionToJson(pose.orientation))},
        });
    }

    web::json::value Point32ToJson(geometry_msgs::Point32 point)
    {
        return web::json::value::object({
            {"x", web::json::value(point.x)},
            {"y", web::json::value(point.y)},
            {"z", web::json::value(point.z)}
        });
    }

    web::json::value PoseToJson(geometry_msgs::PoseStamped pose_stamp)
    {
        return PoseToJson(pose_stamp.pose);
    }

    web::json::value PoseToJson(geometry_msgs::PoseWithCovariance pose_with_covar)
    {
        return PoseToJson(pose_with_covar.pose);
    }

    //考虑inf和nan问题
    web::json::value OdomToJson(nav_msgs::Odometry odom)
    {
        return web::json::value::object({
            {"header", web::json::value(HeaderToJson(odom.header))},
            {"pose", web::json::value(PoseToJson(odom.pose))},
            {"twist", web::json::value(TwistToJson(odom.twist))},
        });
    }

    //考虑inf和nan问题
    web::json::value LaserScanToJson(sensor_msgs::LaserScan scan)
    {
        auto ranges = web::json::value::array();
        for (auto range : scan.ranges)
        {
            if (std::isinf(range) || std::isnan(range))
            {
                ranges[ranges.size()] = 0.0;
            }
            else
            {
                ranges[ranges.size()] = range;
            }
        }
        return web::json::value::object({
            {"header", web::json::value(HeaderToJson(scan.header))},
            {"angle_min", web::json::value(scan.angle_min)},
            {"angle_max", web::json::value(scan.angle_max)},
            {"angle_increment", web::json::value(scan.angle_increment)},
            {"time_increment", web::json::value(scan.time_increment)},
            {"scan_time", web::json::value(scan.scan_time)},
            {"range_min", web::json::value(scan.range_min)},
            {"range_max", web::json::value(scan.range_max)},
            {"ranges", web::json::value(ranges)},
        });
    }

    web::json::value PathToJson(nav_msgs::Path path)
    {
        auto poses = web::json::value::array();
        for (auto pose_stamp : path.poses)
        {
            poses[poses.size()] = PoseToJson(pose_stamp);
        }
        return web::json::value::object({
            {"header", web::json::value(HeaderToJson(path.header))},
            {"poses", web::json::value(poses)},
        });
    }

    web::json::value PolygonToJson(geometry_msgs::Polygon polygon)
    {
        auto points = web::json::value::array();
        for (auto point : polygon.points)
        {
            points[points.size()] = Point32ToJson(point);
        }
        return web::json::value::object({
            {"points", web::json::value(points)}
        });
    }

    web::json::value SegmentedPolygonsToJson(std::vector<cti_msgs::PolygonArray> segmented_polygons)
    {
        auto segmented_polygons_json = web::json::value::array();

        for (auto segmented_polygon : segmented_polygons)
        {
            web::json::value outer_polygon;
            auto inner_polygons = web::json::value::array();
            for (int i=0; i<segmented_polygon.polygons.size(); i++)
            {
                if (i == 0)
                {
                    outer_polygon = PolygonToJson(segmented_polygon.polygons[0]);
                }
                else
                {
                    inner_polygons[inner_polygons.size()] = cti_robot::shared_tool::json::PolygonToJson(segmented_polygon.polygons[i]);
                }
            }
            segmented_polygons_json[segmented_polygons_json.size()] = web::json::value::object({
                {"outer_polygon", web::json::value(outer_polygon)},
                {"inner_polygons", web::json::value(inner_polygons)}
            });
        }

        return web::json::value::object({
            {"segmented_polygons", web::json::value(segmented_polygons_json)}
        }); 
    }
}}}

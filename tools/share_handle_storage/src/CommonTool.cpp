#include "share_handle_storage/CommonTool.hpp"
#include <ctime>
#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>
#include <tf/transform_listener.h>
#include <boost/filesystem.hpp>
#include <cmath>
#include <std_srvs/Empty.h>
#include "spdlog/spdlog.h"

using namespace std::literals;

namespace cti_robot{
namespace shared_tool{
namespace common{

    namespace time_s {

        std::string GetTimeString()
        {
            time_t rawtime;
            struct tm * timeinfo;
            char buffer [100];

            time(&rawtime);
            timeinfo = std::localtime(&rawtime);
            strftime(buffer, 99,"%Y_%m_%d_%H_%M_%S", timeinfo);
            std::string t(buffer);
            return t;
        }

        std::string GetCurrentTimeMinute()
        {
            time_t rawtime;
            struct tm * timeinfo;
            char buffer [100];

            time(&rawtime);
            timeinfo = localtime(&rawtime);
            // strftime(buffer, 99,"%Y_%m_%d_%H_%M_%S", timeinfo);
            strftime(buffer, 99,"%Y_%m_%d_%H_%M", timeinfo);
            std::string t(buffer);
            return t;
        }

        double CalcTime(ros::Time time)
        {
            return (ros::Time::now()-time).toSec();
        }

        std::string TimeStampToString(unsigned long timstamp)
        {
            time_t timeStamp = (time_t)timstamp;
            return asctime(localtime(&timeStamp));
        }

        long GetCurrentTimeStampBySecond()
        {
            return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        }

        long GetCurrentTimeStampByMilliSeconds()
        {
            return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        }

        std::string GetCurrentWeekNumber()
        {
            FILE *fstream = NULL;
            std::string cmd = "date +%w"s;
            char buff[64] = {0};
            if (NULL == (fstream = popen(cmd.c_str(), "r")))
            {
                SPDLOG_ERROR("Cannot Exec Date Cmd.");
                return "8";
            }
            else
            {
                if (EOF != fscanf(fstream, "%s", buff))
                {
                    pclose(fstream);
                    return buff;
                }
                else
                {
                    SPDLOG_ERROR("Cannot Get Return Value.");
                    return "9";
                }
            }
        }

        // formate: yyyy-mm-dd
        std::string GetCurrentDate()
        {
            FILE *fstream = NULL;
            std::string cmd = "date +%Y-%m-%d"s;
            char buff[64] = {0};
            if (NULL == (fstream = popen(cmd.c_str(), "r")))
            {
                SPDLOG_ERROR("Cannot Exec Date Cmd.");
                return "1970-00-00";
            }
            else
            {
                if (EOF != fscanf(fstream, "%s", buff))
                {
                    pclose(fstream);
                    return buff;
                }
                else
                {
                    SPDLOG_ERROR("Cannot Get Return Value.");
                    return "1970-00-00";
                }
            }
        }
    }

    bool IsNodeOnLine(const std::string & node_name)
    {
        FILE *fstream = NULL;
        std::string cmd = "rosnode ping -c 1 "s + node_name + " | grep --line-buffered \"time=\"| awk -F \"=\" '{print $2}'"s;
        char buff[1024] = {0};
        if (NULL == (fstream = popen(cmd.c_str(), "r")))
        {
            SPDLOG_ERROR("Cannot Ping Node [{}]; Ping Failed!.", node_name);
            return false;
        }
        else
        {
            if (EOF != fscanf(fstream, "%s", buff))
            {
                pclose(fstream);

                if (strstr(buff, "ms") != NULL)
                {
                    return true;
                }
                else
                {
                    SPDLOG_ERROR("Node [{}] Not Alive!.", node_name);
                    return false;
                }
            }
            else
            {
                SPDLOG_ERROR("Cannot Get Node [{}] Pint Time.", node_name);
                return false;
            }
        }
    }

    std::string GetThreadPid()
    {
        std::ostringstream oss;
        oss << std::this_thread::get_id();
        return oss.str();
    }

    bool SystemCmd(const std::string & cmd)
    {
        int ret = system(cmd.data());
        SPDLOG_INFO("systemCmd: cmd:{}, ret:{}", cmd, ret);
        return ret == 0;
    }

    bool Png2Pgm(const std::string png, const std::string pgm)
    {
        if (!boost::filesystem::exists(png))
        {
            SPDLOG_ERROR("Png2Pgm : file {} not exist", png);
            return false;
        }

        cv::Mat mat = cv::imread(png, cv::ImreadModes::IMREAD_GRAYSCALE);
        if (mat.empty())
        {
            SPDLOG_ERROR("Png2Pgm : imread file {} error", png);
            return false;
        }

        std::vector<int> param = {CV_IMWRITE_PXM_BINARY, 1};

        if(cv::imwrite(pgm, mat, param))
        {
            return true;
        }
        else
        {
            SPDLOG_ERROR("Png2Pgm : imwrite file {} error", pgm);
            return false;
        }
    }

    bool Pgm2Png(const std::string pgm, const std::string png)
    {
        if (!boost::filesystem::exists(pgm))
        {
            SPDLOG_ERROR("Pgm2Png : file {} not exist", pgm);
            return false;
        }

        cv::Mat mat = cv::imread(pgm, cv::ImreadModes::IMREAD_GRAYSCALE);
        if (mat.empty())
        {
            SPDLOG_ERROR("Pgm2Png : imread file {} error", pgm);
            return false;
        }

        if (boost::filesystem::exists(png))
        {
            boost::filesystem::remove(png);
        }

        std::vector<int> param = {CV_IMWRITE_PNG_COMPRESSION, 9};

        if(cv::imwrite(png, mat, param))
        {
            return true;
        }
        else
        {
            SPDLOG_ERROR("Pgm2Png : imwrite file {} error", png);
            return false;
        }
    }

    bool GetCurrentPosition(geometry_msgs::Pose & pose)
    {
        tf::StampedTransform transform;
        tf::TransformListener listener;

        try{
            listener.waitForTransform("map", "base_link", ros::Time(0), ros::Duration(1.0));
            listener.lookupTransform("map", "base_link", ros::Time(0), transform);
            pose.position.x = transform.getOrigin().x();
            pose.position.y = transform.getOrigin().y();
            pose.position.z = transform.getOrigin().z();
            pose.orientation.x = transform.getRotation().getX();
            pose.orientation.y = transform.getRotation().getY();
            pose.orientation.z = transform.getRotation().getZ();
            pose.orientation.w = transform.getRotation().getW();
            return true;
        }
        catch (tf::TransformException &ex) {
            // ROS_ERROR("%s", ex.what());
            SPDLOG_ERROR("Get Current Position: {}", ex.what());
        }
        return false;
    }
    
    bool IsNanOrInf(double value)
    {
        return std::isnan(value) || std::isinf(value);
    }

    geometry_msgs::Pose FilterNanAndInf(geometry_msgs::Pose pose)
    {
        if (IsNanOrInf(pose.position.x)) pose.position.x = 0.0;
        if (IsNanOrInf(pose.position.y)) pose.position.y = 0.0;
        if (IsNanOrInf(pose.position.z)) pose.position.z = 0.0;
        if (IsNanOrInf(pose.orientation.x)) pose.orientation.x = 0.0;
        if (IsNanOrInf(pose.orientation.y)) pose.orientation.y = 0.0;
        if (IsNanOrInf(pose.orientation.z)) pose.orientation.z = 0.0;
        if (IsNanOrInf(pose.orientation.w)) pose.orientation.w = 0.0;
        return pose;
    }

    bool checkPoseValid(geometry_msgs::Pose pose)
    {
        if (pose.orientation.z == 0.0 && pose.orientation.w == 0.0)
        {
            return false;
        }

        double num = sqrt(pow(pose.orientation.z, 2) + pow(pose.orientation.w, 2));
        if (num > 1.05 || num < 0.95)
        {
            return false;
        }

        if (pose.position.x == 0.0 && pose.position.y == 0.0)
        {
            return false;
        }

        return true;
    }

    geometry_msgs::Pose getPoseShifting(geometry_msgs::Pose pose, tf::Vector3 v3)
    {
        tf::Transform transform;
        transform.setIdentity();

        tf::Quaternion q(pose.orientation.x,
                        pose.orientation.y,
                        pose.orientation.z,
                        pose.orientation.w);
        transform.setRotation(q);
        tf::Vector3 translation_robot_frame = v3;
        tf::Vector3 delta_translation = transform*translation_robot_frame;
        pose.position.x += delta_translation.getX();
        pose.position.y += delta_translation.getY();
        pose.position.z += delta_translation.getZ();
        return pose;
    }

    geometry_msgs::Pose getTurnPosition(geometry_msgs::Pose pose, double angle)
    {
        geometry_msgs::Pose ret_pose = pose;
        tf::Quaternion tmp_qua;
        tf::quaternionMsgToTF(pose.orientation, tmp_qua);
        double yaw = tf::getYaw(tmp_qua) + angle;
        tf::quaternionTFToMsg(tf::createQuaternionFromYaw(yaw), ret_pose.orientation);
        return ret_pose;
    }

    tf::Transform GetCurrentTransform()
    {
        tf::StampedTransform transform;

        tf::TransformListener listener;

        try{
            listener.waitForTransform("map", "base_link",ros::Time(0), ros::Duration(1.0));
            listener.lookupTransform("map", "base_link", ros::Time(0), transform);
        }
        catch (tf::TransformException &ex) {
            // ROS_ERROR("%s", ex.what());
            SPDLOG_ERROR("Get Current Transform: {}",ex.what());
        }

        return transform;
    }

    bool isPoseValid(geometry_msgs::Pose pose)
    {
        return isOrientationValid(pose.orientation.x,
                                    pose.orientation.y,
                                    pose.orientation.z,
                                    pose.orientation.w);
    }

    bool isTFValid(tf::Transform tf)
    {
        return isOrientationValid(tf.getRotation().getX(), 
                                    tf.getRotation().getY(), 
                                    tf.getRotation().getZ(), 
                                    tf.getRotation().getW());
    }

    bool isOrientationValid(double x, double y, double z, double w)
    {
        if (std::isfinite(x) && std::isfinite(y) && std::isfinite(z) && std::isfinite(w))
        {
            if ((-0.01 < x && x < 0.01) && (-0.01 < y && y < 0.01))
            {
                double sum = sqrt(pow(z, 2) + pow(w, 2));
                if (0.95 < sum && sum < 1.05)
                {
                    return true;
                }
                else
                {
                    SPDLOG_ERROR("z and w is too big.");
                }
            }
            else
            {
                SPDLOG_ERROR("x or y is over.");
            }
        }
        else
        {
            SPDLOG_ERROR("someone isn't a num.");
        }
        
        SPDLOG_ERROR("orientation isn't valid: {} {} {} {}", x, y, z, w);
        return false;
    }

    double disBetweenTwoPoses(geometry_msgs::Pose pose_1, geometry_msgs::Pose pose_2)
    {
        return std::sqrt(std::pow(pose_1.position.x-pose_2.position.x, 2) + std::pow(pose_1.position.y-pose_2.position.y, 2));
    }

    double disBetweenTwoPoses(geometry_msgs::PoseStamped pose_1, geometry_msgs::PoseStamped pose_2)
    {
        return std::sqrt(std::pow(pose_1.pose.position.x-pose_2.pose.position.x, 2) + std::pow(pose_1.pose.position.y-pose_2.pose.position.y, 2));
    }

    bool CreateFileAndSync(std::string file_path)
    {
        int fd = open(file_path.c_str(), O_RDWR | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP);
        if (fd != -1)
        {
            if (fsync(fd) < 0)
            {
                return false;
            }
            close(fd);
            return true;
        }
        else
        {
            return false;
        }
    }

    bool CreateFileAndWriteAndSync(std::string file_path, std::string context)
    {
        return CreateFileAndWriteAndSync(file_path, context.c_str(), context.size());
    }

    bool CreateFileAndWriteAndSync(std::string file_path, const char * context, size_t size)
    {
        int fd = open(file_path.c_str(), O_RDWR | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP);
        if (fd != -1)
        {
            if (write(fd, context, size) < 0 || fsync(fd) < 0)
            {
                return false;
            }
            close(fd);
            return true;
        }
        else
        {
            return false;
        }
    }

    void AddContextToFile(std::string file, std::vector<unsigned char> payload)
    {
        std::ofstream f(file, std::ios_base::out | std::ios_base::binary | std::ios_base::app);
        f.write(reinterpret_cast<char*>(payload.data()), payload.size());
        f.close();
    }

    bool CheckCrashReboot()
    {
        ros::NodeHandle n;
        ros::ServiceClient ntp_service = n.serviceClient<std_srvs::Empty>("/is_ntp_over");
        std_srvs::Empty srv;
        if (ntp_service.call(srv))
        {
            //ntp 已同步, 获取启动时间.
            double start_time = ros::Time::now().toSec();
            n.getParam("/system_monitor/service_start_time", start_time);

            if (std::fabs(ros::Time::now().toSec() - start_time) > 30)
            {
                SPDLOG_INFO("CheckCrashReboot: 离ntp同步后时间大于30秒, 程序崩溃后重启.");
                return true;
            }
            else
            {
                SPDLOG_INFO("CheckCrashReboot: 离ntp同步后时间小于30秒.");
                return false;
            }
        }
        else
        {
            SPDLOG_INFO("CheckCrashReboot: ntp 尚未同步, 服务刚启动.");
            return false;
        }
    }

    bool ReadFile(std::string file_path, std::string & data)
    {
        auto result = boost::filesystem::exists(file_path);
        if (result)
        {
            data = ReadFile(file_path);
            // auto stream = std::ifstream(file_path, std::ios::in | std::ios::binary | std::ios::ate);
            // data = slurp(stream);
        }
        return result;
    }

    std::string ReadFile(std::string path)
    {
        constexpr auto read_size = std::size_t{4096};
        auto stream = std::ifstream{path.data()};
        stream.exceptions(std::ios_base::badbit);

        auto out = std::string{};
        auto buf = std::string(read_size, '\0');
        while (stream.read(& buf[0], read_size)) {
            out.append(buf, 0, stream.gcount());
        }
        out.append(buf, 0, stream.gcount());
        return out;
    }
	
    double CalculationArea(std::vector<geometry_msgs::Point> points)
    {
        double area = 0.0;

        for (int i=0; i<points.size(); i++)
        {
            int j = (i+1)%(points.size());

            area += points[i].x * points[j].y - points[i].y * points[j].x;
        }

        area /= 2;
        return std::fabs(area);
    }

    // double CalculationArea(cti_msgs::CtiPointArray zone)
    // {
    //     return CalculationArea(zone.points);
    // }

    // bool IsPoseInZone(geometry_msgs::Point point, cti_msgs::CtiPointArray zone)
    // {
    //     // SPDLOG_INFO("[IsPoseInZone] point: [{} {}]", point.x, point.y);

    //     // for (auto p : zone.points)
    //     // {
    //     //     SPDLOG_INFO("[IsPoseInZone] zone p: [{} {}]", p.x, p.y);
    //     // }

    //     double zone_area = CalculationArea(zone);

    //     double total_area = 0.0;

    //     for (int i=0; i<zone.points.size(); i++)
    //     {
    //         int j = (i+1)%(zone.points.size());

    //         cti_msgs::CtiPointArray tmp_zone;
    //         {
    //             tmp_zone.points.push_back(zone.points[i]);
    //             tmp_zone.points.push_back(zone.points[j]);
                
    //             // geometry_msgs::Point tmp_point;
    //             // {
    //             //     tmp_point.x = point.x;
    //             //     tmp_point.y = point.y;
    //             //     tmp_point.z = point.z;
    //             // }
    //             tmp_zone.points.push_back(point);
    //         }

    //         double tmp_area = CalculationArea(tmp_zone);

    //         // SPDLOG_INFO("[IsPoseInZone] area: {}", tmp_area);

    //         total_area += tmp_area;
    //     }

    //     // SPDLOG_INFO("[IsPoseInZone] zone_area:{}, total_area:{}", zone_area, total_area);

    //     return std::fabs(zone_area-total_area) < 0.1;
    // }

    // geometry_msgs::Polygon PointArrayToPolygon(cti_msgs::CtiPointArray zone)
    // {
    //     geometry_msgs::Polygon polygon;
    //     for (auto point : zone.points)
    //     {
    //         polygon.points.push_back(PointToPoint<geometry_msgs::Point, geometry_msgs::Point32>(point));
    //     }
    //     return polygon;
    // }

    // cti_msgs::CtiPointArray FindRectangle(geometry_msgs::Pose center_pose, double width, double length)
    // {
    //     cti_msgs::CtiPointArray rectangle;

    //     rectangle.points.push_back(PoseToPoint<geometry_msgs::Point>(getPoseShifting(center_pose, tf::Vector3(length/2, width/2, 0.0))));
    //     rectangle.points.push_back(PoseToPoint<geometry_msgs::Point>(getPoseShifting(center_pose, tf::Vector3(length/2, 0-width/2, 0.0))));
    //     rectangle.points.push_back(PoseToPoint<geometry_msgs::Point>(getPoseShifting(center_pose, tf::Vector3(0-length/2, 0-width/2, 0.0))));
    //     rectangle.points.push_back(PoseToPoint<geometry_msgs::Point>(getPoseShifting(center_pose, tf::Vector3(0-length/2, width/2, 0.0))));

    //     return rectangle;
    // }

    // cti_msgs::CtiPointArray CalcMapMaxPoint(cti_msgs::CloudMapMap map)
    // {
    //     cti_msgs::CtiPointArray array;

    //     array.points.push_back(map.origin);

    //     geometry_msgs::Point point_up, point_right, point_up_right;

    //     point_right.x = map.origin.x + map.resolution * map.image_width;
    //     point_right.y = map.origin.y;
    //     point_right.z = map.origin.z;

    //     point_up.x = map.origin.x;
    //     point_up.y = map.origin.y + map.resolution * map.image_height;
    //     point_up.z = map.origin.z;

    //     point_up_right.x = map.origin.x + map.resolution * map.image_width;
    //     point_up_right.y = map.origin.y + map.resolution * map.image_height;
    //     point_up_right.z = map.origin.z;

    //     array.points.push_back(point_up);
    //     array.points.push_back(point_up_right);
    //     array.points.push_back(point_right);

    //     return array;
    // }

    long GetRuntime()
    {
        std::string file = std::string(std::getenv("HOME")) + "/.ros/system_monitor/total_run_time"s;

        if (not boost::filesystem::exists(file))
        {
            SPDLOG_INFO("[GetRuntime] run_time文件不存在, 默认为0");
        }
        else
        {
            std::string data;
            if (cti_robot::shared_tool::common::ReadFile(file, data))
            {
                if (data.empty())
                {
                    SPDLOG_INFO("[GetRuntime] 文件内容是空的");
                }
                else
                {
                    try
                    {
                        return std::stol(data);
                    }
                    catch(std::invalid_argument const& ex)
                    {
                        SPDLOG_INFO("[GetRuntime] 字符串转long异常: {}", ex.what());
                    }
                    catch(std::out_of_range const& ex)
                    {
                        SPDLOG_INFO("[GetRuntime] 字符串转long超过边界: {}", ex.what());
                    }
                }
            }
            else
            {
                SPDLOG_INFO("[GetRuntime] 读文件异常.");
            }
        }
        return 0;
    }

    void SetRuntime(long run_time)
    {
        std::string file = std::string(std::getenv("HOME")) + "/.ros/system_monitor/total_run_time"s;
        CreateFileAndWriteAndSync(file, std::to_string(run_time));
    }

    double CalcAngle(double x1, double y1,
                     double x2, double y2,
                     double x3, double y3)
    {
        double theta = atan2(x1-x3, y1-y3) - atan2(x2-x3, y2-y3);
        if (theta > M_PI)
        {
            theta -= 2* M_PI;
        }
        else if (theta < -M_PI)
        {
            theta += 2* M_PI;
        }

        return std::abs(theta * 180.0 / M_PI);
    }

    bool ParserJsonFromFile(std::string file,
                            web::json::value & json)
    {
        if (not boost::filesystem::exists(file))
        {
            return false;
        }
        else
        {
            try
            {
                std::ifstream fs(file.c_str(), std::ios::in);
                json = web::json::value::parse(fs);
                fs.close();
                return true;
            }
            catch(const std::exception& e)
            {
                return false;
            }
        }
    }
    
    bool mapSavePGMPicture(const nav_msgs::OccupancyGrid &map,
                           std::string mapname, 
                           int threshold_occupied, 
                           int threshold_free)
    {
        SPDLOG_INFO("Map Save to {}", mapname);
        FILE *out = fopen(mapname.c_str(), "w");
        if (!out)
        {
            SPDLOG_ERROR("Couldn't save map file to {}", mapname);
            return false;
        }

        fprintf(out, "P5\n# CREATOR: map_saver.cpp %.3f m/pix\n%d %d\n255\n",
                map.info.resolution, map.info.width, map.info.height);
        for (unsigned int y = 0; y < map.info.height; y++)
        {
            for (unsigned int x = 0; x < map.info.width; x++)
            {
                unsigned int i = x + (map.info.height - y - 1) * map.info.width;
                if (map.data[i] >= 0 && map.data[i] <= threshold_free)
                { // occ [0,0.1)
                    fputc(254, out);
                }
                else if (map.data[i] <= 100 && map.data[i] >= threshold_occupied)
                { // occ (0.65,1]
                    fputc(000, out);
                }
                else
                { // occ [0.1,0.65]
                    fputc(205, out);
                }
            }
        }

        fclose(out);

        SPDLOG_INFO("Map Save PGM Picture Done : {}", mapname);
        return true;
    }
}}}

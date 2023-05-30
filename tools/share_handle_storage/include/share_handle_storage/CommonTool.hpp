#ifndef _CTI_ROBOT_SHARED_TOOL_COMMON_TOOL_HPP_
#define _CTI_ROBOT_SHARED_TOOL_COMMON_TOOL_HPP_

#include "ros/ros.h"
#include <tf/tf.h>
#include <string>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Polygon.h>
// #include "cti_msgs/CtiPointArray.h"
// #include "cti_msgs/CloudMapMap.h"
#include "cpprest/json.h"
#include <nav_msgs/OccupancyGrid.h>

namespace cti_robot{
namespace shared_tool{
namespace common{

    namespace time_s {

        std::string GetTimeString();

        std::string GetCurrentTimeMinute();

        double CalcTime(ros::Time time);

        std::string TimeStampToString(unsigned long timstamp);

        long GetCurrentTimeStampBySecond();

        long GetCurrentTimeStampByMilliSeconds();

        std::string GetCurrentWeekNumber();

        std::string GetCurrentDate();
    }

    bool IsNodeOnLine(const std::string & node_name);

    std::string GetThreadPid();

    bool SystemCmd(const std::string & cmd);

    bool Png2Pgm(const std::string png, const std::string pgm);

    bool Pgm2Png(const std::string pgm, const std::string png);

    bool GetCurrentPosition(geometry_msgs::Pose & pose);

    bool IsNanOrInf(double value);

    geometry_msgs::Pose FilterNanAndInf(geometry_msgs::Pose pose);

    bool checkPoseValid(geometry_msgs::Pose pose);

    geometry_msgs::Pose getPoseShifting(geometry_msgs::Pose pose, tf::Vector3 v3);

    geometry_msgs::Pose getTurnPosition(geometry_msgs::Pose pose, double angle);

    bool isOrientationValid(double x, double y, double z, double w);
              
    bool isPoseValid(geometry_msgs::Pose pose);

    tf::Transform GetCurrentTransform();

    bool isTFValid(tf::Transform tf);

    double disBetweenTwoPoses(geometry_msgs::Pose pose_1, geometry_msgs::Pose pose_2);

    double disBetweenTwoPoses(geometry_msgs::PoseStamped pose_1, geometry_msgs::PoseStamped pose_2);

    bool CreateFileAndSync(std::string file_path);

    bool CreateFileAndWriteAndSync(std::string file_path, std::string context);

    bool CreateFileAndWriteAndSync(std::string file_path, const char * context, size_t size);

    void AddContextToFile(std::string file, std::vector<unsigned char> payload);

    bool CheckCrashReboot();

    bool ReadFile(std::string file_path, std::string & data);

    std::string ReadFile(std::string path);

    double CalculationArea(std::vector<geometry_msgs::Point> points);

    // double CalculationArea(cti_msgs::CtiPointArray zone);

    // bool IsPoseInZone(geometry_msgs::Point pose, cti_msgs::CtiPointArray zone);

    template <class T>
    T PoseToPoint(geometry_msgs::Pose pose)
    {
        T t;
        t.x = pose.position.x;
        t.y = pose.position.y;
        t.z = pose.position.z;
        return t;
    }

    template <class U, class T>
    T PointToPoint(U u)
    {
        T t;
        t.x = u.x;
        t.y = u.y;
        t.z = u.z;
        return t;
    }

    // geometry_msgs::Polygon PointArrayToPolygon(cti_msgs::CtiPointArray zone);

    // cti_msgs::CtiPointArray FindRectangle(geometry_msgs::Pose center_pose, double width, double length);

    // cti_msgs::CtiPointArray CalcMapMaxPoint(cti_msgs::CloudMapMap map);

    long GetRuntime();

    void SetRuntime(long run_time);

    double CalcAngle(double x1, double y1,
                     double x2, double y2,
                     double x3, double y3);
    
    bool ParserJsonFromFile(std::string file,
                            web::json::value & json);

    bool mapSavePGMPicture(const nav_msgs::OccupancyGrid &map,
                           std::string mapname, 
                           int threshold_occupied, 
                           int threshold_free);
}}}

#endif //_CTI_ROBOT_SHARED_TOOL_COMMON_TOOL_HPP_
#include "share_handle_storage/BoostTool.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include <boost/geometry.hpp>
#include <boost/geometry/algorithms/area.hpp>
#include <boost/geometry/algorithms/assign.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>

#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>

#include <boost/filesystem.hpp>

#include <boost/date_time/gregorian/gregorian.hpp> 

#include <geometry_msgs/Polygon.h>

#include <iostream>
#include <sstream>

#include "spdlog/spdlog.h"

namespace cti_robot{
namespace shared_tool{
namespace boost_tool{

    bool Base64Encode(const std::string& input, std::string* output) 
    {
        typedef boost::archive::iterators::base64_from_binary
                    <boost::archive::iterators::transform_width
                        <std::string::const_iterator, 6, 8>> Base64EncodeIterator;

        std::stringstream result;
        std::copy(Base64EncodeIterator(input.begin()), 
                  Base64EncodeIterator(input.end()), 
                  std::ostream_iterator<char>(result));

        size_t equal_count = (3 - input.length() % 3) % 3;
        for (size_t i = 0; i <equal_count; i++) {
            result.put('=');
        }

        *output = result.str();
        return output->empty() == false;
    }
    
    bool Base64Decode(const std::string& input, std::string* output) 
    {
        typedef boost::archive::iterators::transform_width
                        <boost::archive::iterators::binary_from_base64
                            <std::string::const_iterator>, 8, 6> Base64DecodeIterator;
        std::stringstream result;
        try {
            std::copy(Base64DecodeIterator(input.begin()), 
                      Base64DecodeIterator(input.end()), 
                      std::ostream_iterator<char>(result));
        } catch(...) {
            return false;
        }
        *output = result.str();
        return output->empty() == false;
    }

    std::string CreateRandomID()
    {
        boost::uuids::uuid a_uuid = boost::uuids::random_generator()();
        const std::string tmp_uuid = boost::uuids::to_string(a_uuid);
        return tmp_uuid;
    }

    double CalculationArea(geometry_msgs::Polygon zone)
    {
        try {
            if (zone.points.empty())
            {
                SPDLOG_ERROR("区域的定点数量为空.");
                return 0.0;
            }
            std::vector<boost::geometry::model::d2::point_xy<double>> points;
            for (auto point : zone.points)
            {
                points.push_back(boost::geometry::model::d2::point_xy<double>(point.x, point.y));
            }
            //点需要闭环:例如 ((0, 0), (0, 5), (5, 5), (5, 0), (0, 0))
            points.push_back(boost::geometry::model::d2::point_xy<double>(zone.points[0].x, zone.points[0].y));
            boost::geometry::model::polygon<boost::geometry::model::d2::point_xy<double>> polygon;
            boost::geometry::assign_points(polygon, points);
            return std::fabs(boost::geometry::area(polygon));
        }
        catch (const std::exception ex)
        {
            SPDLOG_ERROR("计算面积失败");
            return 0.0;
        }
    }

    double CalculationPathArea(geometry_msgs::Polygon path)
    {
        if (path.points.size() <= 1)
        {
            SPDLOG_ERROR("计算面积失败:路线长度非法");
            return 0.0;
        }

        double length = 0.0;
        for (int i=1; i<path.points.size(); i++)
        {
            auto pose_org = path.points[i-1];
            auto pose_des = path.points[i];
            length += std::sqrt(std::pow((pose_org.x-pose_des.x), 2) + std::pow((pose_org.y-pose_des.y), 2));
        }
        return length * 0.5;
    }

    bool CreateDir(const std::string & dir)
    {
        try
        {
            boost::filesystem::path path(dir);
            if (not boost::filesystem::exists(path))
            {
                boost::filesystem::create_directories(path);
            }
        }
        catch(boost::filesystem::filesystem_error & e)
        {
            SPDLOG_ERROR("Create dir {} fail :{}", dir, e.what());
            return false;
        }

        return true;
    }

    bool isFileExist(std::string file)
    {
        return boost::filesystem::exists(file);
    }

    std::string isFileExistWithNamePrefix(std::string file_name_prefix, std::string dir)
    {
        try
        {
            namespace bf = boost::filesystem;

            if (bf::exists(dir))
            {
                bf::directory_iterator end_it;
                for (bf::directory_iterator it(dir); it != end_it; it++)
                {
                    if (not bf::is_directory(it->status()))
                    {
                        if (std::string::npos != it->path().stem().string().find(file_name_prefix))
                        {
                            return it->path().string();
                        }
                    }
                }
            }
            else
            {
                SPDLOG_ERROR("[{}]文件夹不存在", dir);
            }
        }
        catch (std::exception & e)
        {
            SPDLOG_ERROR("[{}]捕获错误:{}", dir, e.what());
        }
        return std::string();
    }

    unsigned long FileSize(std::string file)
    {
        return boost::filesystem::file_size(file);
    }

    void copyFile(std::string file_from, std::string file_to)
    {
        try
        {
            SPDLOG_INFO("MapManager: copyFile: {} {}", file_from, file_to);
            boost::filesystem::path from(file_from);
            boost::filesystem::path to(file_to);
            boost::filesystem::copy_file(from, to);
        }
        catch(boost::filesystem::filesystem_error & e)
        {
            SPDLOG_ERROR("MapManager: copyFile error :{}", e.what());
        }
        
        return;
    }

    void copyFileAndDeleteOldFile(std::string file_from, std::string file_to)
    {
        if (isFileExist(file_to))
        {
            rmFile(file_to);
        }
        copyFile(file_from, file_to);
        return;
    }

    void rmFile(std::string file)
    {
        SPDLOG_INFO("MapManager: rmFile: {}", file);
        try
        {
            boost::filesystem::path f(file);
            boost::filesystem::remove_all(f);
        }
        catch(boost::filesystem::filesystem_error & e)
        {
            SPDLOG_ERROR("MapManager: rmFile error :{}", e.what());
        }
        return;
    }

    //1==Monday, 7==Sunday, 
    unsigned int GetWeekOfToday()
    {
        // 0==Sunday, 1==Monday, etc
        auto week = boost::gregorian::day_clock::local_day().day_of_week();
        if (week == 0)
        {
            week = 7;
        }
        return week;
    }
}}}

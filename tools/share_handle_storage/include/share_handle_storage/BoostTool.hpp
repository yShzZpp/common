#ifndef _CTI_ROBOT_SHARED_TOOL_BOOST_TOOL_HPP_
#define _CTI_ROBOT_SHARED_TOOL_BOOST_TOOL_HPP_

#include <string>
#include <geometry_msgs/Polygon.h>

namespace cti_robot{
namespace shared_tool{
namespace boost_tool{

    bool Base64Encode(const std::string& input, std::string* output);

    bool Base64Decode(const std::string& input, std::string* output);

    std::string CreateRandomID();

    double CalculationArea(geometry_msgs::Polygon zone);

    double CalculationPathArea(geometry_msgs::Polygon path);

    bool CreateDir(const std::string & dir);

    bool isFileExist(std::string file);

    std::string isFileExistWithNamePrefix(std::string file_name_prefix, std::string dir);

    unsigned long FileSize(std::string file);

    void copyFile(std::string file_from, std::string file_to);

    void copyFileAndDeleteOldFile(std::string file_from, std::string file_to);

    void rmFile(std::string file);

    /*
    
    */
    unsigned int GetWeekOfToday();
    
}}}

#endif //_CTI_ROBOT_SHARED_TOOL_BOOST_TOOL_HPP_
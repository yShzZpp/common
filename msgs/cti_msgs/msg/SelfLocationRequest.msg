uint8 START_LOCATION = 0
uint8 STOP_LOCATION = 1
uint8 cmd

string SCENE_ROBOT_START_UP = ROBOT_START_UP
string SCENE_APP_CMD = APP_CMD
string SCENE_LOCATION_LOST = ROBOT_LOCATION_LOST
string scene

# 使用场景:
#   check_vision_whether_robot_restart_in_no_movement
# 定义:
#   重启前机器所在大楼
string building

# 使用场景:
#   check_vision_whether_robot_restart_in_no_movement
# 定义: 
#   重启前机器所在楼层
string floor   

# 使用场景:
#   check_laserdata_whether_robot_restart_in_no_movement
# 定义: 
#   重启前机器的位置
geometry_msgs/Pose pose

# 使用场景:
#   check_laserdata_whether_robot_restart_in_no_movement
# 定义: 
#   重启前机器所使用的地图数据
nav_msgs/OccupancyGrid map

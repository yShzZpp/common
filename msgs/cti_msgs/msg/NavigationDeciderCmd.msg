# goal（根据全局规划模式选填，清洁和消毒模式不需要）
geometry_msgs/PoseStamped goal

# global_path（下发的清洁路径列表）
nav_msgs/Path path

# goal tolerance 
float32 xy_goal_tolerance
float32 yaw_goal_tolerance

# （navi_planning_frequency建议为1）
float32 navi_planning_frequency 
float32 navi_planning_patience 

float32 navi_controlling_patience 

#global planner mode(目前支持1,2,4,5,6)
uint8 GP_MODE_NONE = 0          # 无需全局规划
uint8 GP_MODE_ASTAR = 1         # A*
uint8 GP_MODE_DIJKSTRA = 2      # Dijkstra
uint8 GP_MODE_RRT = 3           # RRT算法
uint8 GP_MODE_CLEAN = 4         # 清扫
uint8 GP_MODE_ROADNET = 5       # 主干道
uint8 GP_MODE_DISINFECTION = 6  # 消毒
uint8 GP_MODE_TOPO = 7          # topo (离线生成)
uint8 GP_MODE_CLEAN_SMOOTH = 8         # 清扫跟踪路径
uint8 navi_global_planner_mode

#local planner mode(目前支持1,2)（首选TEB）
uint8 LP_MODE_NONE = 0       # 无需局部规划 
uint8 LP_MODE_DWA = 1        # dwa算法   #支持 use_local_path_control
uint8 LP_MODE_TEB = 2        # teb算法   #不支持 use_local_path_control
uint8 LP_MODE_FTC = 3        # ftc算法   #支持 use_local_path_control
uint8 LP_MODE_FRE = 4        # frenet    #不支持 use_local_path_control
uint8 navi_local_planner_mode

# local path control mode(目前没用)
bool use_local_path_control

#恢复措施
#
##特殊场景1: 禁止恢复动作
uint8 ACTION_BEHAVIORS_NONE = 1
#
##特殊场景2: 进电梯,当机器全部进到电梯时           
uint8 ACTION_BEHAVIORS_ENTER_LIFT = 2
#
##特殊场景3: 根据机器离目标点的距离
uint8 ACTION_BEHAVIORS_NEAR_NAVIGATION = 3
#
##特殊场景4: 在电梯里，往前挤
uint8 ACTION_BEHAVIORS_TO_LIFT = 4
#
##特殊场景5: 过闸机过程
uint8 ACTION_BEHAVIORS_CROSS_GATE = 5
#
##特殊场景6: 执行所有恢复动作
uint8 ACTION_BEHAVIORS_ALL = 6

uint8 action_behaviors_mode 

# How many times try to go to one point(目前没用)
uint8 tries_times

# sensor enable/disable (目前没用)
# uint8 sensor_type 

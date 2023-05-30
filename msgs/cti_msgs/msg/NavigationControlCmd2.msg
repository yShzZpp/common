
# cmd 列表
uint8 CMD_EXE_TASK = 0 #执行任务
uint8 CMD_CANCEL_TASK = 1 #取消任务
#临时方案:下发取消导航命令
uint8 CMD_CANCEL_STRAIGHT_TASK = 2 #取消命令:取消进入电梯后,不断前进的任务
# 发送给navigation control模块的控制命令
uint8 cmd

#########################################################################
# 以下有点场景相关的
#########################################################################
# 最基础默认场景的 defalut 导航方式
int32 NAVIGATION_SCENARIO_DEFAULT = 10000

# 常规场景导航方式
int32 NAVIGATION_SCENARIO_NORMAL = 11000

# 躲避场景 导航方式
int32 NAVIGATION_SCENARIO_DODGE = 12000

# 人群拥挤场景 导航方式
int32 NAVIGATION_SCENARIO_CROWD = 13000

# 过闸机场景 导航方式
int32 NAVIGATION_SCENARIO_CROSSING_GATE = 14000

# 进电梯场景使用的 导航方式
int32 NAVIGATION_SCENARIO_ENTER_ELEVATOR = 15000

# 挤电梯场景使用的 导航方式
int32 NAVIGATION_SCENARIO_CROWD_ELEVATOR = 16000

# 出电梯场景使用 导航方式
int32 NAVIGATION_SCENARIO_LEAVE_ELEVATOR = 17000

# 使用主干道的 导航方式
# int32 NAVIGATION_TRUNK_ROAD = 18000

# 接近目标点场景的 导航方式
int32 NAVIGATION_SCENARIO_NEAR_GOAL = 19000

# 在急转弯处场景的 导航方式
int32 NAVIGATION_SCENARIO_SHARP_TURN = 20000			

# 需要原地掉头场景的 导航方式
int32 NAVIGATION_SCENARIO_TURN_AROUND = 21000		

# ---------------
# 以navfn全局规划路径为主的 路径规划方式
int32 NAVIGATION_PLANNER_NAVFN = 22000		

# 以人工手动绘制轨迹规划路径为主的 路径规划方式
int32 NAVIGATION_PLANNER_HUMAN_PATH = 23000				

# 使用主干道路径的 路径规划方式
int32 NAVIGATION_PLANNER_TRUNK_ROAD = 24000

# 以topo图全局规划路径为主的 路径规划方式
int32 NAVIGATION_PLANNER_TOPO_GRAPH = 25000			

# ---------------
# 以dwa局部规划为主的 控制方式
int32 NAVIGATION_CONTROLLER_DWA = 26000

# 以teb局部规划为主的 控制方式
int32 NAVIGATION_CONTROLLER_TEB = 27000

# 以FTC局部规划为主的 控制方式
int32 NAVIGATION_CONTROLLER_FTC = 28000

# 以寻迹局部规划控制为主的 控制方式
int32 NAVIGATION_CONTROLLER_TRACKING = 29000

# 以Waypoint follower跟随控制为主的 控制方式
int32 NAVIGATION_CONTROLLER_WAYPOINT_FOLLOWER = 30000

# 以pure pursuit跟随控制为主的 控制方式
int32 NAVIGATION_CONTROLLER_PURE_PURSUIT = 31000

# 进入Recovery multiple
int32 NAVIGATION_RECOVERY_MULTIPLE = 32000

int32 navi_mode     # 主场景
int32 navi_mode_sub # 在主场景下 还有次场景(细微区别)


bool ignore_pose       # true: 忽略该次的pose
geometry_msgs/Pose pose #目标点

float64 delay_time #命令延迟下发时间

int8 tries_times #失败重试次数 -1为无限次

uint8 CMD_SOURCE_LEVEL_NAVIGATION = 0 #平层导航的命令
uint8 CMD_SOURCE_CROSS_GATE = 1       #越过闸机的命令
uint8 CMD_SOURCE_CROSS_DOOR = 2       #越过安全门的命令
uint8 CMD_SOURCE_ENTER_ELEVATOR = 3   #进电梯模块的命令
uint8 CMD_SOURCE_LEAVE_ELEVATOR = 4   #出电梯模块的命令
uint8 CMD_SOURCE_DODGE = 5            #躲避其它车辆命令
uint8 cmd_source #命令源

bool play_audio


# UInt32 - [0 : 4294967295]
# mode:gp:lp:recovery:param:
# mode
# uint32 NAVIGATION_MODE_BIT = 4
uint32 NAVIGATION_MODE_DEFAULT = 1
uint32 NAVIGATION_MODE_DEFAULT2 = 2

# gp
# uint32 NAVIGATION_PLANNER_BIT = 3
uint32 NAVIGATION_PLANNER_NAVFN = 1
uint32 NAVIGATION_PLANNER_TOPO_GRAPH = 2
uint32 NAVIGATION_PLANNER_CLEAN_PATH = 3

# lp
# uint32 NAVIGATION_CONTROLLER_BIT = 2
uint32 NAVIGATION_CONTROLLER_DWA = 1
uint32 NAVIGATION_CONTROLLER_TEB = 2
uint32 NAVIGATION_CONTROLLER_CDWA = 3

# recovery
uint32 NAVIGATION_RECOVERY_BIT = 1
uint32 NAVIGATION_RECOVERY_MULTIPLE = 10

uint32 navi_mode
#######################################################
bool update_pose # true 更新pose     ,没有此标志无法知道是否需要更新内部备份的pose
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

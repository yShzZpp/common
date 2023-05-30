
# cmd 列表
uint8 CMD_EXE_TASK = 1 #执行任务
uint8 CMD_CANCEL_TASK = 0 #取消任务

uint8 cmd

uint8 NAVIGATION_DEFAULT = 1
uint8 NAVIGATION_CROSSING_GATE = 2
uint8 NAVIGATION_ENTER_ELEVATOR = 3
uint8 NAVIGATION_CROWD_ELEVATOR = 4
uint8 NAVIGATION_TRUNK_ROAD = 5
uint8 NAVIGATION_CLEANING = 6

uint8 navi_mode 

uint8 CMD_SOURCE_LEVEL_NAVIGATION = 0 #平层导航的命令
uint8 CMD_SOURCE_CROSS_GATE = 1       #越过闸机的命令
uint8 CMD_SOURCE_CROSS_DOOR = 2       #越过安全门的命令
uint8 CMD_SOURCE_ENTER_ELEVATOR = 3   #进电梯模块的命令
uint8 CMD_SOURCE_LEAVE_ELEVATOR = 4   #出电梯模块的命令
uint8 CMD_SOURCE_DODGE = 5            #躲避其它车辆命令
uint8 cmd_source #命令源

geometry_msgs/Pose pose #目标点

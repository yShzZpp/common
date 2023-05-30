
# cmd 列表
uint8 CMD_EXE_TASK = 0 #执行任务
uint8 CMD_CANCEL_TASK = 1 #取消任务
#临时方案:下发取消导航命令
uint8 CMD_CANCEL_STRAIGHT_TASK = 2 #取消命令:取消进入电梯后,不断前进的任务

# 发送给navigation control模块的控制命令
uint8 cmd

# 导航算法选择
# 重要事情: 当前动态切换算法有进程挂死问题, 请统一move base里的算法
uint8 NAVIGATION_MODE_DWA = 0 #dwa算法
uint8 NAVIGATION_MODE_TEB = 1 #teb算法
uint8 NAVIGATION_MODE_FTC = 2 #ftc算法
uint8 NAVIGATION_MODE_CDWA = 3 #cdwa算法

uint8 NAVIGATION_DEFAULT = 10				# 默认行驶
uint8 NAVIGATION_CROSSING_GATE = 11			# 穿过闸机、门禁
uint8 NAVIGATION_ENTER_ELEVATOR = 12		# 进电梯
uint8 NAVIGATION_CROWD_ELEVATOR = 13		# 挤电梯
uint8 NAVIGATION_TRUNK_ROAD = 14			# 主干道
uint8 NAVIGATION_CLEANING = 15				# 清扫路线
uint8 NAVIGATION_USER_PATH = 16				# 沿着用户画线行驶

uint8 navi_mode

########################################################################
# 目标点, pose.pose.position.z值在二维地图中使用不到,这里当做标志位使用
#
# 正常场景:
# float64 SPECIAL_SCENSE_NONE = 0.0
#
# 特殊场景A: local planner正常运行, 不允许执行任何recovery操作
# 应用场景: 
# 1. 正向离开电梯过程中
# 2. 进电梯的起步阶段
float64 SPECIAL_SCENSE_NO_RECOVERY = 1.0 
#
# 特殊场景B: local planner 不允许有角速度, 不允许后退, 前进速度减半, 不允许执行任何recovery操作
# 应用场景:
# 1. 进电梯,当机器部分/全部进到电梯里时
float64 SPECIAL_SCENSE_ENTER_LIFT = 2.0 
#
# 特殊场景C: local planner正常运行, 可进入recovery. 根据机器离目标点的距离,采取不同的recovery措施
# 应用场景:
# 1. 正常的平层导航
# 2. 动态躲避其他机器,往躲避点移动
float64 SPECIAL_SCENSE_NEAR_NAVIGATION = 3.0
#
# 特殊场景D: local planner不允许有任何命令下发, 直接进入recovery 
# 应用场景:
# 1. 进电梯,电梯门已释放,机器需要不断向电梯里面挤
float64 SPECIAL_CLOSE_TO_LIFT = 4.0
#
# 特殊场景E: local planner正常运行, 可进入recovery
# 1. 机器过闸机
# 2. 机器过安全门
float64 SPECIAL_THROUGH_THE_GATE = 5.0
#
# 请求clean path, 执行cleaning任务
float64 SPECIAL_CLEANING_TASK = -1.0
########################################################################
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

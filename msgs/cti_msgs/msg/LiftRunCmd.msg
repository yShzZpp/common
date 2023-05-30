
#命令列表
uint8 CMD_LEAVE_LIFT = 1 #离开电梯
uint8 CMD_CANCEL_LEAVE_LIFT = 4 #取消离开电梯命令

uint8 CMD_ENTER_LIFT = 2 #进入电梯
uint8 CMD_CANCEL_ENTER_LIFT = 3 #取消进入电梯命令

# uint8 CMD_CHANGED_TRIES_TIMES = 10 #改变尝试次数
uint8 cmd 

int8 tries_times #失败尝试次数

uint8 NAVI_MODE_DWA = 0 #dwa
uint8 NAVI_MODE_TEB = 1 #teb
uint8 NAVI_MODE_VFH = 2 #vfh
uint8 navi_mode #进入电梯使用的导航算法

# uint8 leave_navi_mode #离开电梯使用的导航算法

geometry_msgs/Pose waitpose    #进电梯时,等电梯的点
geometry_msgs/Pose destination #目标点

#离开电梯场景使用:
#1.电梯内的点 
geometry_msgs/Pose poseinlift 
#2.正常出电梯场景 + 一些异常出电梯场景
uint8 SCENC_LEAVE_LIFT = 0
uint8 SCENC_ENTER_LIFT_ERROR = 1
uint8 SCENC_ENTER_LIFT_ABORT = 2
uint8 SCENC_LEAVE_LIFT_NEED_HUMAN_INTERVENTION = 3
uint8 SCENC_ENTER_LIFT_RECOVERY_NEED_HUMAN_INTERVENTION = 4
uint8 SCENC_ENTER_LIFT_ABORT_NEED_HUMAN_INTERVENTION = 5
uint8 SCENC_OTHER_ROBOT_IN_LIFT = 6
uint8 scene


###进出电梯新方案需要的参数####
#进电梯:
# geometry_msgs/Pose waitliftpose #在电梯外等电梯的点
# float64 enterlift_freelift_distance #进电梯释放电梯门时机: 与waitliftpose点的距离
# float64 enterlift_robot_allin_distance #进电梯场景:机器认为自己全部进入的距离: 与waitliftpose点的距离
# float64 enterlift_no_spcase_distance #进电梯场景: 机器进不去向后退的距离: 与waitliftpose点的距离
# float64 enterlift_in_lift_util_stop_diatance #进电梯场景: 进入电梯后,一直向里移动,直到与最前面的障碍相聚: 默认10cm

# #出电梯:
# geometry_msgs/Pose exitliftpose #在电梯外等电梯的点
# float64 leavelift_freelift_distance #出电梯释放电梯门时机: 与电梯点的距离
   
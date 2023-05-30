
#
#  装箱相关
#
uint8 BOX_CMD_MOUNT = 0   #装箱命令
uint8 BOX_CMD_UNMOUNT = 1 #卸箱命令
#使用场景:
#1.装卸出错后,使得机器恢复到装箱/卸箱前的状态
#2.平台中止装卸箱任务.
uint8 BOX_CMD_RECOVERY_MOUNT = 2
uint8 BOX_CMD_RECOVERY_UNMOUNT = 3

#
#  充电相关 + 单元测试
#
uint8 CHARGE_CMD_ENTER = 4    #进桩充电
uint8 CHARGE_CMD_LEAVE = 5    #退桩停止充电
uint8 CHARGE_CMD_RECOVERY = 6 #恢复到可导航状态, 使用场景:1,进桩失败后恢复,2.进桩过程中停止操作.
uint8 CHARGE_CMD_CLOSE_DUCK = 7 #机器靠上充电桩,不对电量进行检测. 
uint8 CHARGE_CMD_CONTINUE_WITHBOX = 10       #继续充电 使用场景：1.机器断电前在充电，开机后依然认为在充电。带箱子
uint8 CHARGE_CMD_CONTINUE_WITHOUTBOX = 11    #继续充电 使用场景：1.机器断电前在充电，开机后依然认为在充电。不带箱子

#
#  装卸箱:单元测试
#
#uint8 TEST_CMD_MOUNT = 0    测试装箱流程与BOX_CMD_MOUNT命令一致. 故这里注释掉
uint8 TEST_CMD_UNMOUNT = 8 # 测试卸箱流程与BOX_CMD_UNMOUNT命令流程不一致.新增向前寻找二维码过程.

uint8 GIVEUP_CMD_TASK = 9 # 故障不可恢复 放弃此次任务.使机器能恢复成能导航

#
#  装卸箱: 直接抬升,或下降命令
#
uint8 DIRECT_LIFT_BOX = 12 # 直接抬升箱子
uint8 DIRECT_DOWN_BOX = 13 # 直接降下箱子

uint8 UNMOUNT_BOX_AHEAD_BOX = 15 # 把箱子卸到另一个箱子前面

#命令
uint8 cmd

#箱子的id,用来查询箱子的状态(目前只包括电量)
string box_id

#箱子类型,参考cti_msgs::Task中的box_type的宏
uint8 box_type

float32 ahead_box_width #卸到另一个箱子前面的距离

# CHARGE_CMD_LEAVE ==> 是否带箱退出
bool with_box

bool check_box
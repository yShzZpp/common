# 命令
uint32 CMD_STOP_FIND_QR = 0  #停止寻找二维码
uint32 CMD_START_FIND_QR = 1 #开启寻找二维码
uint32 CMD_LOCATION_USING_QR = 2 #用二维码重定位
uint32 CMD_FIND_ALL_QR = 3   #寻找当前位置所能看到的所有二维码id
uint32 CMD_CHECK_ROBOT_NO_MOVE = 4 #检测机器是否断电到再开机这段时间没有移动
uint32 command

# 目标点类型
uint32 TARGET_TYPE_DOCK = 0   #快递柜
uint32 TARGET_TYPE_CHARGE = 1 #充电桩
uint32 targetpose_type

# 命令CMD_START_FIND_QR用到的二维码信息
string target_id

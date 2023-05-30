# state 状态列表
int32 STATE_DISTINGUISH_FAIL = -1          # -1：失败，在进入装车状态后调整时跟丢二维码(不在视野内一段时间)
int32 STATE_DISTINGUISH_NO_READY = 0       # 0：未准备好,看不到二维码
int32 STATE_DISTINGUISH_MOVING = 1         # 1：可移动,看到二维码, 并且数据稳定可以使用。
int32 STATE_DISTINGUISH_TOO_CLOSE = 2      # 2：可移动,距离二维码太近,已看不到二维码
int32 STATE_DISTINGUISH_EXIST_OBSTACLE = 3 # 3：前方有障碍,不允许移动

## CMD_LOCATION_USING_QR命令下的返回状态, 
int32 STATE_LOCATION_GET_POSE = 10  #获取到位置信息
int32 STATE_LOCATION_NO_POSE = 11  #没有获取到位置信息

## CMD_FIND_ALL_QR命令下的返回状态.
int32 STATE_FIND_QR_RESULT = 20  #找寻二维码的结果, 只发一次

# 装卸车状态情况
int32 state

# state == -1时的错误信息
string brief_msg

# 充电时桩上是否有箱
bool is_occupied

## state == STATE_LOCATION_GET_POSE时返回的定位信息
cti_msgs/ChargingStationInfo dock_info

## state == STATE_FIND_QR_RESULT时返回的二维码信息
string[] all_qr

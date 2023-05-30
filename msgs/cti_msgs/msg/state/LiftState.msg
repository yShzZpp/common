
# state的状态列表:
uint8 LIFTSTATE_OPENDOOR_SUCCESS = 0  #电梯到达目标楼层，并已经开门
uint8 LIFTSTATE_OPENDOOR_FAIL = 1     #呼梯/开门--最终失败
uint8 LIFTSTATE_OPENDOOR_RECOVERY = 2 #呼梯出现失联情况--正在尝试重新连接
# uint8 LIFTSTATE_TIMING_REPORT = 3     #定时上报电梯所在楼层和电梯门状态
# 电梯通信模块发送电梯的状态
uint8 state

string errormsg

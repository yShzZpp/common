#state的状态列表
uint32 PASSAGEWAY_STATE_APPLY_SUCCESS = 0 #通道申请成功
uint32 PASSAGEWAY_STATE_APPLY_FAIL = 1    #通道申请失败
uint32 PASSAGEWAY_STATE_OPEN_SUCCESS = 2  #门开成功
uint32 PASSAGEWAY_STATE_OPEN_FAIL = 3     #门开失败
uint32 PASSAGEWAY_STATE_CLOSE_SUCCESS = 4 #门关成功
# uint32 PASSAGEWAY_STATE_CLOSE_FAIL = 5    #门关失败
uint32 PASSAGEWAY_STATE_FREE_SUCCESS = 6  #通道释放成功
# uint32 PASSAGEWAY_STATE_FREE_FAIL = 7     #通道释放失败
uint32 PASSAGEWAY_STATE_ABNORMAL = 8      #申请获得的通道异常
#闸机控制模块发出的状态
uint32 state

#出错后的信息
string errormsg

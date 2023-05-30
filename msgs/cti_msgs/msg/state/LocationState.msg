
# state的状态列表:
uint8 LOCATION_DOING = 0    #定位过程中
uint8 LOCATION_SUCCESS = 1  #定位成功
uint8 LOCATION_FAIL = 2     #定位失败
uint8 LOCATION_LOST = 3     #定位丢失

uint8 LOCATION_NO_ODOM_DATA = 10 # xx秒之内无里程计数据
uint8 LOCATION_NO_SCAN_DATA = 11 # xx秒之内无雷达数据

# 定位模块发送的定位状态
uint8 state

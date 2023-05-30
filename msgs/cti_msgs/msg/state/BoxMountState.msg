# 
# 装箱状态反馈 
# 
uint8 BOX_MOUNT_DOING = 0       #正在装卸箱 
uint8 BOX_MOUNT_SUCCESS = 1     #装卸箱成功 #中止后返回, 说明中止失败, 装箱/卸箱成功
uint8 BOX_MOUNT_FAIL = 2        #装卸箱失败 
uint8 BOX_RECOVERY_SUCCESS = 3  #恢复操作执行成功 #中止后返回, 说明中止成功,没有装箱/卸箱
uint8 BOX_RECOVERY_FAIL = 4     #恢复操作执行失败 

uint8 BOX_CLOSE_TO_SUCCESS = 10 #装卸箱时接近成功的时刻

# 
# 充电状态反馈 
# 
uint8 CHARGE_ENTER_SUCCESS = 5 
uint8 CHARGE_ENTER_FAIL = 6 
uint8 CHARGE_LEAVE_SUCCESS = 7 
uint8 CHARGE_LEAVE_FAIL = 8 
uint8 CHARGE_DOING_FAIL = 9 #充电过程中，出现异常，导致充电中途停止. 

uint8 CHARGE_CLOSETO_DUCK_SUCCESS = 11 #机器靠近充电桩成功
uint8 CHARGE_CLOSETO_DUCK_FAIL = 12 #机器靠近充电桩失败

# 底盘返回状态,是否成功挂载快递柜 
uint8 state

# 错误类型 装箱/卸箱都有
uint8 ERROR_TYPE_NO_ERROR = 0 
uint8 ERROR_TYPE_NOT_FIND_QR_CODE = 1   #找不到二维码
uint8 ERROR_TYPE_UNABLE_ACCESS_DOCK = 2 #无法进入桩
uint8 ERROR_TYPE_UNABLE_EXIT_DOCK = 3   #装箱: 无法退出桩  卸箱: 无法退出箱子底部
uint8 error_type

# 出错时的故障原因
string errormsg

# 时间戳 -- 用来判断是否是旧的消息
time timestamp

uint8 WARNING_TYPE_NO = 0               # 无告警
uint8 WARNING_TYPE_LOST_LOCATION = 1    # 定位丢失
uint8 WARNING_TYPE_LOW_POWER = 2        # 低电量
uint8 WARNING_TYPE_EMERGENT_STOP = 3    # 急停
uint8 WARNING_TYPE_EXECUTE_TIMEOUT = 4  # 执行超时
uint8 WARNING_TYPE_EXECUTE_FREOUT = 5   # 执行频
uint8 WARNING_TYPE_HARDEWARW = 6        # 嵌入式硬件
uint8 WARNING_TYPE_CHARGE = 7           # 充电
uint8 type


uint32 WARNING_NO = 0
uint32 WARNING_LOCATION_LOST = 1        #定位丢失
uint32 WARNING_LOW_POWER = 2            #电量低
uint32 WARNING_EMERGENCY_STOP = 3       #急停被按下
      
uint32 WARNING_OVERTIME = 10            #超时告警
uint32 WARNING_OVERTIME_CALLLIFT = 11   #呼梯超时
uint32 WARNING_OVERTIME_ENTERLIFT = 12  #进电梯超时
uint32 WARNING_OVERTIME_INLIFT = 13     #在电梯内等待超时
uint32 WARNING_OVERTIME_LEAVELIFT = 14  #离开电梯超时
uint32 WARNING_OVERTIME_MOUNTBOX = 15   #装箱超时
uint32 WARNING_OVERTIME_UNMOUNTBOX = 16 #卸箱超时
uint32 WARNING_OVERTIME_REACHGOAL = 17  #到达目标点超时

uint32 WARNING_OVERFREQUENCY = 20            #超频告警
uint32 WARNING_OVERFREQUENCY_MOUNTBOX = 21   #装箱超频
uint32 WARNING_OVERFREQUENCY_UNMOUNTBOX = 22 #卸箱超频
uint32 WARNING_OVERFREQUENCY_REACHGOAL = 23  #到达目标点超频

uint32 WARNING_CHARGE_FAIL = 30              #充电失败告警
uint32 WARNING_CHARGING_NO_ELECTRICITY = 31  #充电时充电桩电流消失告警

# 左驱动模块
uint32 WARNING_HARDWARE = 1000
# 1000,持续大负载运行 电机高温限流
# 1001,持续大负载运行 电机高温报警
# 1002,硬件电路检测到电压过高
# 1003,硬件电路检测到电流过大
# 1004,软件检测到电压过高
# 1005,软件检测到电流过大
# 1006,检测到三相线连接不完整
# 1007,检测到电机霍尔信号异常
# 1008,检测到电机编码器信号异常
# 1009,检测到大电流指令下电机不转动
# 1010,驱动器接收到的pwm指令周期异常
# 1011,驱动器接收到的pwm指令占空比异常
# 1012,驱动器自检不通过
# 1013,驱动器mos管发热

#右驱动模块
uint32 WARNING_HARDWARE_DRIVERMODE = 1100
# 1100,持续大负载运行 电机高温限流
# 1101,持续大负载运行 电机高温报警
# 1102,硬件电路检测到电压过高
# 1103,硬件电路检测到电流过大
# 1104,软件检测到电压过高
# 1105,软件检测到电流过大
# 1106,检测到三相线连接不完整
# 1107,检测到电机霍尔信号异常
# 1108,检测到电机编码器信号异常
# 1109,检测到大电流指令下电机不转动
# 1110,驱动器接收到的pwm指令周期异常
# 1111,驱动器接收到的pwm指令占空比异常
# 1112,驱动器自检不通过
# 1113,驱动器mos管发热
#uint32 WARNING_HARDWARE_DRIVERMODE_XXXX = 1101
#uint32 WARNING_HARDWARE_DRIVERMODE_YYYY = 1102

#顶升模块
uint32 WARNING_HARDWARE_JACKINGUP = 1200
# 1200,上升超时
# 1201,下降超时
# 1202,硬件检测过压
# 1203,硬件检测过流
# 1204,软件检测到电压过高
# 1205,软件检测到电流过大
# 1206,电机缺相
# 1207,霍尔故障
# 1208,保留
# 1209,电机堵转
# 1210,保留
# 1211,保留
# 1212,自检失败
# 1213,顶升上传感器信号异常
# 1214,顶升下传感器信号异常


#传感器模块
uint32 WARNING_HARDWARE_SENSEDATA = 1300

# 1300,1外置IMU零偏校准失败
# 1301,1外置IMU六面校准失败
# 1302,1外置IMU比例校准失败
# 1303,1外置IMUFLASH读写异常
# 1304,1外置IMU数据异常
# 1305,1外置IMU比例校准失败
# 1306,内置IMU六面校准失败
# 1307,内置IMU比例校准失败
# 1308,内置IMU数据异常
# 1309,超声波0数据异常
# 1310,超声波1数据异常
# 1311,超声波2数据异常
# 1312,超声波3数据异常
# 1313,超声波4数据异常
# 1314,单点激光0数据异常
# 1315,单点激光1数据异常
# 1316,单点激光2数据异常
# 1317,单点激光3数据异常
# 1318,单点激光4数据异常
# 1319,单点激光5数据异常
# 1320,红外对地模块0数据异常
# 1321,红外对地模块1数据异常
# 1322,红外对地模块2数据异常
# 1323,红外对地模块3数据异常
# 1324,底盘0度位置传感器异常
# 1325,底盘90度位置传感器异常
# 1326,对箱传感器异常
# 1327,对桩传感器左异常
# 1328,对桩传感器右异常


#电源管理模块
uint32 WARNING_HARDWARE_POWERMANAGER = 1400
# 1400,电池单体过压保护
# 1401,电池单体欠压保护
# 1402,电池整组过压保护
# 1403,电池整组欠压保护
# 1404,电池充电过温保护
# 1405,电池充电低温保护
# 1406,电池放电过温保护
# 1407,电池放电低温保护
# 1408,电池充电过流保护
# 1409,电池放电过流保护
# 1410,电池短路保护
# 1411,电池前端检测IC错误
# 1412,电池软件锁定MOS
# 1413,电源板硬件检测过压
# 1414,电源板硬件检测过流
# 1415,电源板软件检测过流

#通信模块
uint32 WARNING_HARDWARE_COMUNICATION = 1500
# 1500,工控机通信超时
# 1501,左驱动通信超时
# 1502,右驱动通信超时
# 1503,顶升驱动通信超时
# 1504,电源板通信超时
# 1505,数据采集板通信超时
# 1506,外置IMU通信超时
# 1507,内置IMU读写超时
# 1508,单点激光通信异常*6
# 1509,TofSense模块通信异常
# 1510,RFID通信异常


#本体状态模块,,,,
uint32 WARNING_HARDWARE_MOTIONSTATE = 1600
# 1600,左驱动打滑
# 1601,右驱动打滑
# 1602,车身连续打滑
# 1603,左驱动抱死
# 1604,右驱动抱死
# 1605,底盘回0°异常
# 1606,底盘回90°异常
# 1607,车身堵转
# 1608,车身撞击
# 1609,固件0版本不匹配
# 1610,固件1版本不匹配
# 1611,固件2版本不匹配
# 1612,固件3版本不匹配
# 1613,固件4升级失败
# 1614,固件1升级失败
# 1615,固件2升级失败
# 1616,固件3升级失败
# 1617,固件4升级失败

uint32 warning

string warningmsg

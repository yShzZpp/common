
#发布者随机生成,uuid形式的字符串
string id

#时间戳
time timestamp

#当前执行子任务id
string task_id

#当前任务组的id
string task_group_id

# 当前楼层
string current_building

# 目标大楼
string target_building

# 当前机器人所在楼层信息
string curLevel

# 机器人导航的目标楼层信息
string targetLevel

#当前机器人所在的位置坐标
geometry_msgs/Pose  location

#机器人当前Task_Navigation任务的最终目标点
geometry_msgs/Pose  final_destination

#当前机器人装卸箱流程的Id
string hiveDockOptId

#当前机器人 装卸箱流程状态
#机器人逻辑上的装箱状态
uint32 PAYLOADSTATE_EMPTY = 0                  #空箱
uint32 PAYLOADSTATE_LOADING = 1                #正在装箱
uint32 PAYLOADSTATE_LOADED = 2                 #已装箱
uint32 PAYLOADSTATE_UNLOADING = 3              #正在卸箱
uint32 PAYLOADSTATE_FAULT = 4                  #出现故障
uint32 PAYLOADSTATE_INIT = 5                   #正在初始化

uint32 PAYLOADSTATE_LOADING_ALMOSt_DONE  = 6   #装箱接近完成
uint32 PAYLOADSTATE_UNLOADING_ALMOSt_DONE  = 7 #卸箱接近完成
uint32 payloadState

# 装卸箱故障信息
string payloadFaultReson

# 当前机器人充电命令的ID
string chargeOptId

#当前机器人 充电流程状态
uint32 CHARGESTATE_INIT = 0          #无操作
uint32 CHARGESTATE_ENTERING = 1      #正在进入充电桩
uint32 CHARGESTATE_DOING = 2         #正在充电
uint32 CHARGESTATE_LEAVING = 3       #正在离开充电桩
uint32 CHARGESTATE_ERROR = 4         #出现故障
uint32 chargeOptState

#在充电过程中,是否出现了无法正常充电的情况
bool chargingvalid

# 充电故障信息
string chargeFaultReson

# robotstate 的状态列表见 BuildingRobotState.msg
uint32 robotstate

# robot_elevator_relationship 状态列表
uint8 ROBOT_ELEVATOR_OUTSIDE = 0  #机器人在电梯外
uint8 ROBOT_ELEVATOR_ENTERING = 1 #机器人正在进入电梯
uint8 ROBOT_ELEVATOR_INSIDE = 2   #机器人在电梯内
uint8 ROBOT_ELEVATOR_LEAVING = 3  #机器人正在离开电梯
uint8 robot_elevator_relationship

# 机器人运行模式
uint32 ROBOT_MODE_NAVIGATION = 0
uint32 ROBOT_MODE_MAPPING = 1
uint32 ROBOT_MODE_TESTMAPPS = 2
uint32 run_mode

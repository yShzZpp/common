
#####################################################
# 正常流程中的各个状态
#####################################################   

uint32 STATE_START = 0                   #robot刚刚启动
uint32 STATE_LOCATIONING = 1             #定位中
uint32 STATE_WAITTING_TASK = 2           #空闲状态，等待云端的任务
uint32 STATE_MOUNT_BOX = 3               #对准box,卡扣住快递柜,并且退出快递桩
uint32 STATE_UNMOUNT_BOX = 4             #进入快递桩,放下箱子,退出快递桩
uint32 STATE_NAVIGATIONING_LIFT = 5      #导航到电梯前的位置
uint32 STATE_WAITTING_LIFE_OUTSIDE = 6   #在电梯外等待电梯达到并开门
uint32 STATE_ENTERING_LIFE = 7           #进入电梯
uint32 STATE_WAITTING_LIFE_INSIDE = 8    #在电梯内等待电梯到达相应楼层并开门
uint32 STATE_LEAVEING_LIFE = 9           #离开电梯
uint32 STATE_NAVIGATIONING_PLACMENT = 10 #导航到楼层的指定位置
uint32 STATE_RE_LOCATIONING = 11         #重定位过程中
uint32 STATE_MAP_SYNC = 12               #正在同步地图
uint32 STATE_IN_ROMOTE_CONTROL = 13      #正在远程控制中(播放声音，移动，使能)
uint32 STATE_MACHINE_REBOOTING = 14      #机器正在重启
uint32 STATE_ALLINLIFT_AND_STILLMOVINGIN = 15   #机器已经全部进入电梯,释放电梯门,但还没有到达目标点.

uint32 STATE_NAVIGATIONING_GATE = 17        #导航到闸机前
uint32 STATE_WAITTING_GATE_OPEN = 18        #等待闸机开门
uint32 STATE_CROSS_GATE = 19                #越过闸机

uint32 STATE_NAVIGATIONING_LIFT_PAUSE = 21  #导航到电梯前过程中暂停状态
uint32 STATE_NAVIGATIONING_PLACMENT_PAUSE = 22 #导航到目标点过程中暂停状态
uint32 STATE_NAVIGATIONING_GATE_PAUSE = 23  #导航到闸机前的点过程中暂停状态

uint32 STATE_CHARGE_ENTER = 24  #进入充电桩
uint32 STATE_CHARGING = 25      #正在充电
uint32 STATE_CHARGE_LEAVE = 26  #离开充电桩

uint32 STATE_FINDING_ALL_QR = 27 #正在寻找所有二维码

uint32 STATE_DYNAMIC_DODGING = 28 #机器正在移动躲避其它机器
uint32 STATE_STATIC_DODGING = 29 #机器正在停车躲避其它机器

uint32 STATE_CLOSETO_DUCK = 30 #机器正在靠近充电桩
uint32 STATE_ON_DUCK = 31 #机器在桩上(有可能是充电桩, 有可能不是充电桩) 

uint32 STATE_NAVIGATION_APPLYING_ELEVATOR = 33  #正在申请占用电梯

uint32 STATE_FINDING_QR  = 34               #正在寻找二维码,并确定机器位置
uint32 STATE_WAITING_IN_SUIT  = 35          #正在执行原地等待命令

uint32 STATE_SELF_GET_POSE = 38             #正在自主寻找定位信息
uint32 STATE_SELF_CHECKING_CLOUDCONNECT = 39 #正在自检与平台服务器的连接
uint32 STATE_SELF_CHECKING_SENSORS = 40     #正在自检传感器
uint32 STATE_CHECK_LASTSTATE = 41           #正在检测断电前的状态
uint32 STATE_WAITING_NTP = 42               #正在等待同步网络时间
uint32 STATE_SELF_LOCATION = 43             #正在自主定位中

uint32 STATE_WASHFLOOR_ING = 47             #正在洗地
uint32 STATE_WASHFLOOR_PAUSED = 48          #洗地过程中被暂停
uint32 STATE_WASHFLOOR_STOPPING = 49        #洗地过程中正在执行中止动作
uint32 STATE_WASHFLOOT_MANUAL = 50          #洗地过程中被切换成手动模式

uint32 STATE_DO_WASH_STATION = 51           #正在执行清洗工作站任务

uint32 STATE_DINFECTING_FLOOR = 53          #正在消毒
uint32 STATE_DINFECTING_FLOOR_PAUSE = 54    #消毒过程中被暂停

uint32 STATE_SWITCHING_NAVI_MODE = 61       #正在切换导航模式
uint32 STATE_SWITCHING_MAPP_MODE = 62       #正在切换建图模式
uint32 STATE_SWITCHING_TEST_MAP_MODE = 63   #正在切换测试地图模式

uint32 STATE_MAPPING_MODE = 71    #建图模式
uint32 STATE_DO_MAPPING = 72      #正在建图
uint32 STATE_TESTMAP_MODE = 73    #测试地图模式 
uint32 STATE_RECEIVING_MAPS = 74  #正在接收app推送的地图集

uint32 STATE_SUBTASK_START = 81   #临时状态: 防止连续命令导致状态机混乱

#####################################################
# 异常/故障/失败/错误状态
#####################################################

uint32 STATE_LOCATION_LOST = 100            #定位丢失状态
uint32 STATE_CHANGEMAP_FAIL = 101           #切换地图失败
uint32 STATE_ENTERING_ELEVATOR_FAIL = 102   #进入电梯失败
uint32 STATE_LEAVEING_ELEVATOR_FAIL = 103   #离开电梯失败
uint32 STATE_CALL_ELEVATOR_FAIL = 104       #呼梯失败
uint32 STATE_OPEN_ELEVATOR_FAIL = 105       #robot电梯内,电梯一直不开门,导致无法出电梯错误
uint32 STATE_NAVIGATION_LIFT_ERROR = 106    #导航到电梯前失败,无法到达目的地
uint32 STATE_NAVIGATION_PLAT_ERROR = 107    #导航到目标点失败,无法到达目的地
uint32 STATE_RE_LOCATIONING_ERROR = 110     #重定位失败
uint32 STATE_MAP_SYNC_ERROR = 112           #同步地图失败

uint32 STATE_NAVIGATIONING_GATE_ERROR = 117 #导航到闸机前失败
uint32 STATE_WAITTING_GATE_OPEN_FAIL = 118  #闸机开门失败
uint32 STATE_CROSS_GATE_ERROR = 119         #越过闸机失败

uint32 STATE_SELF_CHECK_ERROR = 120         #自检失败

uint32 STATE_CHARGE_ENTER_ERROR = 124  #进入充电桩失败
uint32 STATE_CHARGING_ERROR = 125      #正在充电中出现异常
uint32 STATE_CHARGE_LEAVE_ERROR = 126  #离开充电桩失败

uint32 STATE_CLOSETO_DUCK_ERROR = 130  #机器正在靠近充电桩失败

uint32 STATE_NAVIGATION_CONNECT_ELEVATORSERVER_ERROR = 133 #无法连接到电梯服务器
uint32 STATE_NAVIGATION_ELEVATORINFO_NOTEXIST = 134 #平台/电梯服务器下发的电梯id在robot的楼宇cache中无法找到
uint32 STATE_NAVIGATION_NO_PATH_TO_DES_LEVEL = 135 #从当前楼层没有路径可达目标楼层

uint32 STATE_SELF_GET_POSE_ERROR = 138 #平台端下发的自动定位任务失败

uint32 STATE_SELF_CHECKING_SENSORS_FAIL = 140  #传感器自检失败

uint32 STATE_MOUNT_BOX_FAIL = 151           #装箱失败
uint32 STATE_UNMOUNT_BOX_FAIL = 152         #卸箱失败
uint32 STATE_DINFECT_FLOOR_FAIL = 153       #消毒失败

uint32 STATE_SWITCHING_NAVI_MODE_FAIL = 161      #切换导航模式失败
uint32 STATE_SWITCHING_MAPP_MODE_FAIL = 162      #切换建图模式失败
uint32 STATE_SWITCHING_TEST_MAP_MODE_FAIL = 163  #切换测试地图模式失败

uint32 STATE_RECEIVING_MAPS_ERROR = 174     #接收app推送的地图集出错

uint32 STATE_WASH_PRE_PROCESSING_ERROR = 179 #清洗任务预处理失败
uint32 STATE_WASH_BOX_ERROR = 180            #清洗组件异常
uint32 STATE_WASH_NAVI_ERROR = 181           #清洗过程中导航出现异常

#####################################################
# 异常/故障/失败/错误之后机器进行自我恢复的状态
#####################################################

# 异常后自动恢复
uint32 STATE_RECOVERY_LOCATION_LOST = 200      #定位丢失后,正在恢复
uint32 STATE_RECOVERY_ENTERING_LIFE_FAIL = 202 #进入电梯失败后,需要再次退出到候梯位置
uint32 STATE_RECOVERY_CALL_LIFT_FAIL = 203     #在电梯外呼梯失联,正在恢复
uint32 STATE_RECOVERY_OPEN_LIFT_FAIL = 204     #在电梯内呼梯失联,正在恢复
# uint32 STATE_RECOVERY_WAIT_LIFT_GONE_AFTER_ENTER_LIFE_FAIL_BACK = 205  #等待电梯离开,在进入电梯失败并退到避让位置之后
uint32 STATE_RECOVERY_CROSS_GATE_FAIL = 219    #过闸机失败退回到等待点
uint32 STATE_RECOVERY_CHANGE_ERROR = 220       #正在恢复充电异常状态, 主要等待轮子回正

uint32 STATE_NAVIGATION_APPLYING_ELEVATOR_AGAIN = 233  #目前尚无可用电梯,再次申请占用电梯


#####################################################
# 机器正在中止当前流程 的状态
#####################################################

uint32 STATE_ABORTING_ENTERING_LIFE = 302      #正在中止进电梯状态,需要再次退出到候梯位置
# uint32 STATE_ABORTING_WAITTING_LIFT = 304      #正在中止等电梯状态.
uint32 STATE_ABORTING_NAVIGATION = 306         #正在中止导航状态.
uint32 STATE_ABORTING_SYNCMAP = 312            #正在中止地图更新状态
uint32 STATE_ABORTING_DODGING = 316            #正在中止躲避任务
# uint32 STATE_ABORTING_WAITTING_GATE_OPEN = 318 #正在中止等待闸机开门状态
uint32 STATE_ABORTING_CROSS_GATE = 319         #正在中止越过闸机

uint32 STATE_ABORTING_WASHING_FLOOR = 350      #正在停止洗地任务
uint32 STATE_ABORTING_MOUNT_BOX = 351          #正在中止装箱
uint32 STATE_ABORTING_UNMOUNT_BOX = 352        #正在中止卸箱
uint32 STATE_ABORTING_DINFECT_FLOOR = 353      #正在中止消毒任务

#####################################################
# 终止状态: 需要人工手动干预!!!
#####################################################

uint32 STATE_ENTER_LIFT_RECOVERY_NEED_HUMAN_INTERVENTION = 400 #进电梯恢复出现异常,需要人工干预
uint32 STATE_ENTER_LIFT_ABORT_NEED_HUMAN_INTERVENTION = 401    #进电梯中止出现异常,需要人工干预
uint32 STATE_IN_LIFT_NEED_HUMAN_INTERVENTION = 402             #在电梯内等待出现异常,需要人工干预
uint32 STATE_LEAVE_LIFT_NEED_HUMAN_INTERVENTION = 403          #离开电梯出现异常,需要人工干预
uint32 STATE_NEED_HUMAN_INTERVENTION = 404                     #装卸箱/充电状态等 无法恢复,无法重置 的 终止状态 , 需要人为进行干预
uint32 STATE_NO_ODOM_DATA = 405       #无里程计数据
uint32 STATE_NO_FRONT_SCAN_DATA = 406 #无前雷达数据
uint32 STATE_NO_REAR_SCAN_DATA = 407  #无后雷达数据
uint32 STATE_MOUNT_ERROR_NEED_HUMAN_INTERVENTION = 408  #装卸箱失败时,需要人工干预
uint32 STATE_UP_DOWN_NEED_HUMAN_INTERVENTION = 410  #气压计检测异常, 机器进入滚梯or掉落电梯间
uint32 STATE_WASHING_FLOOR_NEED_HUMAN_INTERVENTION = 450  #洗地过程中出现异常, 需要人工干预

# 机器人的状态机列表,维护自身状态
uint32 state

# 机器人id
string robotid

# 目前只有电梯通信模块使用到,未来可能有更多的模块要用到
string target_floor   # 目标楼层
string current_floor  # 当前楼层

string park_name              # 当前园区
string park_id                # 当前园区id
string park_chinese_name      # 当前园区的中文名
string buildingname           # 当前大楼
string building_id            # 当前大楼id
string building_chinese_name  # 当前大楼的中文名

string target_buildingname #目标大楼

string elevatorid

# loadstate的状态列表
# 机器上(rfid识别/抬升判断)到实际的箱子状态
uint8 STATE_LOAD_EMPTY = 0  #空载
uint8 STATE_LOAD_LOAD = 1   #负载
uint8 STATE_LOAD_UNKNOW = 2
uint8 loadstate

# platform需要的箱子全状态 -> payloadState of RobotNotifyState.msg
uint32 payloadState

# 箱子的类型 -> box_type of Task.msg
uint32 box_type

# 箱子的id s
string box_show_id
string box_uuid

# 充电状态
# uint32 chargestate

# 当前目的地的坐标, 进电梯模块,出电梯模块需要知道
## STATE_WAITTING_TASK = 2 :全0 
# STATE_NAVIGATIONING_LIFT = 5 :当前楼层电梯前的等待点
## STATE_WAITTING_LIFE_OUTSIDE = 6 :全0
# STATE_ENTERING_LIFE = 7 :当前楼层电梯里的点
# STATE_WAITTING_LIFE_INSIDE = 8 :目标楼层电梯里的点
# STATE_LEAVEING_LIFE = 9 :目标楼层电梯外的出电梯点
# STATE_NAVIGATIONING_PLACMENT = 10 :最终目的地
geometry_msgs/Pose destination

# 当前位置
geometry_msgs/Pose pose

# 避让机器人id
string dodge_robot_id

# 当前进出电梯的模式 参考NaviRequest.msg里面的定义
# uint32 elevatormode

#机器当前经过的闸机
string cross_gate_name
#机器当前经过的闸机方向
bool cross_gate_in

#当前机器是否经过定位
bool is_location

#机器人型号
string robot_type
#机器人编号
string robot_number
#机器人运行模式 导航/建图
uint32 run_mode
#机器人运行功能 快递/消毒 机器人
uint8 ROBOT_FUNCTION_EXPRESS = 0
uint8 ROBOT_FUNCTION_MEDICAL = 1
uint8 ROBOT_FUNCTION_WASHFLOOR = 2
uint8 ROBOT_FUNCTION_GARBAGE = 3
uint8 robot_function

# 时间戳 -- 用来判断是否是旧的消息
time timestamp

# 机器开机的时间戳(服务启动)
time robot_startup_timestamp
#当前任务uuid
string task_id

# 任务类型
## 串行任务
### 导航模式
uint32 TYPE_NAVIGATION        = 0  #导航命令(包括平层导航和跨楼层导航)
uint32 TYPE_RE_LOCATION       = 1  #重定位--指定楼层地点的重定位
uint32 TYPE_MOUNT_BOX         = 2  #装箱
uint32 TYPE_UNMOUNT_BOX       = 3  #卸箱
uint32 TYPE_CHARGE            = 4  #开启充电 #停止充电是stop当前充电任务
uint32 TYPE_WAIT_IN_SUIT      = 5  #原地等待
uint32 TYPE_SELF_LOCATION     = 6  #自主定位
uint32 TYPE_MAP_SYNC          = 7  #从云端同步地图
uint32 TYPE_WASH_FLOOR        = 8  #清洗任务
uint32 TYPE_DINFECT_FLOOR     = 9  #消毒任务
uint32 TYPE_PUSH_MAPS         = 10 #手动推送地图到机器上
### 建图模式
uint32 TYPE_MAPPING           = 20 #启动建图 #结束建图的动作是stop当前任务
### 提供给平台命令
uint32 TYPE_FIND_ALL_QR       = 31 #寻找当前位置所能看到的所有二维码

## 并行任务
uint32 TYPE_DIRECT_LIFTDOWN_BOX    = 60  #直接抬升/下降箱子
uint32 TYPE_REMOTE_CONTROL         = 61  #远程控制
uint32 TYPE_REBOOT_MACHINE         = 62  #断电重启机器
uint32 TYPE_SWITCH_NAVIGATION_MODE = 63  #切换成导航模式
uint32 TYPE_SWITCH_MAPPING_MODE    = 64  #切换成建图模式
uint32 TYPE_SWITCH_TESTMAP_MODE    = 65  #切换成测试地图模式
uint32 TYPE_OPEN_WIFI_HOTSPOT      = 66  #打开wifi热点
uint32 TYPE_CLOSE_WIFI_HOTSPOT     = 67  #关闭wifi热点

## 本地调度任务 不对云端开放
uint32 TYPE_SCHEDULE_DODGE         = 80  #躲避任务 --导航到另一个点
uint32 TYPE_SCHEDULE_WAIT          = 81  #等待任务 --原地等待

## 状态机内部任务 不对云端+调度开放
uint32 TYPE_FLATLAYER_NAVIGATION     = 100   #平层导航 
uint32 TYPE_UP_DOWN_ELEVATOR         = 101   #乘坐电梯 
uint32 TYPE_NAVIGATION_ELEVATOR      = 102   #导航到电梯前的点
uint32 TYPE_BOOT_UP_SELF_CHECK       = 103   #开机自检任务序列 
uint32 TYPE_CHECKSTATE_BE_POWEROFF   = 104   #检测机器断电前状态 
uint32 TYPE_SELF_CHECK_SENSORS       = 105   #自检机器的传感器信息
uint32 TYPE_FIND_LOCATION_USING_QR   = 106   #用二维码信息重定位
uint32 TYPE_CHECK_NTP                = 107   #自检机器是否已同步网络时间
uint32 TYPE_CHECK_CLOUD_CONNECT      = 108   #自检机器是否已连接上云服务器,并获取到本机配置信息
uint32 TYPE_SELF_GET_POSE            = 109   #自动寻找机器人位置信息
uint32 TYPE_BOOT_UP_SELF_LOCATION    = 110   #开机自动定位

uint32 type

##### TYPE_NAVIGATION/TYPE_SCHEDULE_DODGE ##########################################
string desBuilding      #目标大楼  //TYPE_DINFECT_FLOOR
geometry_msgs/Pose pose #目标点 
string desLevel         #目标楼层  //TYPE_DINFECT_FLOOR
string entity_id        #电梯id   
string elevator_id      #电梯id   //TYPE_DINFECT_FLOOR
uint32 pose_type        #点的类型 -> CloudWaypointInfo.msg type
bool fuzzy_parking      #是否允许模糊停车

##### TYPE_RE_LOCATION ####################################################
geometry_msgs/Pose repose #重定位点
string rebuilding         #重定位大楼
string relevel            #重定位楼层

##### TYPE_MOUNT_BOX / TYPE_UNMOUNT_BOX ###################################
##### TYPE_CHARGE #########################################################
string box_uuid      #箱子的uuid,用来查询箱子的状态(目前只包括电量)
string box_show_id   #箱子上显示的编号(用来和洗地箱通信/或者此状态用上面的box_id查询出来)
string qr
string rfid
bool just_on_duck
bool check_box_charge_state  #是否需要检测机器上的箱子有没有充上点

#箱子类型
uint32 BOX_TYPE_EXPRESS = 0          #快递柜
uint32 BOX_TYPE_DISINFECTION = 1     #消毒柜
uint32 BOX_TYPE_GARBAGE = 2          #垃圾箱
uint32 BOX_TYPE_VENDING = 3          #零售箱
uint32 BOX_TYPE_CLEANING = 4         #清扫箱
uint32 BOX_TYPE_WELCOME = 5          #迎宾箱
uint32 BOX_TYPE_DELIVERY = 6         #派件箱
uint32 BOX_TYPE_DOUBLE_DELIVERY = 7  #双层箱
uint32 BOX_TYPE_PATROL = 8           #巡逻箱
uint32 BOX_TYPE_MUL_DISINFECTION = 9 #多功能消毒箱
uint32 BOX_TYPE_HIGH_VALUE = 10      #医疗高值耗材箱

uint32 BOX_TYPE_CHARGING_PILE = 100  #是充电桩/不是箱子
uint32 BOX_TYPE_UNKNOWN = 404        #未知
uint32 box_type

uint32 SCENE_NORMAL = 0              #正常装卸箱(寻二维码装箱和原地卸箱)
uint32 SCENE_UNMOUNT_ON_CHARGE = 1   #卸到充电桩上
uint32 SCENE_UNMOUNT_AHEAD_BOX = 2   #卸到另一个箱子前面
uint32 box_special_scenes

float32 ahead_box_width  #卸到另一个箱子前面的距离

##### TYPE_DIRECT_LIFTDOWN_BOX ############################################
uint32 DIRECT_LIFT_BOX = 0 #直接抬升箱子
uint32 DIRECT_DOWN_BOX = 1 #直接下降箱子
uint32 direct_liftdown_type

##### TYPE_OPEN_WIFI_HOTSPOT
##### TYPE_SWITCH_MAPPING_MODE
##### TYPE_SWITCH_TESTMAP_MODE ############################################
#string ssid # cti-xxx
string pwd 

##### TYPE_PUSH_MAPS ######################################################
string[] files_list 

##### TYPE_MAPPING / TYPE_PUSH_MAPS:building_name #########################
string building_name
string floor_num

##### TYPE_MAP_SYNC:building_name #########################################
string[] map_file_id_collection

##### TYPE_REMOTE_CONTROL #################################################
uint32 REMOTE_CONTROL_E_STOP_ENABLE = 0
uint32 REMOTE_CONTROL_E_STOP_DISABLE = 1
uint32 REMOTE_CONTROL_GO_FORWARD = 2
uint32 REMOTE_CONTROL_GO_BACK = 3
uint32 REMOTE_CONTROL_TURN_LEFT = 4
uint32 REMOTE_CONTROL_TURN_RIGHT = 5
uint32 REMOTE_CONTROL_PLAY_SOUND = 6
uint32 control

#{"distance": 0.1, "angle": 90, "voice": 1}
string param 
###########################################################################

##### TYPE_WAIT_IN_SUIT/TYPE_SCHEDULE_WAIT ################################
float32 wait_time
###########################################################################

##### TYPE_WASH_FLOOR #####################################################
string module_params    #清洗组件参数
string washing_params   #清洗运动参数
###########################################################################

##### TYPE_SELF_LOCATION ##################################################
string location_scene  #见SelfLocationRequest.msg的scene属性
###########################################################################

##### TYPE_REBOOT_MACHINE #################################################
#多长时间后机器重启,以分钟为单位.
#0:代表立刻重启
#负数:代码关机后不再启动
int32 reboot_time 
###########################################################################

##### TYPE_SCHEDULE_DODGE/TYPE_SCHEDULE_WAIT ##############################
string dodge_robot_id
###########################################################################

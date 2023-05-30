# 因为原BuildingRobotState 经常变动导致看bag消息不懂  
# 现拷贝一份简版 发不来 不经常变动
# publisher node : /robot_base_node
# topic: /robot_base/brief_state  播放包用
# 机器人的状态机列表,维护自身状态
uint32 state

# 机器人id
string robotid

# 目前只有电梯通信模块使用到,未来可能有更多的模块要用到
string target_floor   # 目标楼层
string current_floor  # 当前楼层

string buildingname

string elevatorid

uint8 loadstate
# 充电状态
uint32 chargestate
# 当前位置
geometry_msgs/Pose pose
# 当前进出电梯的模式 参考NaviRequest.msg里面的定义
uint32 elevatormode
#机器人型号
string robot_type
#机器人编号
string robot_number
#机器人运行模式 导航/建图
uint32 run_mode
uint8 robot_function
# 时间戳 -- 用来判断是否是旧的消息
time timestamp

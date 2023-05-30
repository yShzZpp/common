# cmd 列表
uint8 CMD_CALL_ELEVATOR = 0   # 呼梯
uint8 CMD_CLOSE_DOOR = 1      # 关门
uint8 CMD_CANCEL_TASK = 2     # 取消任务
uint8 CMD_CALL_ELEVATOR_IN_ELEVATOR = 3 #在电梯里呼梯
uint8 CMD_OPEN_DOOR = 4 #开门

# 发送给梯控模块的控制命令
uint8 cmd

string elevator_id
string entity_id

#后续将current_floor和target_floor合并成一个

string target_floor   # 目标楼层
string current_floor  # 当前楼层

string token #与服务器通信时认证需要

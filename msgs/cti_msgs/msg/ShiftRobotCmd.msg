
uint8 SHIFT_FORWARD_BACK = 0     # 前后运动
uint8 SHIFT_LATERAL = 1          # 左右运动
uint8 SHIFT_ROTATE = 2           # 旋转运动

uint8 shift_mode                 # 移动机器操作 类型
float32 shift_value              # 运动量
float32 min_obstacle             # 最小障碍物距离
float32 timeout_sec              # 超时必须要返回结果 (秒)
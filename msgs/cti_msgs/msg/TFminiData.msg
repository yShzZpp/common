
# 北醒单点测地 健康值
Header header

# 对传感器的状态描述 
uint8 state     
uint8 OK = 0                    # 模块工作OK
uint8 FAIL_READ_DATA = 1        # 读取数据失败
uint8 UNBELIEVABLE_DATA = 2         # 数据非法
uint8 FAIL_OPEN_PORT = 4        # 打开端口失败
uint8 SENSOR_BLOCK = 5          # 模块被遮挡
uint8 DISTANCE_LOSS = 6         # 模块距离丢失  对天 或者 黑色反射材质吸收了


#地面场景
uint8 scene_type 
uint8 NORMAL_GROUND = 0      # 正常的地面   机器速度全开
uint8 NORMAL_STEPS = 1       # 检测到的台阶  机器限制速度
uint8 LONGLONG_STEPS = 2     # 检测到的很长的台阶  机器限制速度限制前进速度为0
uint8 LOCK_ROBOT = 3         # 锁住机器 


uint8 port_state    # 串口工作状态
int8 read_state     # 数据读取

bool step_detect   # true 检测到台阶            false 未检测到台阶
bool lock_robot    # true 锁住机器人            false 机器人还未锁住
bool open_detect   # true 打开对台阶检测功能     false 台阶检测关闭

float32 dist                 # 测量到的距离值
float32 dist_mean            # 数据滤波
float32 dist_var             # 距离值方差

uint16 strength              # 测量到的信号强度值
float32 strength_mean        # 滤波之后的强度值
float32 strength_var         # 强度值方差  

float32 temp                 # 模块的温度
float32 trigger_threshold    # 黑色反射面触发悬崖的阈值
float32 black_trigger_threshold  # 黑色反射面触发悬崖的阈值

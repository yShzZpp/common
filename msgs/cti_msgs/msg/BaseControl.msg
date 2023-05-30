Header header

# 控制信号来源 方便调试
# uint8 SOURCE_APP = 0        # 标识控制信号来源于  app模块  
uint8 SOURCE_DOCK = 1       # 标识控制信号来源于  装卸箱模块
uint8 SOURCE_REMOTE = 2     # 标识控制信号来源于  远程监控模块
uint8 SOURCE_JOYSTICK = 3   # 标识控制信号来源于  手柄模块
uint8 SOURCE_STATEMACHINE = 4  # 标识控制信号来源于  状态机模块
uint8 contrl_source         # 控制信号来源       指明控制来源


# 电机离合
uint8 MOTOR_NULL  = 0       # 无动作
uint8 MOTOR_DISABLE  = 1    # 电机(离合) 失能
uint8 MOTOR_ENABLE  = 2     # 电机(离合) 使能
uint8 ctrl_motor

# 轮盘控制
uint8 WHEEL_NULL  = 0       # 无动作
uint8 WHEEL_00  = 1         # 轮盘 00 度
uint8 WHEEL_90  = 2         # 轮盘 90 度
uint8 ctrl_driving_wheel	 

# 抬升控制
uint8 PLATE_NULL  = 0       # 无动作
uint8 PLATE_DOWN  = 1       # 抬升 下降
uint8 PLATE_UP    = 2       # 抬升  上升
uint8 ctrl_plate	

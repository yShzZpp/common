
#Header header

uint8 CONTROL_MODE_NAVI = 1  # 1  导航控制
uint8 CONTROL_MODE_JOY = 2   # 2  手柄控制
uint8 CONTROL_MODE_APP = 3   # 3  app控制
uint8 current_state_mode     # 指明  导航当前被谁控制  
   
uint8 ver_uart_protocol    # 导航与运控使用的协议版本 

uint32 version_app_motion     # 运控app固件版本 
uint32 version_boot_motion    # 运控boot固件版本

uint32 version_app_power      # 电源板app固件版本 
uint32 version_boot_power     # 电源板boot固件版本 

uint32 version_app_driver_l   # 左驱动app固件版本 
uint32 version_boot_driver_l  # 左驱动boot固件版本 

uint32 version_app_driver_r   # 右驱动app固件版本 
uint32 version_boot_driver_r  # 右驱动boot固件版本 

uint32 version_app_imu        # IMU app固件版本 
uint32 version_boot_imu       # IMU boot固件版本 

uint32 version_app_lifting      # 抬升机构 app固件版本 
uint32 version_boot_lifting     # 抬升机构 app固件版本 

float32  total_odom       # 行车总里程

bool base_is_alive    # 底盘是否通信上  ture : 运控有数据   false:运控无数据上报



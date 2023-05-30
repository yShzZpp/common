# topic : /elevator_sensor
Header header

uint8 STOP = 0                #   0：停止 
uint8 START_MOVE_UP = 1       #   1：向上运动开始  
uint8 START_MOVE_DOWN = 2     #   2：向下运动开始  
uint8 MOVEING_UP = 3          #   3：向上运动中  
uint8 MOVEING_DOWN = 4        #   4：向下运动中
uint8 elevator_move_status

# bit 0 : 气压计状态 （0 正常  1无数据）
uint8 BAROMETER_OK = 0                  
uint8 BAROMETER_ERROR = 1       
uint8 barometer_status

#    bit 1 : 加速度计校准状态 （0 已校准 1未校准）
uint8 ACC_CALOBRAT_COMPLETE = 0                                    
uint8 ACC_CALOBRAT_ERROR  = 1  
uint8 acc_cali_status     
       
#    bit 2 : IMU放置姿态 （0 安装水平 1 未安装水平）
uint8 IMU_POSE_HORIZONTAL = 0           
uint8 IMU_POSE_NO_HORIZONTAL = 1
uint8 imu_pose                            
 
#    bit 3 : 楼层检测超时判断（0 正常 1超时）
uint8 FLOOR_DET_NORMAL = 0               
uint8 FLOOR_DET_TIMEOUT = 1  
uint8 floor_det_timeout                           

#    bit 4 : 获取电梯参数（0 正常 1无数据）
uint8 PARAM_NORMAL = 0
uint8 PARAM_NO_DATA = 1
uint8 elevator_param

float32 floor_height  # 电梯一次动作楼层变化高度    
float32 floor_nums                  # 电梯一次动作楼层变化层数   
float32 barometer_height_raw #     气压计原始高度   
float32 barometer_height_fuse # 融合后高度 
float32 imu_acc_z #  IMU加速度Z方向原始值  
float32 wcs_acc_z #  世界坐标系Z方向运动加速度   
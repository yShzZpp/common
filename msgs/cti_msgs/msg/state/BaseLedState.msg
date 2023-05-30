
#Header header

#底盘运控上报的数据
#抬升状态 
uint8 cmd_updown
#对地检测 
uint8 updown_state 
#头顶超声波 Laser_dis[4]
float32[] Laser_dis
uint8 head_ultra_1
# 打滑检测-加速度值 
uint8 head_ultra_2 
uint8 motor_variance_l
uint8 motor_variance_r 
float32 Limit_IRange_Restore 
float32 Limit_OutRange_Restore

# 限位开关-bit map
uint16 limit_switch
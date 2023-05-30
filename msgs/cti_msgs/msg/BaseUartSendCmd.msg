Header header

# robot_base 节点实际下发控制变量
# topic: /robot_base/debug/sendcmd
uint8 cmd_motor_clutch    # 控制底盘离合 是否使能  1 disable  2 enable
uint8 cmd_control_mode
uint8 cmd_driving_wheel   # 控制轮盘    1 0度   2 90度
uint8 cmd_plate           # 控制抬升    1 下降  2 升起

uint8 head_ultra_on
uint8 foot_det_on
uint8 step_det_on
uint8 shoulder_tf_on
uint8 neck_tf_on
uint8 rear_tf_on
uint8 touch_det_on
uint8 have_box
uint8 clr_odom
uint8 system_reset
uint8 pd_lock
uint8 uart_no_data
uint8 neck_front_ultra_on   # 脖子前
uint8 left_front_ultra_on   # 左前
uint8 right_front_ultra_on  # 右前
uint8 left_back_ultra_on    # 左后
uint8 right_back_ultra_on  
uint8 cutoff_record_power
uint8 cutoff_lora_power
uint8 cutoff_zigbee_power
uint8 cutoff_mc_power




# uint16 tx_timeout_cnt
# uint16 rx_timeout_cnt
# uint16 crc_err_cnt
# uint16 cksum_err_cnt
# uint16 int_cnt
# uint16 reset_cnt
# uint16 send_cnt
# uint16 frame_err_cnt0
# uint16 frame_err_cnt1
# uint16 frame_err_cnt2

# uint8 tx_timeout
# uint8 rx_timeout
# uint8 single_timeout0
# uint8 single_timeout1
# uint8 single_timeout2
# uint8 bus_bad0
# uint8 bus_bad1
# uint8 bus_bad2
# uint16_t _reserved1:8;

# // important state to nav
uint8 slip_left
uint8 slip_right
uint8 slip_continue
uint8 locked_rotor_l
uint8 locked_rotor_r
uint8 limit_current
uint8 motion_err_disable
uint8 bad_condition
uint8 lifting_err
uint8 wheel_driving_err
uint8 box_sensor_err
uint8 energy_can_id_err
uint8 battery_485_err
uint8 vehicle_hit
uint8 slope_too_large
uint8 slope_too_long
uint8 w_too_large
uint8 rpm_diff_too_large
uint8 move_camp
# extra infomation for motion driver
uint8 machine_reset_code
uint8 motor_disable_idx
uint8 imu_error 
uint16 motion_reboot # 
uint16 type_of_box # 箱子类型
    

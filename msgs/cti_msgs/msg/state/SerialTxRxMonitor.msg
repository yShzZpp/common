
Header header

# 运控与底盘通信丢个包通信监控
#topic: /robot_base/serial_monitor

# 导航
uint32 nav_tx_counter        # 导航发出的数据包数
uint32 nav_rx_counter        # 导航收到的数据包数
uint32 nav_rx_err_counter    # 导航收到因校验失败丢弃的数据包数
# 底盘
uint32 base_tx_counter       # 底盘发出的数据包数
uint32 base_rx_counter       # 底盘收到的数据包数
uint32 base_rx_err_counter   # 底盘收到因校验失败丢弃的数据包数
uint32 tx_timeout_cnt
uint32 rx_timeout_cnt
uint32 int_cnt
uint32 reset_cnt
uint32 frame_err_cnt0
uint32 frame_err_cnt1
uint32 frame_err_cnt2
uint32 tx_cnt
uint32 rx_cnt
uint32 rx_err_cnt
uint8 tx_timeout
uint8 rx_timeout
uint8 single_timeout0
uint8 single_timeout1
uint8 single_timeout2
uint8 bus_bad0
uint8 bus_bad1
uint8 bus_bad2
uint8 correct_pct0
uint8 correct_pct1
uint8 correct_pct2
# important state to nav
uint8 motor_slip
uint8 locked_rotor_l
uint8 locked_rotor_r
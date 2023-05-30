# Header header
# CAN的 状态
# 结构体 _STATE
int8 state_total_onoff
int8 state_front_magnet_state
int8 state_back_magnet_state
int8 state_recorder_state
int8 state_over_voltage
int8 state_over_current
int8 state_pd_lock
# int8 state__reserve



# 结构体 POWER_RAW_INFO

int8 raw_total_current_l
int8 raw_total_current_h
int8 raw_chip_voltage_l
int8 raw_chip_voltage_h
int8 raw_temperature_l
int8 raw_temperature_h
int8 raw_tx_cnt


# 结构体 POWER_VER_INFO
# int8 ver_ver_h
# int8 ver_ver_s_l
# int8 ver_ver_s_h
# int8 ver_pdlock_errcode
# int8 ver_tx_cnt
uint32 version_app
uint32 version_boot
# int8 ver___reserve[3]

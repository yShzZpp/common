
Header header
# 底盘上报 can相关的信息
uint8 frame_cnt
uint8 motion_reset_code
uint8 drv_l_reset_code
uint8 drv_r_reset_code
uint8 energy_reset_code

uint8 ver_uart_protocol
uint8 ver_drv_l_protocol
uint8 ver_drv_r_protocol
uint8 ver_energy_protocol

# NAV_DRV_CAN_ERR drv_l_can_err;
uint8 L_C_CanErrorSum
uint8 L_C_CanErrorBR
uint8 L_C_CanErrorSTF
uint8 L_C_CanErrorFOR
uint8 L_C_CanErrorACK
uint8 L_C_CanErrorCRC
uint8 L_C_CanErrorOther
uint8 L_C_CanErrRes

# NAV_DRV_CAN_ERR drv_r_can_err;
uint8 R_C_CanErrorSum
uint8 R_C_CanErrorBR
uint8 R_C_CanErrorSTF
uint8 R_C_CanErrorFOR
uint8 R_C_CanErrorACK
uint8 R_C_CanErrorCRC
uint8 R_C_CanErrorOther
uint8 R_C_CanErrRes

# NAV_ENERGY_CAN_ERR energy_can_err;
uint8 energy_receive_cnt
uint8 energy_can_err1
uint8 energy_can_err2
uint8 energy_can_err3
uint8 energy_can_err4
uint8 energy_can_err5
uint8 energy_can_err6
uint8 energy_canVersion

int16 set_torque_l
int16 cur_torque_l
int16 set_torque_r
int16 cur_torque_r

uint16 drv_diff_l
uint16 drv_diff_r
uint16 cali_angle_diff_value_l
uint16 cali_angle_diff_value_r

#Header header

# Can 驱动轮的状态
# 结构体 VERSION_ST 
# int8 ver_h
# int8 ver_s_l
# int8 ver_s_h

 uint32 version_app
 uint32 version_boot

# 结构体 STATE_S1
int8 permit_enable
int8 enable_state
int8 can_receive_timeout
int8 pwm_period_error
int8 pwm_duty_error
int8 OtherMotorCanErr
int8 direction
# int8 __reserve1:2


# 结构体 ERROR_E1
int8 hard_overvoltage
int8 hard_overcurrent
int8 soft_overvoltage
int8 soft_overcurrent
int8 soft_undervoltage
int8 loss_phase
int8 hall_error
int8 locked_rotor

# 结构体 ERROR_E2
int8 undervoltage_limit
int8 overtempr_limit
# int8 __reserve2:6;

 # 结构体 ERROR_E3
uint8 HallStartErr
uint8 HallErrChange
uint8 EncodeStartErr
uint8 EncodeErrChange
uint8 HallError    

# 结构体 STATE_ST

int8 debug
# STATE_S1 s1
# ERROR_E1 e1
# ERROR_E2 e2
uint8 G_AutoTextResult
int8 hall_state
int8 CANVERSION


Header header

# 
# robot_base 节点收集运控上报的 传感器状态 信息 分装成BaseSensors.msg 发出来
# topic: /robot_base/sensors
# 
uint8 state_motor_clutch           # 离合状态 1 disable(失能) 2 enable(使能)
uint8 state_driving_wheel	       # 驱动轮 轮盘方向  0 底盘未校准   1 驱动轮朝正前方  2 驱动轮朝90度
uint8 state_plate	               # 抬升平台        0 底盘未校准   1 下降          2 抬升
uint8 state_touch                  # 车头触摸  伸缩 状态  目前未使用
uint8 state_emergency_off                # 急停按钮是否需被按下  0 未被按下 1 被按下  
uint8 state_proximity_switch       # 装箱用的接近开关     0 未触发  1 触发

uint8 state_hall_switch_l          # 装箱前霍尔     0 未触发  1 触发
uint8 state_hall_switch_r          # 装箱后霍尔     0 未触发  1 触发

int16 state_charge                 # 负数 未连接上     0 未充电    正数 正在充电
uint8 TYPE_NOCHARGE = 0            # 0 未在充电
uint8 TYPE_CHARGE_BY_HANDS = 1     # １:　手动电缆充电
uint8 TYPE_CHARGE_BY_AUTO = 2      # ２: 机器自助自动充电
uint8 state_charge_type
uint8 state_sensor_charge_f        # 充电桩前接近开关  0 未触发  1 触发
uint8 state_sensor_charge_b        # 充电桩后接近开关  0 未触发  1 触发
uint8 motor_slip                   # 电机打滑         0 未触发  1 触发
uint8 locked_rotor_l               # 电机堵转        0 未触发  1 触发
uint8 locked_rotor_r               # 电机堵转        0 未触发  1 触发
int8 state_carrying_box            # -1 不知道车上有无箱子  1 : 车上无箱子 2 : 车上有箱子 
# int8 base_vaild                    # 底盘是否正常工作(0 正常 非零异常)

uint8 motion_reboot                # 0 机器重启过了  1 机器没重启
uint8 imu_error                    # 0 imu正常  1 imu异常
uint8 type_of_box                  # 箱子类型 还没定义



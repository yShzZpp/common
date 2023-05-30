Header header

uint8 ctrl_mode     # 1  导航控制  2 : 手柄控制 3:app控制
uint8 ctrl_motor    # 1  disable  2 : enable
uint8 ctrl_LB_hold  # 遥控机LB一直按着
uint8 ctrl_driving_wheel    # 0 不响应  1:0     2 : 90  
uint8 ctrl_plate            # 0 不响应  1:下降   2 : 抬升
uint8 ctrl_touch			# 0 不响应  1:收缩   2 : 伸出
uint8 ctrl_QR_led           # 0 不响应  1:关灯   2 : 开灯
uint8 ctrl_rgb_led          # 未定义 led 彩灯
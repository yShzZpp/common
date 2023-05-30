# 命令
uint8 CLOSE = 0 #  command   关闭二维码识别功能
uint8 OPEN = 2  #  command   开启二维码识别功能



uint8 command 

int32 target_id               # 二维码ID
float32 yd_err              # 横向误差
float32 yaw_err             # 航向误差


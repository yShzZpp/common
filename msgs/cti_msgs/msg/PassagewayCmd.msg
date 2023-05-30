
# cmd 列表 
uint8 PASSAGEWAY_CMD_CONNECT_IOT_DEVICE = 0    # 连接指定ssid的IOT设备,只发一次
uint8 PASSAGEWAY_CMD_APPLY = 1                 # 申请通道: 要保持通道畅通,只发一次
uint8 PASSAGEWAY_CMD_FREE = 2                  # 释放通道
uint8 PASSAGEWAY_CMD_DISCONNECT_IOT_DEVICE = 3 # 断开指点ssid的IOT设备

# 发送给闸机控制模块的控制命令
uint8 cmd

string ssid

bool direction # 进入通道: true; 离开通道:false

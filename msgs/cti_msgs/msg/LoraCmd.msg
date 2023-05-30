string CMD_APPLY_LORA_DEVICE = apply
string CMD_FREE_LORA_DEVICE = free
string CMD_SEND_MSG = send
string cmd

#json格式
#CMD_APPLY_LORA_DEVICE:是lora设备的参数
#   example:
#   {"cmd": 1, "carrier_frequency": 427.5, "net_id": 141, "mode_id": 0, "encrypt_key":"", "decrypt_key":""}
#CMD_SEND_MSG:是发送的数据,默认就是发送json数据
string info 

string SOURCE_ELEVATOR = elevator
string SOURCE_SECURITY_GATE = security_gate
string SOURCE_WASH_FLOOR = wash_floor
string SOURCE_KNOCK_DOOR = knock_door
#此场景用于展会时的,虚拟电梯,并且开关门使用lora
string SOURCE_VIRTUAL_ELEVATOR = virtual_elevator
string source

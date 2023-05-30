# 低层上报当前车子上 驮着箱子信息info
# pub topic: /robot_base/carry_box_info
Header header
uint8 CAR_BOX_DISCONNECT = 0 # 车箱已经断开通信
uint8 CAR_BOX_CONNECTED  = 1 # 车箱已经链接上通信
uint8 carbox_comm   # 车箱通信是否连接上

uint32 box_type # 具体类型见Task.msg定义
string type_str

uint32 box_num            # 箱号
uint32 box_version        # 箱子版本号
uint32 box_fht_ver_app    # 箱子端烽火台模块APP版本号
uint32 box_fht_ver_boot   # 箱子端烽火台模块BOOT版本号
uint32 car_fht_ver_app    # 小车端烽火台模块APP版本号
uint32 car_fht_ver_boot   # 小车端烽火台模块版本号

uint8 voltage # 箱子电池电压 V
int8 current  # 箱子电池电流 A
uint8 RSOC    # 箱子电池剩余电量0~100%

uint8 STATUS_NOCHARG = 0 # 箱子没在充电
uint8 STATUS_CHARGING = 1 # 箱子已经在充电
uint8 charge_status # 箱子是否在充电
uint16 protect_status
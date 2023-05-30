string uuid
uint32 type     #事件类型
time timestamp  #事件时间戳

#箱rfid侦测事件
uint32 EVENT_TYPE_RFID_DETECT = 3      # 0x80001001
#箱rfid侦测参数
string rfid_tag       

#模糊停车事件
uint32 EVENT_TYPE_FUZZY_PARKING = 4
geometry_msgs/Pose park_pose

#找到二维码事件
uint32 EVENT_TYPE_FIND_ALL_QR = 5
string[] all_qr

#自动重定位事件 -- 新
uint32 EVENT_TYPE_AUTO_RELOCATE = 6
string building_name
string floor_name
geometry_msgs/Pose relocation_pose

#清洗机器人 切成手动模式
uint32 EVENT_TYPE_WASH_MANUAL_MODE = 7
#清洗机器人 切回自动模式
uint32 EVENT_TYPE_WASH_AUTO_MODE = 8

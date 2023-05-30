
# 电梯id
string device_id

# 电梯所在楼层
string floor

# 电梯门的状态
string DOORSTATE_OPENED  = Opened
string DOORSTATE_OPENING = Openning
string DOORSTATE_CLOSED  = Closed
string DOORSTATE_CLOSING = Closing
string DOORSTATE_UNKNOW  = Unknow
string door_state

# 电梯的行驶方向
string MOVEDIRECTION_STOP = Stop
string MOVEDIRECTION_UP   = Up
string MOVEDIRECTION_DOWN = Down
string MOVEDIRECTION_UNKNOW = Unknow
string move_direction

# 信息的来源
string SOURCE_4G   = 4G
string SOURCE_WIFI = WiFi
string SOURCE_LORA = Lora
string SOURCE_VIR_ELEVATOR = VirtualLift
string SOURCE_ELEVATOR_CLIENT = ElevatorClient
string information_source

# 信息的时间戳
uint64 timestamp
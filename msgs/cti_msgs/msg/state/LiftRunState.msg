
# state的状态列表:
uint8 LIFTRUNSTATE_ENTERING = 0   #进电梯过程中
uint8 LIFTRUNSTATE_ALLIN = 1      #robot完全进入电梯,并到达目标点
uint8 LIFTRUNSTATE_LEAVEING = 2   #离开电梯过程中
uint8 LIFTRUNSTATE_ALLOUT = 3     #robot完全离开电梯,并到达目标点
uint8 LIFTRUNSTATE_ENTERING_FAIL = 4   #robot进入电梯失败,无法到达目标位置
uint8 LIFTRUNSTATE_LEAVEING_FAIL = 5   #robot离开电梯失败,无法离开电梯

uint8 LIFTRUNSTATE_LEAVEING_OUT = 6 #robot 部分机体已经离开电梯门,但依然在向后退,提前发送关闭电梯门命令
uint8 LIFTRUNSTATE_ENTERING_IN = 7  #robot 部分机体已经进入电梯方位,但依然向目标点前进,提前发送关门电梯门命令

uint8 LIFTRUNSTATE_ALLIN_BUT_NOT_REACH_GOAL = 8 # robot 全部机体已经进入电梯,但还需要向目标点移动.

# 进出电梯模块发送的机器人进出电梯状态
uint8 state

#远程控制--请让一让，我要过去啦 //1
string SCENE_REMOTE_CONTROL_GETOUT_OF_THEWAY = RemoteControlGetOutOfTheWay
#远程控制--能帮我开开门吗 //2
string SCENE_REMOTE_CONTROL_OPEN_THEDOOR = RemoteControlOpenTheDoor
#远程控制--你挡住我了 //3
string SCENE_REMOTE_CONTROL_DONT_STAND_IN_MY_WAY = RemoteControlDontStandInMyWay
#远程控制--我在送快递 //4
string SCENE_REMOTE_CONTROL_EXPRESS_DELIVERY = RemoteControlExpressDelivery
#远程控制--请记得关箱门喔 //5
string SCENE_REMOTE_CONTROL_REMEMBER_TO_CLOSE_THE_DOOR = RemoteControlRememberToCloseTheDoor
#远程控制--请先帮我定位 //6
string SCENE_REMOTE_CONTROL_LOCATION = RemoteControlLocation
#远程控制--能帮我关上箱门吗 //7
string SCENE_REMOTE_CONTROL_CAN_YOU_CLOSE_THE_DOOR = RemoteControlCanYouCloseTheDoor
#远程控制--我要出电梯啦 //8
string SCENE_REMOTE_CONTROL_GETOUT_OF_THESTARIRS = RemoteControlGetOutOfTheStairs'

#开机
string SCENE_ROBOT_START = RobotStart 
#关机
#string SCENE_ROBOT_STOP = RobotPowOff

#定位成功
string SCENE_RELOCATION_SUCCESS = RelocationSuccess
#定位失败
string SCENE_RELOCATION_FAIL = RelocationFail
#识别二维码定位失败(原因: 机器距离二维码超过阀值),播放的提示语音
string SCENE_RELOCATION_CV_QR_OUTOF_THRESHOLD = SeekFailOutofThreshold

#低电量
string SCENE_LOW_POWER = LowPower

#起步  "我要出发了" 
string SCENE_BE_READY_TO_NAVIGATION = ReadyToNavigation 

#倒车  "我要倒车啦"            
string SCENE_BACKING = Backing
#左转
string SCENE_TURN_LEFT = TurnLeft
#右转             
string SCENE_TURN_RIGHT = TurnRight
#到达目标点        
string SCENE_REACH_GOAL = ReachGoal

#避障近       
string SCENE_NEAR_AVOIDANCE = NearAvoidance
#避障远    
string SCENE_FAR_AVOIDANCE = FarAvoidance

#侯梯
string SCENE_WAIT_ELEVATOR = WaitElevator
#电梯到达, 将要进电梯      
string SCENE_ENTER_ELEVATOR = EnterElevator  
#无法进入电梯时: "电梯满了,我坐下一趟"
string SCENE_NO_SPACE_LEAVE_ELEVATOR = GiveUpElevator
#机器乘坐电梯时,电梯在非目标楼层打开电梯门 "我要去N楼"
string SCENE_REACH_OTHER_FLOOR = IWantToGo 
#到达指定楼层, 出电梯  "N楼到了,我要出去"
string SCENE_LEAVE_ELEVATOR = OutElevator       
#机器被disable
string SCENE_DISABLE_ROBOT = HelpMe

#过闸机        
string SCENE_CROSS_GATE = ThroughGate
#过门
string SCENE_CROSS_DOOR = ThroughDoor
#过闸机或者门,出现问题
string SCENE_CROSS_FAIL = ThroughGateFail

#迎宾语句:久等了,请跟我来
string SCENE_PLEASE_FOLLOWME = PleaseFollowMe
#迎宾语句:欢迎来到坎德拉
string SCENE_WELCOME_CANDELA = WelcomeCandela

#运送垃圾过程中,定时播放"垃圾分类"语音
string SCENE_GARBAGE_CLASSIFICATION = GarbageClassification 

#急停按钮按下
string SCENE_PRESS_EMERGENCY_STOP = PressEmergencyStop

#急停按钮释放
string SCENE_RELEASE_EMERGENCY_STOP = ReleaseEmergencyStop

#远程控制--蜂鸣声
#string SCENE_BUZZING = Buzzing

#### 清洗机器人相关 ####
#开始自动清洗
string SCENE_START_AUTO_CLEAN = StartAutoClean
#开启手动清洗
string SCENE_START_MANUAL_CLEAN = StartManualClean
#结束手动清洗
string SCENE_FINISH_MANUAL_CLEAN = FinishManualClean
#清洗任务结束
string SCENE_FINISH_CLEAN_TASK = FinishCleanTask
#清洗避让
string SCENE_CLEAN_AVOIDANCE = CleanAvoidance

string appscene

string floor


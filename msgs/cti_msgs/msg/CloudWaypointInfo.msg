string id
string name
string floor      #楼层信息, 注意:CloudFloorInfo.msg里面没有使用该值, GetAllWayPoints.srv里面用到了该值
string gateId     #闸机的id
string elevatorId #电梯的id

geometry_msgs/Pose coordinate #坐标

uint32 WAYPOINT_TYPE_NORMAL=0		#普通点
uint32 WAYPOINT_TYPE_BAY=1		    #车位
uint32 WAYPOINT_TYPE_HUB=2		    #驿站
uint32 WAYPOINT_TYPE_DOCK=3		    #桩
uint32 WAYPOINT_TYPE_GATE=4		    #闸机
uint32 WAYPOINT_TYPE_INTERCHANGE=5	#对接点
uint32 WAYPOINT_TYPE_HOLD=6		    #等待点
uint32 WAYPOINT_TYPE_PARKING=7		#停靠点
uint32 WAYPOINT_TYPE_HOME=9		    #重定位点
uint32 WAYPOINT_TYPE_RESCUE=10      #救援点,当机器空闲并且发现无法与平台通信上/当前机器信号非常查,则自动行驶到救援点(默认是一层楼中信号最好的那个点),
uint32 WAYPOINT_TYPE_TRANSIT=11		#中转点
uint32 WAYPOINT_TYPE_DOCK_CHARGE=12  #充电桩
uint32 WAYPOINT_TYPE_DOCK_NORMAL=13  #普通桩
uint32 WAYPOINT_TYPE_DOCK_WALL=14    #贴墙桩
uint32 WAYPOINT_TYPE_LIFT=15		 #电梯点
uint32 WAYPOINT_TYPE_AVOID=16		 #避梯点
uint32 WAYPOINT_TYPE_FRONT_ELEVATOR = 601 #电梯前的点  #导航内部使用
uint32 WAYPOINT_TYPE_FRONT_DOCK     = 602 #桩前的点   #导航内部使用
uint32 WAYPOINT_TYPE_OTHER=1000		#其他点

uint32 waypointType	#路点类型

uint32 SCENARIO_TYPE_LOGISTICS=1	    #收派
uint32 SCENARIO_TYPE_DEPOSIT=2		    #投柜
uint32 SCENARIO_TYPE_STERILIZATION=3	#消毒
uint32 SCENARIO_TYPE_TRANSPORT=4	    #转运
uint32 SCENARIO_TYPE_DISPATCH=5		    #共享
uint32 SCENARIO_TYPE_CLEANING=6         #清扫
uint32 SCENARIO_TYPE_GARBAGE=7          #垃圾

uint32[] scenarioType	#场景类型

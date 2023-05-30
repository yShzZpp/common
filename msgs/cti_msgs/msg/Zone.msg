
uint32 id           # 区域编号
string place        # 区域地点名
# 0 没指定属性
uint32 PROPERTY_CLEARING = 1        # 清理区域
uint32 PROPERTY_NARROW = 2          # 限行区域
uint32 PROPERTY_CORRIDOR = 3        # 长直走廊区域
uint32 PROPERTY_SKIDDING = 4        # 易打滑区域
uint32 PROPERTY_FORBIDDEN = 5       # 禁行区域
uint32 PROPERTY_HIGHSPEED = 6       # 快速区域
uint32 PROPERTY_RING_ISLAND = 7     # 环形岛区域
uint32 PROPERTY_CLEARING_PATH = 8   # 清理路线 #专门给洗地的特例, 后续所有的路线信息在PathInfo.msg中 
uint32 PROPERTY_LOWSPEED = 9   	    # 低速行驶区域

uint32 property     # 区域属性
geometry_msgs/Pose[] anchors  #区域锚点列表

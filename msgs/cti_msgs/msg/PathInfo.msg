uint32 id           # 路线编号
string place        # 路线名称

# 0 没指定属性
uint32 PROPERTY_CLEARING = 1        # 清洗路线

uint32 property     # 路线属性
geometry_msgs/Pose[] anchors  #路线点列表

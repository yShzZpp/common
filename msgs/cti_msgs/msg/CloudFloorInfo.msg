string id                  # uuid
string name                # 中文名字
string floor               # 字符串 eg 26
string full_name           # buildingName_floorName eg. wankeyuncheng_7B_26
string owned_building      # 所属大楼, 内容为代表地图的名称, 如wankeyuncheng_7B
string[] connectedFloorIds # 与当前大楼当前楼层相连的其他大楼楼层uuid集合

bool allowInterchange
cti_msgs/CloudGateInfo[] gates
cti_msgs/CloudWaypointInfo[] waypoints
cti_msgs/CloudDockInfo[] docks

cti_msgs/CloudFloorPathInfo[] paths
cti_msgs/CloudFloorZoneInfo[] zones

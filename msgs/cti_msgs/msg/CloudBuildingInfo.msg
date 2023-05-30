string id                     #uuid
string name                   #中文名字
string name2                  #代表地图的字符串

string hubId                  #园区驿站uuid
string[] connectedBuildingIds #与当前大楼相连的其他大楼uuid集合

cti_msgs/CloudFloorInfo[] floors
cti_msgs/CloudElevatorInfo[] elevators

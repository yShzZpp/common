uint8 SELF_LOCATION_SUCCESS = 0
uint8 SELF_LOCATION_FAIL = 1
uint8 result

string building
string floor
geometry_msgs/Pose pose

##### 一些定位方法需要额外上传的数据
string charge_pile_uuid

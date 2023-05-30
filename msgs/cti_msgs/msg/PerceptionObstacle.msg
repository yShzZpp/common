# obstacle ID
Header header

int32 id

# obstacle type
# 0 - 未知
# 1 - 行人
# 2 - 小汽车
# 3 - 大汽车（卡车、公交）
# 4 - 非机动车（电动车、自行车）
# 5 - 烛光机器人
int8 type

# obstacle position in the map coordinate system.
geometry_msgs/Pose pose
# obstacle velocity
geometry_msgs/Vector3 velocity

# Point Cloud 
sensor_msgs/PointCloud2 pointcloud

# Size of obstacle bounding box.
float64 length
float64 width
float64 height

# obstacle corner points.
geometry_msgs/Polygon polygon

# obstacle trajectory
geometry_msgs/Pose[] trajectory

# LIDAR_2D = 0,
# LIDAR_8 = 1,
# LIDAR_16 = 2,
# LIDAR_32 = 3,
# LIDAR_40 = 4,
# RADAR = 5,
# CAMERA = 6,
# ULTRASONIC = 7,
# UNKNOWN_SENSOR_TYPE = 10,
int8 sensor_type

float64 reliability

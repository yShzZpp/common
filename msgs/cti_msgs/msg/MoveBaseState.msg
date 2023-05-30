std_msgs/Header header

int8 MOVE_BASE_UNKNOWN = -1

# move_base  正在规划全局路径
int8 MOVE_BASE_PLANNING = 0
# move_base  正在跟随全局路
int8 MOVE_BASE_CONTROLLING = 1
# move_base  正在Recovery
int8 MOVE_BASE_CLEARING = 2


# move_base 节点执行的状态
int8 state

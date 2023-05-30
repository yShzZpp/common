
# 多功能洗涤扫地机器人 工作模式状态
# topic : /wash_base/work_mode_state

# 对工作模式执行结果
uint8 STATE_NONE = 0             # 无动作
uint8 STATE_ENTER_SUCCESS = 1    # 进入成功
uint8 STATE_ENTER_FAIL = 2       # 进入失败
uint8 STATE_ENTER_ING = 3        # 进入过程中

uint8 STATE_EXIT_SUCCESS = 4     # 退出成功
uint8 STATE_EXIT_FAIL = 5        # 退出失败
uint8 STATE_EXIT_ING = 6         # 退出过程中

uint8 sweeper             #扫地模式状态
uint8 scrubber            #洗地模式状态
uint8 dump_garbage        #倒垃圾模式状态
uint8 sewage              #排污模式状态
uint8 self_clean          #污水箱自清洁模式状态
uint8 butt_ws             #进入工作站状态状态

uint8 speed_brush_l             #左边刷电机速度转速
uint8 speed_brush_r             #右边刷电机速度转速
uint8 speed_brush_roll          #滚刷电机速度转速
uint8 speed_brush_disc          #盘刷电机速度转速
uint8 travel_brush_disc         #总成推杆行程百分比

uint8 clean_box_level           #清水液位档位
uint8 sewage_box_level          #污水液位档位
uint8 garbage_box_level         #垃圾箱档位
# 清水液位档位[0-5]
# 污水液位档位[0-5]
# 垃圾箱档位[0-1]

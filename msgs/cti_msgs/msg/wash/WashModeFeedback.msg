
# 多功能洗涤扫地机器人 模式状态反馈
# topic : /wash_base/wash_mode_feedback

# 对action执行结果
uint8 ACTION_RESULT_NONE = 0             # 无动作
uint8 ACTION_RESULT_ENTER_SUCCESS = 1    # 进入成功
uint8 ACTION_RESULT_ENTER_FAIL = 2       # 进入失败
uint8 ACTION_RESULT_ENTER_ING = 3        # 进入过程中

uint8 ACTION_RESULT_EXIT_SUCCESS = 4     # 退出成功
uint8 ACTION_RESULT_EXIT_FAIL = 5        # 退出失败
uint8 ACTION_RESULT_EXIT_ING = 6         # 退出过程中

uint8 result_sweeper          #扫地模式
uint8 result_scrubber         #洗地模式
uint8 result_polish           #抛光模式
uint8 result_mop              #拖地推尘模式
uint8 result_dump_garbage     #倒垃圾模式
uint8 result_change_module    #换模块模式
uint8 result_sewerage         #排污模式
uint8 result_self_clean       #污水箱自清洁模式

# 反馈: 当前机器自动模式还是手动模式
uint8 OPERATION_MODE_NULL = 99       # 
uint8 OPERATION_MODE_AUTOMATIC = 0  # 自动模式
uint8 OPERATION_MODE_MANUAL = 1     # 手动模式
uint8 current_operation_mode

############### 以下 通用状态  ################
# 反馈 :清洁模块
uint8 ACTUATOR_MODULE_NULL =        0 
uint8 ACTUATOR_MODULE_SCRUBBER =    1   # 洗地模块
uint8 ACTUATOR_MODULE_POLISH =      2   # 抛光模块
uint8 ACTUATOR_MODULE_MOP =         3   # 拖地推尘模块
uint8 actuator_module 

# 反馈 :清洁箱水位
uint8 CLEAN_BOX_WARER_LEVEL_0 = 0   # 0%
uint8 CLEAN_BOX_WARER_LEVEL_1 = 10  # 10%
uint8 CLEAN_BOX_WARER_LEVEL_2 = 30  # 30%
uint8 CLEAN_BOX_WARER_LEVEL_3 = 50  # 50%
uint8 CLEAN_BOX_WARER_LEVEL_4 = 70  # 70%
uint8 CLEAN_BOX_WARER_LEVEL_5 = 100 # 100%
uint8 clean_box_water_level

# 反馈 :污水箱水位
uint8 SEWAGE_BOX_WARER_LEVEL_0 = 0      # 0%
uint8 SEWAGE_BOX_WARER_LEVEL_1 = 10     # 10%
uint8 SEWAGE_BOX_WARER_LEVEL_2 = 50     # 50%
uint8 SEWAGE_BOX_WARER_LEVEL_3 = 100    # 100%
uint8 sewage_box_water_level

# 反馈 :抛光液液位
uint8 POLISH_WATER_LEVEL_0 = 0      # 0%
uint8 POLISH_WATER_LEVEL_1 = 10     # 10%     
uint8 POLISH_WATER_LEVEL_2 = 50     # 50%
uint8 POLISH_WATER_LEVEL_3 = 100    # 100%
uint8 polish_level

uint8 putter_position_percent  # 总成推杆行程百分比 [0 100]

# 反馈 :当前垃圾箱容量
uint8 GRABAGE_BOX_STATE_EMPTY = 0       # 未满
uint8 GRABAGE_BOX_STATE_FULL = 1        # 已满
uint8 garbage_box_state

#  反馈 边刷速度  滚刷速度
# uint8 SIDE_ROLLER_BRUSH_STOP = 0         # 停止
# uint8 SIDE_ROLLER_BRUSH_MAX_SPEED = 100  # 全速
uint8 side_brush_l_speed                   # 左边刷速度
uint8 side_brush_r_speed                   # 右边刷速度
uint8 roll_brush_speed			   # 滚刷速度

# 反馈 :盘刷电机档位
uint8 DISC_BRUSH_SPEED_LEVEL_0 = 0  # 0档位
uint8 DISC_BRUSH_SPEED_LEVEL_1 = 1  # 1档位
uint8 DISC_BRUSH_SPEED_LEVEL_2 = 2  # 2档位
uint8 DISC_BRUSH_SPEED_LEVEL_3 = 3  # 3档位
uint8 DISC_BRUSH_SPEED_LEVEL_4 = 4  # 4档位
uint8 disc_brush_speed_level

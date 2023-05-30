

# 多功能洗涤扫地机器人 模式设置
# topic : /wash_base/wash_mode_control

uint8 ACTION_NONE = 0
uint8 ACTION_ENTER = 1              # 进入模式
uint8 ACTION_EXIT =  2              # 退出模式
uint8 action_sweeper           # 扫地模式
uint8 action_scrubber          # 洗地模式
uint8 action_polish            # 抛光模式
uint8 action_mop               # 拖地推尘模式
uint8 action_dump_garbage      # 倒垃圾模式
uint8 action_change_module     # 换模块模式
uint8 action_sewerage          # 排污模式
uint8 action_self_clean        # 污水箱自清洁模式

# 边刷和滚刷推杆行程
uint8 SIDE_ROLLER_BRUSH_RANGE_ZEOR = 0     # 完全收回
# uint8 SIDE_ROLLER_BRUSH_RANGE_MAX = 100  # 完全推出
uint8 SIDE_ROLLER_BRUSH_RANGE_STOP = 255   # 0xFF停止
uint8 side_brush_l_range                   # 左边刷
uint8 side_brush_r_range                   # 右边刷
uint8 roll_brush_range                     # 滚刷
uint8 brush_disc_range                     # 总成推杆行程百分比

# 边刷和滚刷电机速度
uint8 SIDE_ROLLER_BRUSH_STOP = 0         # 停止
uint8 SIDE_ROLLER_BRUSH_MAX_SPEED = 100  # 全速
uint8 side_brush_l_speed                   # 左边刷速度
uint8 side_brush_r_speed                   # 右边刷速度
uint8 roll_brush_speed			           # 滚刷速度


# 盘刷电机速度控制
uint8 DISC_BRUSH_SPEED_LEVEL_0 = 0  # 0档位
uint8 DISC_BRUSH_SPEED_LEVEL_1 = 1  # 1档位
uint8 DISC_BRUSH_SPEED_LEVEL_2 = 2  # 2档位
uint8 DISC_BRUSH_SPEED_LEVEL_3 = 3  # 3档位
uint8 DISC_BRUSH_SPEED_LEVEL_4 = 4  # 4档位
uint8 disc_brush_speed_level

# 清洁水量控制
uint8 CLEAN_BOX_WARER_LEVEL_0 = 0   # 0档位
uint8 CLEAN_BOX_WARER_LEVEL_1 = 1   # 1档位
uint8 CLEAN_BOX_WARER_LEVEL_2 = 2   # 2档位
uint8 CLEAN_BOX_WARER_LEVEL_3 = 3   # 3档位
uint8 CLEAN_BOX_WARER_LEVEL_4 = 4   # 4档位
uint8 clean_box_water_level

# 抛光液液
uint8 POLISH_WATER_LEVEL_0 = 0  # 0档位
uint8 POLISH_WATER_LEVEL_1 = 1  # 1档位
uint8 POLISH_WATER_LEVEL_2 = 2  # 2档位
uint8 POLISH_WATER_LEVEL_3 = 3  # 3档位
uint8 polish_level

# 更换模块
uint8 ACTUATOR_MODULE_NULL =        0 
uint8 ACTUATOR_MODULE_SCRUBBER =    1 # 洗地模块
uint8 ACTUATOR_MODULE_POLISH =      2 # 抛光模块
uint8 ACTUATOR_MODULE_MOP =         3 # 拖地推尘模块
uint8 actuator_module 



# 多功能洗涤扫地机器人 工作模式设置
# topic : /wash_base/work_mode_control

uint8 ACTION_NONE = 0
uint8 ACTION_ENTER = 1              # 进入模式
uint8 ACTION_EXIT =  2              # 退出模式
uint8 ACTION_FORCE_STOP = 3         # 异常强制推出模块
# 模式动作
uint8 action_sweeper           # 扫地模式
uint8 action_scrubber          # 洗地模式
uint8 action_dump_garbage      # 倒垃圾模式
uint8 action_sewerage          # 排污模式
uint8 action_self_clean        # 污水箱自清洁模式
uint8 action_goto_workstation  # 进入工作站

# 参数
uint8 speed_brush_l     # 左边刷电机速度百分比[0 - 100]
uint8 speed_brush_r     # 右边刷电机速度百分比[0 - 100]
uint8 speed_brush_roll  # 滚刷电机速度百分比[0 - 100]
uint8 speed_brush_disc  # 盘刷电机速度百分比[0 - 100]
uint8 travel_brush_disc # 总成推杆行程百分比[0 - 100]
uint8 speed_clean_water # 清水流量控制百分比[0 - 100]

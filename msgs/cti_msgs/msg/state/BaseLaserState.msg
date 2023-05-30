
#Header header

#底盘运控上报的数据
# uint32_t Laser_dis[4];
uint32[] Laser_dis
# uint8 state[4];//有东西，没东西，检测到台阶
uint8[] ground_det_invalid  # 0 对地传感器有效 1 对地传感器出现异常无效
uint8[] foot_detected # 0 传感器正常 1 传感器检测大脚
uint8[] step_detected # 0 传感器正常 1 传感器检测到台阶悬崖
uint8[] head_ultrs    # 0 头顶超声波未触发 1 0 头顶超声波触发了

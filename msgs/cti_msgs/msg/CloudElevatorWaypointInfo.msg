string floor
string[] tags

bool reverseFlag				# 电梯进出反向标志，当前楼层与目标楼层不一致时，需要改进出电梯模式为前进前出

geometry_msgs/Pose inside

geometry_msgs/Pose out          # 电梯外的点，若点有效，代表该点为额外采集的点，否则需要配合【偏移】和【反向标志进】行换算
geometry_msgs/Pose avoid        # 躲避点 ……
geometry_msgs/Pose wait         # 等待点 ……

float64 outPoseOffsetV			# 当前楼层【电梯外的点】相对内部点的预设垂直偏移距离
float64 avoidPoseOffsetV        # 当前楼层【躲避点】相对内部点的预设垂直偏移距离
float64 waitPoseOffsetV         # 当前楼层【等待点】相对内部点的预设垂直偏移距离
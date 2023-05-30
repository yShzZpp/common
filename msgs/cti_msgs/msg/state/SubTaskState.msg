
#子任务状态
uint32 STATE_PROCESSING = 0 #正在执行中
uint32 STATE_COMPLETED = 1  #任务已完成
uint32 STATE_PAUSED    = 2  #任务被暂停
uint32 STATE_ABORTING  = 3  #任务正在被中止
uint32 STATE_ERROR     = 4  #任务出错
uint32 STATE_START_TASKLIST = 10 #任务链开始执行 -- task manager使用,平台可不用关注
uint32 state

#当前最新的导航状态
uint32 newest_robot_state

#错误信息
string errReason

#子任务的id
string task_id
#任务组的id
string task_group_id
#任务组的上下文呢
string task_group_context

#任务通过web api对外发送的任务进展 
#task_relevant.proto 下面的 cti::task_relevant::TaskList格式
# string task_lists_state
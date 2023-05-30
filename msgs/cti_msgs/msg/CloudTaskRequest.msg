
#发布者生成,uuid形式的字符串,这个值同时代表着请求Id，响应部分与之匹配
string group_id

#任务上下文
string group_context

#时间戳
uint64 timestamp

#下发的命令
uint32 ACTION_EXEC                 = 0  #串行执行任务链中的任务
uint32 ACTION_PAUSE                = 1  #暂停当前任务链中正在执行的任务,后续任务同步等待
uint32 ACTION_RESUME               = 2  #恢复当前任务链中正在执行的任务
uint32 ACTION_ABORT_ALL            = 3  #中止当前的任务链中正在执行的任务,并放弃后续所有任务
uint32 ACTION_ABORT_ONE            = 4  #中止当前的任务链中正在执行的任务,然后执行任务链中的下一个任务

#目前先内部使用,针对的是task list
uint32 ACTION_INSET_TASKS          = 5  #中止当前正在执行的任务,执行新的任务链表.
                                        #被中止的任务+未执行的任务链表放到新的任务链表之后(视具体业务场景)
#针对sub task list                                       
# uint32 ACTION_REPLACE_CURRENT_TASK = 6  #中止当前正在执行的任务,执行新的任务.
#                                         #未执行的任务链表放到新的任务链表之后(视具体业务场景)
uint32 action

#任务琏
cti_msgs/Task[] tasklist

#以下参数专门用于从充电桩上退出
# true: 带箱子退出
# false: 不带箱子退出
bool with_box


uint8 TASK_SUCCESS = 0  #导航任务执行成功
uint8 TASK_FAIL = 1     #执行失败
uint8 TASK_ABORT_SUCCESS = 2  #中止命令执行成功
uint8 TASK_ABORT_FAIL = 3     #中止命令执行失败

uint8 state

uint8 cmd_source

uint32 outcome #故障类型
string message #故障原因
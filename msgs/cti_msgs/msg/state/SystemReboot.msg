
# 系统重启
# topic : /system_reboot
uint8 TYPE_NO_CMD = 0
uint8 TYPE_SOFT_RESET_DRIVER = 1   # 驱动板软复位
uint8 TYPE_SOFT_RESET_MOTION = 2   # 运控板软复位
uint8 TYPE_SOFT_RESET_BASE = 3     # 底盘(驱动+运控)全部都软复位
uint8 TYPE_HARD_RESET_MACHINE = 4  # 整机硬复位(=断电重启)
uint8 TYPE_REBOOT_ENERGY_BOARD = 5 # 软件重启电源板
uint8 TYPE_DORMANCY = 6            # 机器休眠
uint8 TYPE_SHUTDOWN = 7            # 机器断电关机 不起来

uint8 reboot_type

uint32 delay_second  # 以秒为单位, dormancy/reboot指令延迟delay_sencond 秒后执行
uint32 dormancy_time # 以分为单位, 机器休眠 dormancy_time 分钟后自动启动

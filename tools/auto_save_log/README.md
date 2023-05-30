## 自动记录日志

### 维护(重中之重)
后续有人新增需求或者修改故障代码，一定记得把修改代码后的测试方法(正常/异常用例)更新到**《测试用栗》**，保证用栗**全部pass**后，才能将该版本交给别人或者放到工控机上运行


### 使用说明
需要修改配置文件`cti_record_config.yaml`

1. 将`all_log_path`和`error_log_path`修改成工控机的目录(或者你的本机目录)
2. 可添加`topics`下面的故障类型，但需要修改monitor.py，增加故障触发topic的callback函数
3. 编译时依赖`cti_msgs`模块
4. 启动时，执行`roslaunch cti_record_log record.launch`

**请结合下面的说明，详细阅读`cti_record_config.yaml`配置文件**

### 节点
1. 节点cti_reord_log负责记录日志

2. 节点cti_monitor_log负责监控日志的大小，接收故障触发topic后过滤/拷贝错误日志

### 机制

#### 全topics收集
节点cti_reord_log负责

将配置文件yaml中的所列举的topics，通过rosbag命令记录到`all_log_path`中

#### 监控文件夹大小
节点cti_monitor_log负责

一个定时器负责监控日志大小，超过阀值就将最旧的bag删除  

只监控`all_log_path`目录，而没有监控`error_log_path`目录

#### 故障topics收集
节点cti_monitor_log负责

订阅故障topic，解析，当出现故障时，暂停定时器，等待一个bag的时长用于收集出错后的日志，将all topics过滤/拷贝到error路径下，恢复定时器。

##### 拷贝
是将`all_log_path`目录下最新的`error_log_num`个bag都拷贝到`error_log_path`目录

在一段时间内`(interval_time * error_log_num * 60)`,只在最开始的故障发生时，执行拷贝操作

##### 过滤
是将`all_log_path`目录下最新的error_log_num个bag进行过滤，只拷贝该故障需要的topics形成bag到`error_log_path`目录

在一段时间内`(interval_time * error_log_num * 60)`,对每个故障类型，只在该故障类型最开始的故障发生时，执行拷贝操作


### 备注
1. 配置文件`cti_record_config.yaml`中的`bag_max_num`尚未使用，预留
2. 设置的日志大小阀值取决于所抓取的topics内容的多少，但不要设置太小，通常要大于20G，防止出现日志目录中只有一个bag的情况
3. 如果使用编译选项带`install`并且设置环境变量`install/setup.bash`，那么修改配置文件`cti_record_config.yaml`后也要执行编译命令
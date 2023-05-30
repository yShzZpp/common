
#发布者随机生成,uuid形式的字符串
string id

#对应请求的消息Id,有效的消息Id,当前消息为一个响应，这个值与相应请求id一致。
string requestId

#时间戳
uint64 timestamp

#对应请求的action,与请求的action保持一致,见ChargeRequest.msg
uint32 action

#命令是否接受
#true 接受命令并开始执行
#false 拒绝执行action命令，拒绝原因参见message
bool result

#命令被拒绝原因
string message



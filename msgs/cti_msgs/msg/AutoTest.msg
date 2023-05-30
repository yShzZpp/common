

uint32 TEST_TYPE_MOUNT = 0   #装卸箱自动测试
uint32 TEST_TYPE_CHARGE = 1  #充电自动测试
uint32 TEST_TYPE_PLAYSOUND = 2  #不断播放随机音频文件
uint32 TEST_TYPE_CHANGMAP = 3 #不断执行切换地图命令
uint32 test_type

string box_id #箱子的id,用来查询箱子是否充上电
string box_qr #箱子的二维码id,
string charge_qr #充电桩的二维码id

uint32 test_times

string rebuilding
string refloor_1
string refloor_2
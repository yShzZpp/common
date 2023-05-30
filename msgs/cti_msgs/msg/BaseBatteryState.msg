
#Header header

#底盘运控上报的数据
#电池状态 
uint16 all_vol      # 单位10mV 总电压 
int16 current       # 单位10mA 电流 充电为正，放电为负
uint16 remainning_cap  #剩余容量  单位10mAh
uint16 nominal_cap
uint16 circulation_cnt
uint16 produce_date
uint16 equilibrium
uint16 equilibrium_high
uint16 protect_status
uint8 software_ver
uint8 RSOC #剩余容量百分比 
uint8 FET_ctrl_state #bit0 表示充电，bit1 表示放电，0 表示 MOS 关闭，1 表示打开 
uint8 strings_num
uint8 NTC_num
uint16 NTC_data1
uint16 NTC_data2
uint8  producer
uint16[] single_mv
uint8[]  hardware_version  #string like: M061-09, fill with blank

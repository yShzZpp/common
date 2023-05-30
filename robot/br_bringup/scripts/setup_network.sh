#!/bin/bash

# sudo systemctl disable ModemManager.service
# sleep 1
# ps -ef|grep ModemManager | grep -v grep|awk '{print $2}' | xargs sudo kill -9
# sleep 1

# 常量定义
CONST_CART_TYPE_HUAWEI=0      # 0:华为模块 
CONST_CART_TYPE_DINGQIAO=1    # 1: 鼎桥模块
CONST_CART_TYPE_UNKNOW=100    # 100 未知模块


# g_* 全局变量
g_Usb_Port=""   # 操作的端口 [/dev/ttyUSB0 /dev/ttyUSB1 /dev/ttyUSB2 /dev/ttyUSB3]
g_Cart_Type=""  # 4G卡类型   [  ]
g_Cart_Name=""  # 4G卡名     [  ]
g_Workdir=$(cd $(dirname $0); pwd)
g_Dialup_File=$g_Workdir"/dial_up.py"    # 脚本测试路径
# g_Dialup_File="/opt/cti/kinetic/share/br_bringup/scripts/dial_up.py"  # 脚本

ping_flag=1
ping_cnt=0

# 1 检测网卡类型 目前支持华为4G/鼎桥 两种网卡
function detect_NetCardType()
{
	# 网卡前缀
	huawei_prefix="wwp"
	# huawei_prefix="eno"
	dingqiao_prefix="enxac"

	# 循环18次
	for i in {1..18}
	do
		# 检测华为模块 网卡
		huawei_card=`ifconfig -a | grep $huawei_prefix | awk '{print $1}'`
		echo "000 $huawei_card"
		if [ -n "$huawei_card" ];then
			g_Cart_Type=$CONST_CART_TYPE_HUAWEI
			g_Cart_Name=$huawei_card
			g_Cart_Name=${g_Cart_Name%?} # 去掉末尾的:冒号
			break
		fi

		# 检测鼎桥模块 网卡
		dignqiao_card=`ifconfig -a | grep $dingqiao_prefix | awk '{print $1}'`
		if [ -n "$dignqiao_card" ];then
			g_Cart_Type=$CONST_CART_TYPE_DINGQIAO
			g_Cart_Name=$dignqiao_card
			g_Cart_Name=${g_Cart_Name%?} # 去掉末尾的:冒号
			echo	"1111"
			break
		fi

		# 等待3秒
		sleep 3
	done
	echo "找到网卡名 Name: $g_Cart_Name  ，网卡类型 : $g_Cart_Type"
}

function detect_NetCardType3()
{
	for i in {1..18}
	do
		huawei_card=`lsusb | grep 12d1:15c1`
		if [ -n "$huawei_card" ];then

			g_Cart_Type=$CONST_CART_TYPE_HUAWEI

			card_name=`dmesg | grep 'renamed from' | grep cdc | tail -n1 | awk '{print $5}'`
			g_Cart_Name=${card_name%?} # 去掉末尾的:冒号
		break
		fi

		# 检测鼎桥模块 网卡
		dignqiao_card=`lsusb | grep 2ecc:3010`
		if [ -n "$dignqiao_card" ];then

			g_Cart_Type=$CONST_CART_TYPE_DINGQIAO

			card_name=`dmesg | grep 'renamed from' | grep rndis | tail -n1 | awk '{print $5}'`
			g_Cart_Name=${card_name%?} # 去掉末尾的:冒号

			break
		fi
		# 等待3秒
		sleep 3
	done
}

function detect_NetCardType2()
{
	# 网卡前缀
	# huawei_prefix="enx"
	huawei_prefix="eno"
	dingqiao_prefix="enxac"

	# 循环18次
	for i in {1..18}
	do
		# 检测华为模块 网卡
		huawei_card=`dmesg | grep 'renamed from' | grep cdc | tail -n1 | awk '{print $2}'`
		if [ -n "$huawei_card" ];then

			g_Cart_Type=$CONST_CART_TYPE_HUAWEI

			card_name=`dmesg | grep 'renamed from' | grep cdc | tail -n1 | awk '{print $4}'`
			g_Cart_Name=${card_name%?} # 去掉末尾的:冒号
		break
		fi

		# 检测鼎桥模块 网卡
		dignqiao_card=`dmesg | grep 'renamed from' | grep rndis | tail -n1 | awk '{print $2}'`
		if [ -n "$dignqiao_card" ];then

			g_Cart_Type=$CONST_CART_TYPE_DINGQIAO

			card_name=`dmesg | grep 'renamed from' | grep rndis | tail -n1 | awk '{print $4}'`
			g_Cart_Name=${card_name%?} # 去掉末尾的:冒号

			break
		fi

		# 等待3秒
		sleep 3
	done
	echo "找到网卡名 Name: $g_Cart_Name  ，网卡类型 : $g_Cart_Type"
}

# 2 检测USB端口
function detect_Port()
{
	# 循环18次
	for i in {1..18}
	do
		# 端口
		if [ $g_Cart_Type = $CONST_CART_TYPE_HUAWEI ];then
			echo "华为模块"
			ttyUSB=`dmesg | grep 'GSM modem (1-port) converter now attached to' | tail -n 3| head -n 1| awk '{print $NF}'`
		else
			echo "鼎桥模块"
			ttyUSB=`dmesg | grep 'GSM modem (1-port) converter now attached to' | tail -n 1 | awk '{print $NF}'`
		fi

		# 检测端口是否成功
		if [ -n "$ttyUSB" ];then
			g_Usb_Port="/dev/"$ttyUSB
			break
		fi
		# 等待3秒
		sleep 3
	done

	chmod 666 $g_Usb_Port
	echo "检测到 ttyUsb 端口: $g_Usb_Port"
}

# 开始拨号上网
function start_Dialup()
{
	echo "开始拨号中..."
	echo "执行命令：python $g_Dialup_File $g_Usb_Port $g_Cart_Type"
	python $g_Dialup_File $g_Usb_Port $g_Cart_Type $g_Cart_Name
	echo "拨号执行结束中..."
}

# 重新获取IP 地址
function obtain_IP()
{
	echo "获取IP地址... $g_Cart_Name"
	sudo dhclient -4 -v -nw $g_Cart_Name
	echo "拨号完成"
}

function ping_Network()
{
    #超时时间
    local timeout=1

    #目标网站
    local target=www.baidu.com
    #获取响应状态码
    local ret_code=`curl -I -s --connect-timeout ${timeout} ${target} -w %{http_code} | tail -n1`

    if [ "x$ret_code" = "x200" ]; then
        echo "网络畅通"
		ping_flag=1
		ping_cnt=0
		return 1
    else
        echo "网络不畅通"
		ping_flag=0
		ping_cnt=$(($ping_cnt+1))
        return 0
    fi
}

function hadleModemManager()
{
	status=`service ModemManager status | grep running`
	if [ -n "$status" ];then
		echo "ModemManager Running "
		echo "Execute systemctl disable ModeeManager.service "
		systemctl disable ModemManager.service
		# echo "Reboot "
		sleep 10
		reboot
	else
		echo "ModemManager Stop"
	fi
}

function chmode_file()
{
	chgrp cti /home/cti/.ros/network_info.json
	chown cti /home/cti/.ros/network_info.json
}


################################################################
# 主程序流程 
# 等待20秒 让系统能有足够时间检测到网卡 usb端口
hadleModemManager

sleep 1

# 1 检测网卡
detect_NetCardType3
# detect_NetCardType2

# 2 根据网卡类型 检测usb端口
detect_Port

# 3 开始拨号上网
start_Dialup

# 4 开始拨号上网
obtain_IP

# 5 开始拨号上网
ping_Network

chmode_file

sleep 10

while true
do
	sleep 10

	date=`date +%Y%m%d-%H:%M:%S`
	echo $date
	echo "检测网络..."
	ping_Network
	sleep 1
	if [ $ping_cnt -ge "3" ]; then
		echo "尝试重连中..."
		ping_cnt=0
		start_Dialup
		obtain_IP
	fi
done

exit 0

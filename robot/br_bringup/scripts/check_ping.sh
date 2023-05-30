#! /bin/bash

echo "check ping..."

date=`date +%Y%m%d-%H:%M:%S`
net_ip="www.baidu.com www.taobao.com www.qq.com"
#net_ip="192.168.3.8 192.168.3.7 192.168.3.3 192.168.10.238"

function Ping_IP()
{
	ping_data=`ping -A -c $1 $2 |grep "rtt\|received"`	

	rece_pak=`echo $ping_data | awk '{print $4}'`

	if [[ $rece_pak -eq 0 ]];then
		min_time=0
		max_time=0
		avg_time=0
		mdev_time=0
		loss_percent="100%"
		result_flag=0
	else 
		min_time=`echo $ping_data | awk '{print $14}'| awk -F/ '{print $1}'`
		max_time=`echo $ping_data | awk '{print $14}'| awk -F/ '{print $3}'`
		avg_time=`echo $ping_data | awk '{print $14}'| awk -F/ '{print $2}'`
		mdev_time=`echo $ping_data | awk '{print $14}'| awk -F/ '{print $4}'`
		loss_percent=`echo $ping_data | awk '{print $6}'`
		result_flag=1
	fi

	echo $result_flag
}

function Check_Network()
{
	num=0
	for i in $net_ip
	do
		result=$(Ping_IP $1 $i)
		if [[ $result == '0' ]];then
			let num+=1
		fi
	done

	if [ $num -ge 2 ] ; then
		echo 0
	else
		echo 1
	fi
}

echo $date

res_net=$(Check_Network 2)

if [[ $res_net == '0' ]];then
	echo "移动网络已掉线..."
else
	echo "移动网络正常..."
fi

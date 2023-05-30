#! /bin/bash

now_time=`date +%Y%m%d%H%M%S`
result_log=/home/cti/.ros/result_map_${now_time}.log

source /opt/ros/kinetic/setup.bash > ${result_log} 2>&1
	
source /opt/cti/kinetic/setup.bash > ${result_log} 2>&1

nohup roslaunch /opt/cti/kinetic/share/br_bringup/launch/start_map.launch > ${result_log} 2>&1 &


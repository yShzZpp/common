#! /bin/bash

now_time=`date +%Y%m%d%H%M%S`
result_log=/home/cti/.ros/log/result_mapping_${now_time}.log

source /opt/ros/kinetic/setup.bash > ${result_log} 2>&1

source /opt/cti/kinetic/setup.bash > ${result_log} 2>&1

nohup  roslaunch slam_toolbox candle_mapper_params_online_sync.launch > ${result_log} 2>&1 &


#! /bin/bash
#author: yxl

# now_time=`date +%Y%m%d%H%M%S`
# result_log=/home/cti/.ros/log/result_${now_time}.log

nohup /opt/cti/kinetic/share/br_bringup/scripts/killcpu 6 &

#nohup /opt/cti/kinetic/share/br_bringup/scripts/killmem &

nohup roslaunch /opt/cti/kinetic/share/virtual_robot_base/launch/startup_with_rviz.launch &
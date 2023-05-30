###
 # @Author: your name
 # @Date: 2020-03-27 10:22:26
 # @LastEditTime: 2020-03-27 10:22:59
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /src/robot/br_bringup/scripts/start_motion_upgrade.sh
 ###
#! /bin/bash

result_log=/home/cti/.ros/motion_upgrade_result.log

source /opt/ros/kinetic/setup.bash > ${result_log} 2>&1
source /opt/cti/kinetic/setup.bash > ${result_log} 2>&1

nohup roslaunch upgrade upgrade.launch > ${result_log} 2>&1 &


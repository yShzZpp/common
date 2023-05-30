#!/usr/bin/env python
# -*- coding:utf-8 -*-
#author: "yuxiaolei"
#date: "2018-10-16"

import os
import yaml
import signal 
import subprocess 
import sys
import time

import rospy
import rosbag

#日志在.ros/log/latest/cti_record_bag-xx.log

topic_set = set()
time_inter = 1
all_log_path = ''

def mkdir(path):
	folder = os.path.exists(path)
	if not folder:                   
		os.makedirs(path)            
		rospy.loginfo("---  new folder: %s ---", path)
	else:
		rospy.loginfo("---  There is this folder: %s ---", path)

def recordLog():
    args_list = []
    args_list.append('--split')
    duration = '--duration=%dm' % time_inter
    args_list.append(duration)

    args_list.append('-o') #加空格都不行
    path = all_log_path+'/'
    args_list.append(path)

    for topic in topic_set:
        args_list.append(topic)

    print args_list
    rosbag.rosbag_main.record_cmd(args_list)

def readAndParseFile():

    yaml_path = rospy.get_param("config_file")
    f = open(yaml_path, 'r')
    cfg = f.read()
    yaml_info = yaml.load(cfg)

    for topic in yaml_info['topics']:
        rospy.loginfo("topic: %s", topic)
        topic_set.add(topic)

    global time_inter
    time_inter = rospy.get_param('bag_duration')

    global all_log_path
    all_log_path = rospy.get_param('all_log_path')
    mkdir(all_log_path)

    rospy.loginfo("path: %s, interval time: %s", all_log_path, time_inter)
    f.close()

def main():
    rospy.init_node('cti_reord_log')
    readAndParseFile()
    recordLog()

if __name__ == '__main__':
    main()


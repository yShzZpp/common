#!/usr/bin/env python
# -*- coding:utf-8 -*-
#author: "yuxiaolei"
#date: "2020-07-02"

#日志在.ros/log/latest/cti_bag_monitor-xx.log

import os
import time
import shutil
import tarfile
import rospy
import threading
import math
from cti_msgs.msg import BuildingRobotState

tarflag = True
bagexist = True
 
def mkdir(path):
	folder = os.path.exists(path)
	if not folder:                   
		os.makedirs(path)            
		rospy.loginfo("---  new folder: %s", path)
	else:
		rospy.loginfo("---  folder is exist: %s", path)

def sumFileSize(path):
    size = 0L
    for root , dirs, files in os.walk(path, True):
        size += sum([os.path.getsize(os.path.join(root, name)) for name in files])
    rospy.loginfo("sum size: %ld", size)
    return size

def deleteFile(file):
    os.remove(file)
    rospy.loginfo("remove file: %s", file)

def deleteDir(dir):
    shutil.rmtree(dir)
    rospy.loginfo("remove dir: %s", dir)

def tar(dir, fname):
    rospy.loginfo("we will tar: %s", fname)
    cur_path = os.getcwd() #获取当前路径
    os.chdir(dir) #设置当前路径为需要打包文件存放的目录
    allpath = os.path.join(dir, fname)
    t = tarfile.open(allpath + ".tar.gz", "w:gz")
    t.add(fname)
    t.close()
    os.chdir(cur_path) #恢复路径

# 删除非bag，非bag.active的文件
def filterFile(dir):
    file_lists = os.listdir(dir)
    for fn in file_lists:
        if fn.split('.')[-1] != 'bag' and fn.split('.')[-1] != 'active' and fn.split('.')[-1] != 'gz':
            if not os.path.isdir(os.path.join(dir, fn)):
                deleteFile(os.path.join(dir, fn))
            else :
                deleteDir(os.path.join(dir, fn))
        # elif fn.split('.')[-1] == 'bag':
        #     tar(dir, fn)
        #     deleteFile(os.path.join(dir, fn))

def tarOneBag(dir):
    global bagexist
    file_lists = os.listdir(dir)
    bagexist = False
    for fn in file_lists:
        if fn.split('.')[-1] == 'bag':
            tar(dir, fn)
            deleteFile(os.path.join(dir, fn))
            bagexist = True
            break

def spin_job():
    rospy.spin()

def callback(data):
    global tarflag
    if (data.state == 0) or (data.state == 34) or (data.state == 40) or (data.state == 41):
        tarflag = True
    else:
        tarflag = False

def delete_old_file(path, time_interval):
    now = time.time()
    file_lists = os.listdir(path)
    for fn in file_lists:
        fd = os.path.join(path, fn)
        rospy.loginfo("check fd: %s", fd)
        if (now - os.path.getmtime(fd)) > time_interval :
            if not os.path.isdir(fd):
                deleteFile(fd)

def delete_old_file_and_dir(path, time_interval):
    now = time.time()
    file_lists = os.listdir(path)
    for fn in file_lists:
        fd = os.path.join(path, fn)
        rospy.loginfo("check fd: %s", fd)
        if (now - os.path.getmtime(fd)) > time_interval :
            if os.path.isdir(fd):
                deleteDir(fd)
            else :
                deleteFile(fd)

def delete_old_update_file():
    updatefile_path = rospy.get_param('updatefile_path')
    old_updatefile_time_interval = rospy.get_param('old_updatefile_time_interval')
    delete_old_file_and_dir(updatefile_path, old_updatefile_time_interval)

def delete_ros_log():
    ros_log_path = rospy.get_param('ros_log_path')
    ros_time_interval = rospy.get_param('time_interval')
    delete_old_file_and_dir(ros_log_path, ros_time_interval)

def delete_ros_dir_redundancy_file():
    ros_dir_path = rospy.get_param('ros_dir_path')
    ros_dir_time_interval = rospy.get_param('ros_dir_time_interval')
    delete_old_file(ros_dir_path, ros_dir_time_interval)

def delete_upgrading_redundancy_file():
    upgrading_path = rospy.get_param('upgrading_path')
    if os.path.exists(upgrading_path):
        upgrading_redundancy_file_time_interval = rospy.get_param('upgrading_redundancy_file_time_interval')
        result = os.popen('dpkg -s br-robot-release | grep Version | cut -d \' \' -f 2')
        version = result.read()
        rospy.loginfo("version: %s", version)
        now = time.time()
        file_lists = os.listdir(upgrading_path)
        for fn in file_lists:
            if fn != 'upgrade_info_current' and fn != 'APP.tar' and fn.find(version) == -1:
                fd = os.path.join(upgrading_path, fn)
                rospy.loginfo("check fd: %s", fd)
                if (now - os.path.getmtime(fd)) > upgrading_redundancy_file_time_interval :
                    if os.path.isdir(fd):
                        deleteDir(fd)
                    else :
                        deleteFile(fd)
            else:
                rospy.loginfo("not delete: %s", fn)

def clean_job():
    time.sleep(60)
    delete_old_update_file()
    delete_ros_log()
    delete_ros_dir_redundancy_file()
    delete_upgrading_redundancy_file()
    while not rospy.is_shutdown():
        time.sleep(60*60)
        hour = (math.floor(time.time()/3600)+8)%24 # +8是因为北京在东八区
        rospy.loginfo("current time : %d", hour)
        if hour == 3 :
            delete_ros_log()

def main():

    rospy.init_node('cti_monitor_log')
    rospy.Subscriber("/robot_state", BuildingRobotState, callback)

    interval_time = rospy.get_param('monitor_interval')
    all_log_path = rospy.get_param('all_log_path')
    max_size = rospy.get_param('bags_max_size') * (1024*1024*1024)

    rospy.loginfo("monitor start ---")
    rospy.loginfo("path: %s; max size: %ld, interval time: %d", all_log_path, max_size, interval_time)
    
    spin_thread = threading.Thread(target = spin_job)
    spin_thread.start()

    clean_ros_log = threading.Thread(target = clean_job)
    clean_ros_log.start()

    while tarflag and bagexist :
        tarOneBag(all_log_path)
        time.sleep(1)
        rospy.loginfo("tar flag %d, bag exist %d", tarflag, bagexist)

    while not rospy.is_shutdown():
        # 删除非bag，非bag.active文件, 将bag文件压缩并删除原bag文件.
        filterFile(all_log_path)
        # 判断文件夹大小, 删除最旧的日志
        while sumFileSize(all_log_path) >= max_size:
            #delete file
            rospy.loginfo("we need delete file")
            file_lists = os.listdir(all_log_path)
            if len(file_lists) >= 1 :
                file_lists.sort(key=lambda fn: os.path.getmtime(all_log_path + '/'+ fn) 
                                if not os.path.isdir(all_log_path + '/' + fn) else 0)
                oldest_file = os.path.join(all_log_path, file_lists[0])
                rospy.loginfo("oldest files: %s", oldest_file)
                deleteFile(oldest_file)
            else :
                rospy.loginfo("only one bag in dir,so don't delete it")
        rospy.loginfo("we will sleep %d", 60*interval_time)
        time.sleep(60*interval_time)

if __name__ == '__main__':
    main()


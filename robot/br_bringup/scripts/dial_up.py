# -*- coding: utf-8 -*-

#Author: huanwenguang
#purpose: 4G模块联网
#date: 2022/02/11
#usage: sudo python network.py ttyUSB2 115200
'''
按q退出程序
'''
import serial
import time, datetime
import os
import json, sys
import threading    
import serial, json
from time import sleep
import struct
ser = None
exitFlag = 0
cnt = 0;

g_port = ""
g_ifconfig_name  = ""

def recv_serial():  
    while True:
        if exitFlag:
            print("thread exit")
            exit(0)
        data = ser.read_all()
        if data == '':
            time.sleep(1)
            continue
        else:
        # break
            time.sleep(0.1)
        print(data)

def apn_get():
    ccid = get_ICCID()
    China_Mobile    = ["898600","898602","898604","898607"]
    China_Unicom    = ["898601","898606","898609"]
    China_Telecom   = ["898603","898611"]

#   移动
    for sub in China_Mobile:
        if sub in ccid:
            return "cmwap"

#   联通
    for sub in China_Unicom:
        if sub in ccid:
            return "3gnet"

#   电信
    for sub in China_Telecom:
        if sub in ccid:
            return "ctlte"
            
    return "cmwap"

# 华为模块 拨号端口上网
def huawi_connect(apn):  
        # ret = ser.write(b"AT+CFUN=0\r\n")
        time.sleep(2)
        # ret = ser.write(b"AT+CFUN=1\r\n")
        time.sleep(2)
        # ret = ser.write(b"AT+CGREG?\r\n")
        ret = ser.write(b"AT^NDISDUP=?\r\n")
        #buffer = ser.read()
        time.sleep(2)
        print(b"AT^NDISDUP=2,1,\"" + apn + "\"\r\n")
        ret = ser.write(b"AT^NDISDUP=2,1,\"" + apn + "\"\r\n")
        #ret = ser.write(b"AT^NDISDUP=2,1,\"cmwap\"\r\n")
        # ret = ser.write(b"AT^NDISDUP=2,1\r\n")
        #buffer = ser.read()
        time.sleep(2)
        ret = ser.write(b"AT^NDISSTATQRY=1\r\n")
        #buffer = ser.read()
        time.sleep(1)

# 鼎桥模块 拨号端口上网        
def dinaqiao_connect(apn):  
        ret = ser.write(b"AT+CFUN=0\r\n")
        time.sleep(2)
        ret = ser.write(b"AT+CFUN=1\r\n")
        time.sleep(2)
        ret = ser.write(b"AT+CGREG?\r\n")
        # ret = ser.write(b"AT^NDISDUP=?\r\n")
        #buffer = ser.read()
        time.sleep(2)
        print(b"AT^NDISDUP=2,1,\"" + apn + "\"\r\n")
        ret = ser.write(b"AT^NDISDUP=2,1,\"" + apn + "\"\r\n")
        # ret = ser.write(b"AT^NDISDUP=2,1\r\n")
        #buffer = ser.read()
        time.sleep(2)
        # ret = ser.write(b"AT^NDISSTATQRY=1\r\n")
        #buffer = ser.read()
        time.sleep(1)

def distcont():  
    ret = ser.write(b"AT^NDISDUP=1,0\r\n")
    data = ser.read()
    time.sleep(1)
    print(data)

def card_inserted(): 
    ignore = ser.read_all() #  
    time.sleep(1)
    ret = ser.write(b"AT+CPIN?\r\n")
    data = ser.read_all()
    
    print(data)

def get_CSQ():  
    ignore = ser.read_all() # 
    ret = ser.write(b"AT+CSQ\r\n")
    time.sleep(1)
    data = ser.read_all()
    print(type(data))
    csq = data.strip(',99\r\n\r\nOK').strip('+CSQ: ')
    print(csq)
    return csq
	

def get_ICCID():  

    ignore = ser.read_all() # 
    ret = ser.write(b"AT^ICCID?\r\n")
    time.sleep(1)
    data = ser.read_all()
    # print(data)
    iccid = data.strip('\r\n\r\nOK').strip('^ICCID: ')
    print(iccid)
    return iccid


def get_CIMI():  

    ignore = ser.read_all() # 
    ret = ser.write(b"AT+CIMI\r\n")
    time.sleep(1)
    data = ser.read_all()
    cimi = data.strip('\r\n\r\nOK').strip('\r\n')
    print(cimi)
    return cimi

def save_dict_to_yaml(dict_value, save_path):
    """dict保存为yaml"""
    with open(save_path, 'w') as file:
        file.write(json.dumps(dict_value))

def save_Info():
    dict = {}

    dict["port"] = g_port
    dict["ifconfig_name"] = g_ifconfig_name
    dict["quality"] = get_CSQ()
    dict["cimi"] = get_CIMI()
    dict["iccid"] = get_ICCID()

    # filePath = os.path.expanduser('~')+"/.ros/network_info.json"
    filePath = "/home/cti/.ros/network_info.json"
    save_dict_to_yaml(dict,filePath)
    print("保存网卡信息到文件: %s"%filePath)
    print(dict)


def main():
    global ser
    global g_port
    global g_ifconfig_name
    print(len(sys.argv))
    if (len(sys.argv) != 4):
        print ("usage: python simple_network.py /dev/ttyUSB2 0 eth0")
        exit(1)
   
    g_port = sys.argv[1]
    cart_type = sys.argv[2]
    g_ifconfig_name = sys.argv[3]
    ser = serial.Serial(g_port, 115200, timeout=2)
    print("cart_type",cart_type)

    save_Info()

    apn = apn_get()
    print("apn:%s"%apn)

    if cart_type == '0':
        print("端口号:%s 华为4G卡类型"%g_port)
        huawi_connect(apn)
    elif cart_type == '1':
        print("端口号:%s 鼎桥4G卡类型"%g_port)
        dinaqiao_connect(apn);       

if __name__ == '__main__':
    main()

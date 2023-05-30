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
    # sleep(0.02)
    # time.sleep(1)
        print(data)
        
def connect():  
        ret = ser.write(b"AT+CFUN=0\r\n")
        time.sleep(2)
        ret = ser.write(b"AT+CFUN=1\r\n")
        time.sleep(2)
        ret = ser.write(b"AT+CGREG?\r\n")
        # ret = ser.write(b"AT^NDISDUP=?\r\n")
        #buffer = ser.read()
        time.sleep(2)
        ret = ser.write(b"AT^NDISDUP=2,1,\"cmwap\"\r\n")
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
    ret = ser.write(b"AT+CPIN?\r\n")
    data = ser.read()
    time.sleep(1)
    print(data)



def get_CSQ():  
    ret = ser.write(b"AT+CSQ\r\n")
    data = ser.read()
    time.sleep(1)
    print(data)
	

def get_ICCID():  
    ret = ser.write(b"AT^ICCID?\r\n")
    data = ser.read()
    time.sleep(1)
    print(data)


def main():
    global ser
    print(len(sys.argv))
    if (len(sys.argv) != 2):
        print ("usage: python simple_network.py /dev/ttyUSB2")
        exit(1)
    port = sys.argv[1]
    print(port)
    ser = serial.Serial(port, 115200, timeout=2)
  
    connect()
   
    
if __name__ == '__main__':
    main()


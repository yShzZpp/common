
/*
author: matt ji
date: 2019-8-29
brief: 通过dbus接口来控制wifi
v0.9 2019-8-26
gcc -Wall tkwifi.c wifi.c wifi-fun.c -o tkwifi `pkg-config --cflags --libs libnm uuid`
v1.0 2019-11-5
1. 重新梳理流程
2. 也可以做为测试电梯wifi的程序
v1.1 2019-11-21
sudo apt-get install libnm-dev uuid-dev

*/


/*
6个接口：

1. 寻找到wif硬件设备，比如/org/freedesktop/NetworkManager/Devices/0
2. 检查所有连接，看有没有特定连接(tikong-00013)的名字 check_exist,
    如果有，就记录下来/org/freedesktop/NetworkManager/Settings/10
3. 如果没有，创建连接函数 add_wifi_connection, CON_NAME 为tikong-00013与ssid一样
    不能设置成自动连接，因为如果有好几部电梯，就不知道连的哪一部电梯


一. 连接wifi connect_wifi(const char *ssid) return const char *;
    1. 寻找wifi硬件 find_hw()
        a. 成功继续进行第二步
        b. 不成功返回no wifi hardware字符串
    2. 检查之前是否已经连接过
        a. 第一次连接 创建连接，add_connect()
        b。 已经连接过，enable_connect()

connect_wifi()
1. 找到wifi接口设备path/DEVICES/1
2. 调用path/DEVICES/1使用Disconnect，可以断连wifi
3. 然后再连接特定的连接
4. 使用完之后需要再断连wifi，而不只是断连tikong的SSID

新流程：
1. 得到库版本
2. 断连wifi
2. 连接wifi
    2.1 find_hw会把wifi硬件设备的路径找到
    2.2 check_exist会把原有连接路径找到
    2.3 如果之前没有连接过，就add_conn
    2.4 active_conn 使能连接
3. 测试连通性，需要输入测试ip的网段

20191212:增加取得4g信号质量

2020-1-2：增加开启wifi热点功能  
*/

#include "wifi.h"
#include "ap.h"
#include "stdio.h"
#include "unistd.h"
#include <stdbool.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

//gcc tkwifi.c libtkwifi.a -o tkwifi -lpthread -lnm -lgio-2.0 -lgobject-2.0 -lglib-2.0 -luuid

//===========测试程序开始=========
int
main (int argc, char *argv[])
{
    int status;
    //缓冲区大于500字节
    char ret[501]={};
    struct timeval start, end;
    status = get_version(ret);
    if(status == 0)
    {
        printf("nettool version %s, %ld\n", ret, sizeof(ret));
    }

    if(argc != 4)
    {
        printf("./nettool wlp2s0 ssid password\n");
        //printf("./nettool ttyUSB0\n");
        return 1;
    }

    memset(ret, 0, sizeof ret);
    char buffer[33]={};
    struct timeval tv;
    time_t curtime;

//1. 断连现有wifi
    status = disconnect_wifi(argv[1], ret);
    if(status == 0)
    {
        printf("disconnet wifi\n");
    }
    else
    {
        printf("err code: %d, err msg: %s\n", status, ret);
    }
    memset(ret, 0, sizeof ret);
    //计算时间开始
    gettimeofday(&start, 0);
//2. 使能ap热点ssid，connect_wifi内部会先去检测这个ssid是否已经存在，
    //已经存在的话就active，不存在的话就先建立
    status = connect_ap(argv[1], argv[2], argv[3], ret);
    if(status != 0)
    {
        printf("err code: %d, err msg: %s\n", status, ret);
        return 2;
    }

    memset(ret, 0, sizeof ret);

    //计算时间结束
    gettimeofday(&end, 0);
    double timeuse = 1000000*(end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    timeuse /= 1000; // 1000 to ms, 1000000 to seconds
    printf("timeuse %fms\n", timeuse);

    return 0;
}

//===========测试程序结束=========

//备用代码
    //写入文件
    /*
    FILE *fp;
    while(1)
    {
        //status = check_4g_signal(argv[1], ret);
        status = check_4g_signal("ttyUSB2", ret);
        if(status == 0)
        {
            //printf("err code: %d, err msg: %s\n", status, ret);
            
           gettimeofday(&tv, NULL);
           curtime = tv.tv_sec;
           strftime(buffer, 30, "%Y-%m-%d-%T: ", localtime(&curtime));
            fp = fopen("/tmp/signal.txt", "a");

           fputs(buffer, fp);
           fprintf(fp, "signal strength %s\n", ret);
            fclose(fp);

           //fputs("This is testing for fputs...\n", fp);
           
        }
        sleep(2);
    }*/
/*
   int try = 0;
    while(try < 20)
    {
        status = check_connectivity(argv[1], argv[2], "192.168.1.", ret);
        if(status == 0)
        {
            break;
        }
        else
        {
            printf("err code: %d, err msg: %s\n", status, ret);
            try++;
            sleep(1.5);
        }
    }
    */

    //这里如果一连上就去ping会出现connect: Network is unreachable,实际上在命令行是可以ping通的
   // sleep(10);
    /*status = remove_conn(argv[2], ret);
    if(status != 0)
    {
        printf("%s\n", ret);
    }*/

//    status = check_signal(argv[1], ret);
//    printf("err code: %d, err msg: %s\n", status, ret);
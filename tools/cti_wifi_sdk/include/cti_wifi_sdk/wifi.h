#ifndef WIFIH
#define WIFIH


int get_version(char *ver);
//dft_route没有特殊情况下设置为0
int connect_wifi(const char *iface, const char *ssid, const char *pw, const int dft_route, char *ret);
//如果当前iface有活动的wifi，断掉，
int disconnect_wifi(const char *iface, char *ret);
//ip 为要测试的网段，比如梯控是192.168.1. 闸机是192.168.4.
int check_connectivity(const char *iface, const char *ssid, const char *ip, char * ret);
//删除这个ssid上所有的连接
int remove_conn(const char *ssid, char *ret);
//获取某个网络接口当前状态，如果是4g，
int check_signal(const char * iface, char *ret);
//int disable_auto(const char *iface, char *ret);
//需要更新的连接的路径 属性名 属性的值
int update_property(const char *path, const char* property, const char* value, char * ret);
//列出这个ssid上所有的连接的路径，ret默认500字节
//如果没有就返回no path，如果有很多，字符串放不下
//就返回need 1000 bytes这样就可以继续来调用
//正确就返回0，没有路径或者缓冲区太少就返回1
int list_connections(const char *ssid, char *ret);

#endif
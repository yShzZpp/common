#ifndef WIFIFUNH
#define WIFIFUNH
#include <gio/gio.h>
#include <uuid/uuid.h>
#include <string.h>
#include <stdbool.h>
#include <nm-dbus-interface.h>
#include <NetworkManager.h>
#include <stdlib.h>
#include <stdio.h>


#define TEST 1
//#undef TEST




//char   ret[100]=""; 
//#define IFACE "wlp3s0"

extern char WIFIDEVICE[];

#ifdef __cplusplus
extern "C"
{
#endif
char *
nm_utils_uuid_generate (void);

 gboolean
//add_connection (GDBusProxy *proxy, const char *con_name);
add_connection (const char *iface, const char *con_name, const char *pw, const bool dft_route, char *err);

gboolean
check_online(char *err);

gboolean 
get_property(const char *path, const char * if1, const char *method, const char *if2, const char* property, char *value, char *err);

gboolean
get_property_r(const char *path, const char * if1, const char *method, const char *if2, const char* property, int *value, char *err);

gboolean
get_active_connection_details (const char *obj_path, const char *ssid);

gboolean
find_tk_wifi (GDBusProxy *proxy, const char *ssid);

gboolean
remove_fun(const char *obj_path, char *err);

gboolean
find_tk_conn(GDBusProxy *proxy);

gboolean
active_conn(char *err);

gboolean 
is_wifi(const char *obj_path, const char *iface);

gboolean
find_hw(const char *iface);

gboolean
find_hw_r(const char *iface, char *device_path, char *err);


gboolean
find_ap(const char *iface, char *err);

gboolean
disc_wifi_fun(char *ret);
gboolean exec(const char* cmd, char *ret);
//gboolean exec(const char* cmd, char *ret);
#ifdef __cplusplus
}
#endif

#endif
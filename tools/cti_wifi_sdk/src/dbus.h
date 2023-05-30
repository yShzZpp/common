#ifndef _DBUS_H
#define _DBUS_H
#include <gio/gio.h>
#include <uuid/uuid.h>
#include <string.h>
#include <stdbool.h>
#include <nm-dbus-interface.h>
#include <NetworkManager.h>
#include <stdlib.h>
#include <stdio.h>
gboolean
check_ap_exist(const char *ssid, char *link_path, char *err);
gboolean
add_ap_connection (const char *iface, const char *con_name, const char *pw, char *link_path, char *err);
gboolean
active_ap_conn(const char *device_path, const char *link_path,char *err);

#endif
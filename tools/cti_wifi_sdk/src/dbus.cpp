#include "dbus.h"
#include <nm-dbus-interface.h>
#include <NetworkManager.h>
#include <uuid/uuid.h>
#include <string>
#include "misc.h"
#include <iostream>
using namespace std;
char *
nm_utils_uuid_generate_ap (void)
{
    uuid_t uuid;
    char *buf;

    buf = (char *)g_malloc0 (37);
    if (buf==NULL) 
    {
        printf("can't malloc in uuid\n");
        exit (1);
    }
    uuid_generate_random (uuid);
    uuid_unparse_lower (uuid, &buf[0]);
    return buf;
}


gboolean
get_active_connection_details(const char *obj_path, const char *ssid)
{
  GDBusProxy *props_proxy = NULL;
  GVariant *ret = NULL, *path_value = NULL;
  //const char *path = NULL;
  GError *error = NULL;

  /* This function gets the backing Connection object that describes the
   * network configuration that the ActiveConnection object is actually using.
   * The ActiveConnection object contains the mapping between the configuration
   * and the actual network interfaces that are using that configuration.
   */

  /* Create a D-Bus object proxy for the active connection object's properties */
  props_proxy = g_dbus_proxy_new_for_bus_sync (G_BUS_TYPE_SYSTEM,
                                 G_DBUS_PROXY_FLAGS_NONE,
                                 NULL,
                                 //"org.freedesktop.NetworkManager"
                                 NM_DBUS_SERVICE,
                                 obj_path,
   "org.freedesktop.NetworkManager.Settings.Connection",
                                               NULL, NULL);
  g_assert (props_proxy);

  /* Get the object path of the Connection details */
  ret = g_dbus_proxy_call_sync (props_proxy,
                                "GetSettings",
                                NULL,
                                G_DBUS_CALL_FLAGS_NONE, -1,
                                NULL, &error);
  if (!ret) {
    g_dbus_error_strip_remote_error (error);
    g_warning ("Failed to get active connection Connection property: %s\n",
               error->message);
    g_error_free (error);
      g_object_unref (props_proxy);
    return false;
  }
  
  g_variant_get (ret, "(@a{sa{sv}})", &path_value);
  gboolean found = FALSE;


  GVariant *s_con = NULL;
  const char  *type, *id;

  s_con = g_variant_lookup_value (path_value, NM_SETTING_CONNECTION_SETTING_NAME, NULL);
  found = g_variant_lookup (s_con, NM_SETTING_CONNECTION_TYPE, "&s", &type);
  g_assert (found);
  found = g_variant_lookup (s_con, NM_SETTING_CONNECTION_ID, "&s", &id);
  g_assert (found);
  int length = 0;

  found = false;
  //printf("path %s <==> ssid %s\n", obj_path, str);

  if(strcmp(type, "802-11-wireless") == 0)
  {
      gchar buf[100];
    GVariant *a = NULL;
    //得到802-11-wireless字段
    GVariant *wireless= NULL;
    wireless = g_variant_lookup_value (path_value, NM_SETTING_WIRELESS_SETTING_NAME, NULL);
    //found = g_variant_lookup (wireless, NM_SETTING_WIRELESS_SSID, "^ay", str);  
    a = g_variant_lookup_value (wireless, NM_SETTING_WIRELESS_SSID, NULL);
    
    GVariantIter  *iter;
    g_variant_get(a, "ay", &iter);
    gchar s;
    while(g_variant_iter_loop(iter, "y", &s))
    {
          buf[length] = s;
          length++;
    }
    
    //凡是指針都要釋放
    g_variant_iter_free(iter);
    //ssid里是字符数组，不是放的字符串，所以要循环比对
    int j = 0;
    if(length == strlen(ssid))
    {
      for(j = 0; j < length; j++)
      {
        if(buf[j] != ssid[j])
        {
          break;
        }
      }
    }
    if(j == length)
    {
      printf("path: %s, connection name: %s, ssid: ", obj_path, id);
      for(int j = 0; j < length; j++)
          g_print("%c", buf[j]);
      g_print("\n");
      found = TRUE;
    } 

    if (wireless != NULL)
    {
      g_variant_unref (wireless);	
    }
    if (a != NULL)
    {
      g_variant_unref (a);	
    }   
  }

	if (s_con != NULL)
	{
		g_variant_unref (s_con);	
	}

out:
  if (path_value)
    g_variant_unref (path_value);
  if (ret)
    g_variant_unref (ret);
  g_object_unref (props_proxy);
  return found;
}


gboolean
check_ap_exist(const char *ssid, char *link_path, char *err)
{
    GDBusProxy *proxy = NULL;
    GVariant *ret = NULL;
    GError *error = NULL;

    /* Create a D-Bus proxy; NM_DBUS_* defined in nm-dbus-interface.h */
    proxy = g_dbus_proxy_new_for_bus_sync (G_BUS_TYPE_SYSTEM,
                   G_DBUS_PROXY_FLAGS_NONE,
                   NULL,
                   NM_DBUS_SERVICE,
                   NM_DBUS_PATH_SETTINGS,
                   NM_DBUS_INTERFACE_SETTINGS,
                   NULL, NULL);
    g_assert (proxy != NULL);

  /* Call ListConnections D-Bus method */
  ret = g_dbus_proxy_call_sync (proxy,
                                "ListConnections",
                                NULL,
                                G_DBUS_CALL_FLAGS_NONE, -1,
                                NULL, &error);
  if (!ret) {
    g_dbus_error_strip_remote_error (error);
    g_print ("ListConnections failed: %s\n", error->message);
    g_error_free (error);

    //wrx	这里必须去释放掉proxy
		g_object_unref (proxy);

    return FALSE;
  }

  char **paths = NULL;
  g_variant_get (ret, "(^ao)", &paths);
  g_variant_unref (ret);

  gboolean found = FALSE;

  for (int i = 0; paths[i]; i++)
  {    
    found = get_active_connection_details(paths[i], ssid);
    if(found == TRUE)
    {
      memcpy(link_path, paths[i], strlen(paths[i]));
        //goto OUT;
      break;
    }
        
  }

  g_strfreev (paths);  
    g_object_unref (proxy);

    return found;
}

/*add_wifi_connection->add_connection*/
gboolean
add_ap_connection(const char *iface, const char *con_name, const char *pw, char * link_path,char *err)
{
  GDBusProxy *proxy;
  GError *error = NULL;

  /* Create a D-Bus proxy; NM_DBUS_* defined in nm-dbus-interface.h */
  proxy = g_dbus_proxy_new_for_bus_sync (G_BUS_TYPE_SYSTEM,
                                         G_DBUS_PROXY_FLAGS_NONE,
                                         NULL,
                                         NM_DBUS_SERVICE,
                                           //"/org/freedesktop/NetworkManager/Settings"
                                         NM_DBUS_PATH_SETTINGS,
                                           // "org.freedesktop.NetworkManager.Settings"
                                         NM_DBUS_INTERFACE_SETTINGS,
                                         NULL, &error);
  if (!proxy) {
    g_dbus_error_strip_remote_error (error);
    g_print ("Could not create NetworkManager D-Bus proxy: %s\n", error->message);
    strcpy(err, error->message);
    g_error_free (error);
    return FALSE;
  }

  GVariantBuilder connection_builder;
  GVariantBuilder setting_builder;
  char *uuid;
  const char *new_con_path;
  GVariant *ret;
//创建连接的接口是 AddConnection
/*
The AddConnection() method
AddConnection (IN  a{sa{sv}} connection,
               OUT o         path);
Add new connection and save it to disk. This operation does not start the network connection unless (1) device is idle and able to connect to the network described by the new connection, and (2) the connection is allowed to be started automatically.
IN a{sa{sv}} connection:
Connection settings and properties.
OUT o path:
Object path of the new connection that was just added.

*/
  /* Initialize connection GVariantBuilder */
  g_variant_builder_init (&connection_builder, G_VARIANT_TYPE ("a{sa{sv}}"));

  /*1. Build up the 'connection' Setting */
  g_variant_builder_init (&setting_builder, G_VARIANT_TYPE ("a{sv}"));
  uuid = nm_utils_uuid_generate_ap ();
  g_variant_builder_add (&setting_builder, "{sv}",
                         NM_SETTING_CONNECTION_UUID,
                         g_variant_new_string (uuid));
  g_free (uuid);
  g_variant_builder_add (&setting_builder, "{sv}",
                           //"id"
                         NM_SETTING_CONNECTION_ID,
                         g_variant_new_string (con_name));
  g_variant_builder_add (&setting_builder, "{sv}",
                            //"type"
                         NM_SETTING_CONNECTION_TYPE,
                           //802-11-wireless
                         g_variant_new_string (NM_SETTING_WIRELESS_SETTING_NAME));
  g_variant_builder_add (&setting_builder, "{sv}",
                            //"interface-name"
                         NM_SETTING_CONNECTION_INTERFACE_NAME,
                         g_variant_new_string (iface));    
    //自动连接                        
    g_variant_builder_add (&setting_builder, "{sv}",
                            //"type"
                         NM_SETTING_CONNECTION_AUTOCONNECT,
                         //设置ap自动连接为true
                         g_variant_new_boolean (TRUE)); 
    
    g_variant_builder_add (&connection_builder, "{sa{sv}}",
                         NM_SETTING_CONNECTION_SETTING_NAME,
                         &setting_builder);

    /*2.  Add the 'wireless' Setting */
    g_variant_builder_init (&setting_builder, G_VARIANT_TYPE ("a{sv}"));
    //GVariantBuilder *builder;
    g_variant_builder_add (&setting_builder, "{sv}",
                         NM_SETTING_WIRELESS_MODE,
                         g_variant_new_string(NM_SETTING_WIRELESS_MODE_AP));  
    //设置ap要多一个mac地址
    GVariantBuilder *builder;
    GVariant *value;
//mac是一个字符数组
    char cmd[100]="";
    strcpy(cmd, "cat /sys/class/net/");
    strcat(cmd, iface);
    strcat(cmd, "/address");
    string buf = exec(cmd);
    buf.pop_back();
    //cout << buf << endl;
    int mac[6];
    //unsigned char mac[6];
    sscanf(buf.c_str(), "%02x:%02x:%02x:%02x:%02x:%02x", &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
    //sscanf(buf.c_str(), "%c:%c:%c:%c:%c:%c", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    builder = g_variant_builder_new (G_VARIANT_TYPE ("ay"));
    for(int i = 0; i< sizeof mac/sizeof mac[0]; i++)
        g_variant_builder_add (builder, "y", mac[i]);
    value = g_variant_new ("ay", builder);
    g_variant_builder_unref (builder);  
    g_variant_builder_add (&setting_builder, "{sv}",
                         NM_SETTING_WIRELESS_MAC_ADDRESS,
                        // g_variant_new_string(NM_SETTING_WIRELESS_MODE_AP));   
                        value);   
    //g_object_unref(value);       
    //ssid是一个字符数组
    builder = g_variant_builder_new (G_VARIANT_TYPE ("ay"));
    for(int i = 0; i< strlen(con_name); i++)
        g_variant_builder_add (builder, "y", con_name[i]);
    value = g_variant_new ("ay", builder);
    g_variant_builder_unref (builder);
    g_variant_builder_add (&setting_builder, "{sv}",
                           //"ssid" GArray_guchar_ *
                           NM_SETTING_WIRELESS_SSID,
                           value);     
    //seen-bssids 是一个as结构
    builder = g_variant_builder_new (G_VARIANT_TYPE ("as"));
    g_variant_builder_add (builder, "s", "D8:F2:CA:5D:B9:F2");
    value = g_variant_new ("as", builder);
    //value = g_variant_new ("s", "D8:F2:CA:5D:B9:F2");
    g_variant_builder_unref (builder);
    g_variant_builder_add (&setting_builder, "{sv}",
                           NM_SETTING_WIRELESS_SEEN_BSSIDS,
                           value);                                                                             
    g_variant_builder_add (&connection_builder, "{sa{sv}}",
                         NM_SETTING_WIRELESS_SETTING_NAME,
                         &setting_builder);

    //g_variant_unref (value);


    /*3. Build up the 'wifi-security' Setting */
    g_variant_builder_init (&setting_builder, G_VARIANT_TYPE ("a{sv}"));
    g_variant_builder_add (&setting_builder, "{sv}",
                         NM_SETTING_WIRELESS_SECURITY_KEY_MGMT,
                         g_variant_new_string ("wpa-psk"));
    g_variant_builder_add (&setting_builder, "{sv}",
                         NM_SETTING_WIRELESS_SECURITY_PSK,
                         g_variant_new_string (pw));                       

    g_variant_builder_add (&connection_builder, "{sa{sv}}",
                         NM_SETTING_WIRELESS_SECURITY_SETTING_NAME,
                         &setting_builder);
    
    
  /*4. Build up the 'ipv4' Setting */
  g_variant_builder_init (&setting_builder, G_VARIANT_TYPE ("a{sv}"));
  g_variant_builder_add (&setting_builder, "{sv}",
                         NM_SETTING_IP_CONFIG_METHOD,
                         //ap模式设置成shared
                         g_variant_new_string (NM_SETTING_IP4_CONFIG_METHOD_SHARED));


    g_variant_builder_add (&connection_builder, "{sa{sv}}",
                         NM_SETTING_IP4_CONFIG_SETTING_NAME,
                         &setting_builder);

      /* Build up the 'ipv6' Setting 目前不支持*/ 
    /*
    g_variant_builder_init (&setting_builder, G_VARIANT_TYPE ("a{sv}"));
    g_variant_builder_add (&setting_builder, "{sv}",
                         NM_SETTING_IP_CONFIG_METHOD,
                         //ap模式设置成shared
                         g_variant_new_string (NM_SETTING_IP6_CONFIG_METHOD_SHARED));


    g_variant_builder_add (&connection_builder, "{sa{sv}}",
                         NM_SETTING_IP6_CONFIG_SETTING_NAME,
                         &setting_builder);*/
  /* Call AddConnection with the connection dictionary as argument.
   * (g_variant_new() will consume the floating GVariant returned from
   * &connection_builder, and g_dbus_proxy_call_sync() will consume the
   * floating variant returned from g_variant_new(), so no cleanup is needed.
   */
  ret = g_dbus_proxy_call_sync (proxy,
                  "AddConnection",
                  g_variant_new ("(a{sa{sv}})", &connection_builder),
                  G_DBUS_CALL_FLAGS_NONE, -1,
                  NULL, &error);
  if (ret) {
    g_variant_get (ret, "(&o)", &new_con_path);
    g_print ("Added: %s\n", new_con_path);
    memcpy(link_path, new_con_path, strlen(new_con_path));
    g_variant_unref (ret);
    g_object_unref (proxy);
    return TRUE;
  } else {
    g_dbus_error_strip_remote_error (error);
    g_print ("Error adding connection: %s\n", error->message);
    strcpy(err, error->message);
    g_clear_error (&error); 
    g_object_unref (proxy);
    return FALSE;
  }
}


gboolean
active_ap_conn(const char *device_path, const char *link_path,char *err)
{

    GDBusProxy *proxy = NULL;
    GError *error = NULL;
    GVariant *ret = NULL;

    //g_print("then connect %s\n", SSID);

  /* Create a D-Bus proxy; NM_DBUS_* defined in nm-dbus-interface.h */
  proxy = g_dbus_proxy_new_for_bus_sync (G_BUS_TYPE_SYSTEM,
                           G_DBUS_PROXY_FLAGS_NONE,
                           NULL,
                           // "org.freedesktop.NetworkManager"
                           NM_DBUS_SERVICE,
                           //"/org/freedesktop/NetworkManager"
                           NM_DBUS_PATH,                                          
                           "org.freedesktop.DBus.Properties",
                           NULL, NULL);
  g_assert (proxy != NULL);
    //首先检查WirelessEnabled是否为TRUE
  //无脑enable，即使原来就是True，也没有问题,set没有返回值
  //NetworkingEnabled只能读
  
  GVariant *value = g_variant_new ("b", TRUE);

  ret = g_dbus_proxy_call_sync (proxy,
    "Get",
    g_variant_new ("(ss)", "org.freedesktop.NetworkManager","NetworkingEnabled"),
    //g_variant_new ("(ss)", "org.freedesktop.NetworkManager","NetworkingEnabled"),
    G_DBUS_CALL_FLAGS_NONE, -1,
    NULL, &error);
  if(ret)
  {
    gboolean status = FALSE;
      GVariant *v = NULL;
    g_variant_get(ret, "(v)", &v);
    g_variant_get(v, "b", &status);
    printf("network %s\n",(status == TRUE) ? "Enabled": "Disabled");

    //wrx
    if(v != NULL)
		{
			g_variant_unref (v);
		}
  }
  else{
    g_dbus_error_strip_remote_error (error);
    strcpy(err, "read network status: ");
    strcat(err, error->message);
    g_clear_error (&error); 
    g_object_unref (proxy);
    return FALSE;
  }

	//wrx 
	g_variant_unref (ret);
	ret = NULL;


  ret = g_dbus_proxy_call_sync (proxy,
    "Get",
    g_variant_new ("(ss)", "org.freedesktop.NetworkManager","WirelessEnabled"),
    G_DBUS_CALL_FLAGS_NONE, -1,
    NULL, &error);
  if(ret)
  {
    gboolean status = FALSE;
      GVariant *v;
    g_variant_get(ret, "(v)", &v);
    g_variant_get(v, "b", &status);
    printf("wireless %s\n",(status == TRUE) ? "Enabled": "Disabled");
    if(status == FALSE)
    {
      printf("set wireless enabled\n");
      g_dbus_proxy_call_sync (proxy,
        "Set",
        g_variant_new ("(ssv)", "org.freedesktop.NetworkManager","WirelessEnabled", value),
        G_DBUS_CALL_FLAGS_NONE, -1,
        NULL, &error);
      if(error != NULL)
      {
        g_dbus_error_strip_remote_error (error);
        strcpy(err, "set wireless enable: ");
        strcat(err, error->message);
        g_clear_error (&error);
        g_object_unref (proxy);

        //wrx 
				g_variant_unref (ret);

        return FALSE;
      }
      sleep(2);
    }
  }

  g_object_unref (proxy);
  /* Create a D-Bus proxy; NM_DBUS_* defined in nm-dbus-interface.h */
  proxy = g_dbus_proxy_new_for_bus_sync (G_BUS_TYPE_SYSTEM,
                           G_DBUS_PROXY_FLAGS_NONE,
                           NULL,
                           // "org.freedesktop.NetworkManager"
                           NM_DBUS_SERVICE,
                           //"/org/freedesktop/NetworkManager"
                           NM_DBUS_PATH,                                          
                           "org.freedesktop.NetworkManager",
                           NULL, NULL);
  g_assert (proxy != NULL);

  //wrx
	if(ret != NULL)
	{
		g_variant_unref (ret);
    ret = NULL;
	}


    //接着使能连接
  ret = g_dbus_proxy_call_sync (proxy,
                              "ActivateConnection",
                              g_variant_new ("(ooo)", link_path,device_path,"/"),
                              G_DBUS_CALL_FLAGS_NONE, -1,
                              NULL, &error);



  if (ret) {
    g_variant_unref (ret);
    g_object_unref (proxy);
    return TRUE;
  } else {
    g_dbus_error_strip_remote_error (error);
    g_print ("Error active connection: %s\n", error->message);
    strcpy(err, error->message);
    g_clear_error (&error); 
    g_object_unref (proxy);
    return FALSE;
  }
}
/*
author: matt ji
date: 2019-8-29
brief: 关于wifi控制的底层接口
*/
#include "wifi-fun.h"

char PATH[100] = {};
//wifi device是硬件设备名 /org/freedesktop/NetworkManager/Devices/0
char WIFIDEVICE[100] = {};
//char ACTIVECONN[200]={};

char *
nm_utils_uuid_generate(void)
{
	uuid_t uuid;
	char *buf;

	buf = g_malloc0(37);
	uuid_generate_random(uuid);
	uuid_unparse_lower(uuid, &buf[0]);
	return buf;
}

//目前只支持Get 以及ss两个参数，以及返回值是字符串
//后期需要更改
gboolean
get_property(const char *path, const char *if1, const char *method, const char *if2, const char *property, char *value, char *err)
{
	//wrx	变量初始化
	GDBusProxy *proxy = NULL;
	gboolean status = FALSE;
	GError *error = NULL;
	/* Create a D-Bus proxy; NM_DBUS_* defined in nm-dbus-interface.h */
	proxy = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM,
										  G_DBUS_PROXY_FLAGS_NONE,
										  NULL,
										  // "org.freedesktop.NetworkManager"
										  NM_DBUS_SERVICE,
										  path,
										  if1,
										  NULL, NULL);
	g_assert(proxy != NULL);
	GVariant *ret = NULL;
	if (strcmp(method, "Get") == 0)
	{
		ret = g_dbus_proxy_call_sync(proxy,
									 "Get",
									 g_variant_new("(ss)", if2, property),
									 G_DBUS_CALL_FLAGS_NONE, -1,
									 NULL, &error);
	}

	if (ret)
	{
		GVariant *vvalue = NULL;
		if (strcmp(property, "ActiveConnection") == 0)
		{
			char *conn = NULL;
			//这里要先用v取出值，d-feet上输出是o，但是代码里输出却是v
			g_variant_get(ret, "(v)", &vvalue);
			g_variant_get(vvalue, "o", &conn);
			g_print("current active Connection %s\n", conn);
			if (strstr(conn, "/NetworkManager/ActiveConnection") != NULL)
			{
				strcpy(value, conn);
				status = TRUE;
			}
			else
			{
				strcpy(err, "current no active connection");
				status = FALSE;
			}
			g_variant_unref(ret);
			g_object_unref(proxy);

			//wrx	status放到最后面再去return.
			if (conn != NULL)
			{
				free(conn);
			}
			//return status;
		}
		else if (strcmp(property, "Connection") == 0)
		{
			char *conn = NULL;
			//这里要先用v取出值，d-feet上输出是o，但是代码里输出却是v
			g_variant_get(ret, "(v)", &vvalue);
			g_variant_get(vvalue, "o", &conn);
			g_print("current active Connection path %s\n", conn);

			strcpy(value, conn);
			g_variant_unref(ret);
			g_object_unref(proxy);
			status = TRUE;

			//wrx	status放到最后面再去return.
			if (conn != NULL)
			{
				free(conn);
			}
			//return status;
		}
		else if (strcmp(property, "Interface") == 0)
		{
			char *iface = NULL;
			//这里要先用v取出值，d-feet上输出是o，但是代码里输出却是v
			g_variant_get(ret, "(v)", &vvalue);
			g_variant_get(vvalue, "s", &iface);
			g_print("current interface %s\n", iface);
			strcpy(value, iface);
			g_variant_unref(ret);
			g_object_unref(proxy);
			status = TRUE;

			//wrx	status放到最后面再去return.
			if (iface != NULL)
			{
				free(iface);
			}
			//return status;
		}
		//这一段后期再改，需要用到c++
		else if (strcmp(property, "State") == 0 || strcmp(property, "state") == 0)
		{
			//const char *conn = NULL;
			//这里要先用v取出值，d-feet上输出是o，但是代码里输出却是v
			g_variant_get(ret, "(u)", &value);
			//g_variant_get (vvalue, "o", &conn);
			//g_print("current active Connection path %s\n", conn);

			//strcpy(value, conn);
			g_variant_unref(ret);
			g_object_unref(proxy);
			status = TRUE;

			//wrx	status放到最后面再去return.
			//return status;
		}

		//wrx	这里要去释放掉vvalue
		if (vvalue != NULL)
		{
			g_variant_unref(vvalue);
		}
		return status;
	}
	else
	{
		g_dbus_error_strip_remote_error(error);
		g_print("Error get property %s, %s\n", property, error->message);
		strcpy(err, error->message);
		g_clear_error(&error);
		g_object_unref(proxy);
		return FALSE;
	}
}

gboolean
get_property_r(const char *path, const char *if1, const char *method, const char *if2, const char *property, int *value, char *err)
{
	GDBusProxy *proxy;
	gboolean status = FALSE;
	GError *error = NULL;
	/* Create a D-Bus proxy; NM_DBUS_* defined in nm-dbus-interface.h */
	proxy = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM,
										  G_DBUS_PROXY_FLAGS_NONE,
										  NULL,
										  // "org.freedesktop.NetworkManager"
										  NM_DBUS_SERVICE,
										  path,
										  if1,
										  NULL, NULL);
	g_assert(proxy != NULL);
	GVariant *ret = NULL;
	if (strcmp(method, "Get") == 0)
	{
		ret = g_dbus_proxy_call_sync(proxy,
									 "Get",
									 g_variant_new("(ss)", if2, property),
									 G_DBUS_CALL_FLAGS_NONE, -1,
									 NULL, &error);
	}

	if (ret)
	{

		//这一段后期再改，需要用到c++
		//d-feet上看state是uint32，但是代码却提示是v
		if (strcmp(property, "State") == 0 || strcmp(property, "state") == 0)
		{
			//const char *conn = NULL;
			GVariant *vvalue;
			g_variant_get(ret, "(v)", &vvalue);
			g_variant_get(vvalue, "u", value);
			//g_variant_get (vvalue, "o", &conn);
			//g_print("current active Connection path %s\n", conn);

			//strcpy(value, conn);
			g_variant_unref(ret);
			g_object_unref(proxy);
			status = TRUE;
			return status;
		}
	}
	else
	{
		g_dbus_error_strip_remote_error(error);
		g_print("Error get property %s, %s\n", property, error->message);
		strcpy(err, error->message);
		g_clear_error(&error);
		g_object_unref(proxy);
		return FALSE;
	}
}

gboolean
disc_wifi_fun(char *err)
{

	//wrx	变量初始化为NULL
	GDBusProxy *proxy = NULL;
	gboolean status = FALSE;
	GError *error = NULL;
	/* Create a D-Bus proxy; NM_DBUS_* defined in nm-dbus-interface.h */
	GVariant *ret = NULL;
	char value[200] = {};
	//status = check_online(err);
	//get_property(const char *path, const char * if1, const char *method, const char *if2, const char* property, char *value, char *err);

	status = get_property(WIFIDEVICE, "org.freedesktop.DBus.Properties", "Get", "org.freedesktop.NetworkManager.Device", "ActiveConnection", value, err);
	if (status == TRUE)
	{
		if (strstr(value, "/NetworkManager/ActiveConnection") != NULL)
		{
			proxy = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM,
												  G_DBUS_PROXY_FLAGS_NONE,
												  NULL,
												  // "org.freedesktop.NetworkManager"
												  NM_DBUS_SERVICE,
												  WIFIDEVICE,
												  "org.freedesktop.NetworkManager.Device",
												  NULL, NULL);
			g_assert(proxy != NULL);
			ret = g_dbus_proxy_call_sync(proxy,
										 "Disconnect",
										 NULL,
										 G_DBUS_CALL_FLAGS_NONE, -1,
										 NULL, &error);

			if (ret)
			{
				g_variant_unref(ret);
				g_object_unref(proxy);
				return TRUE;
			}
			else
			{
				g_dbus_error_strip_remote_error(error);
				g_print("Error disconnet wifi: %s\n", error->message);
				strcpy(err, error->message);
				g_clear_error(&error);
				g_object_unref(proxy);
				return FALSE;
			}
		}
		else
		{
			strcpy(err, "current no active connection");
			status = FALSE;
		}
	}
	else
	{
		status = FALSE;
	}

	return status;
}

/*add_wifi_connection->add_connection*/
gboolean
add_connection(const char *iface, const char *con_name, const char *pw, const bool dft_route, char *err)

{
	//wrx 	变量初始化
	GDBusProxy *proxy = NULL;
	GError *error = NULL;

	/* Create a D-Bus proxy; NM_DBUS_* defined in nm-dbus-interface.h */
	proxy = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM,
										  G_DBUS_PROXY_FLAGS_NONE,
										  NULL,
										  NM_DBUS_SERVICE,
										  //"/org/freedesktop/NetworkManager/Settings"
										  NM_DBUS_PATH_SETTINGS,
										  // "org.freedesktop.NetworkManager.Settings"
										  NM_DBUS_INTERFACE_SETTINGS,
										  NULL, &error);
	if (!proxy)
	{
		g_dbus_error_strip_remote_error(error);
		g_print("Could not create NetworkManager D-Bus proxy: %s\n", error->message);
		strcpy(err, error->message);
		g_error_free(error);
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
	g_variant_builder_init(&connection_builder, G_VARIANT_TYPE("a{sa{sv}}"));

	/* Build up the 'connection' Setting */
	g_variant_builder_init(&setting_builder, G_VARIANT_TYPE("a{sv}"));

	uuid = nm_utils_uuid_generate();
	g_variant_builder_add(&setting_builder, "{sv}",
						  NM_SETTING_CONNECTION_UUID,
						  g_variant_new_string(uuid));
	g_free(uuid);

	g_variant_builder_add(&setting_builder, "{sv}",
						  //"id"
						  NM_SETTING_CONNECTION_ID,
						  g_variant_new_string(con_name));
	g_variant_builder_add(&setting_builder, "{sv}",
						  //"type"
						  NM_SETTING_CONNECTION_TYPE,
						  //802-11-wireless
						  g_variant_new_string(NM_SETTING_WIRELESS_SETTING_NAME));
	g_variant_builder_add(&setting_builder, "{sv}",
						  //"interface-name"
						  NM_SETTING_CONNECTION_INTERFACE_NAME,
						  g_variant_new_string(iface));
	//自动连接
	g_variant_builder_add(&setting_builder, "{sv}",
						  //"type"
						  NM_SETTING_CONNECTION_AUTOCONNECT,
						  g_variant_new_boolean(FALSE));

	g_variant_builder_add(&connection_builder, "{sa{sv}}",
						  NM_SETTING_CONNECTION_SETTING_NAME,
						  &setting_builder);

	/* Add the (empty) 'wireless' Setting */
	g_variant_builder_init(&setting_builder, G_VARIANT_TYPE("a{sv}"));

	GVariantBuilder *builder;
	GVariant *value;

	//ssid是一个字符数组
	builder = g_variant_builder_new(G_VARIANT_TYPE("ay"));
	for (int i = 0; i < strlen(con_name); i++)
		g_variant_builder_add(builder, "y", con_name[i]);
	//g_variant_builder_add (builder, "y", 'i');
	//g_variant_builder_add (builder, "y", 'k');
	//g_variant_builder_add (builder, "y", 'o');
	//g_variant_builder_add (builder, "y", 'n');
	//g_variant_builder_add (builder, "y", 'g');
	value = g_variant_new("ay", builder);
	g_variant_builder_unref(builder);

	g_variant_builder_add(&setting_builder, "{sv}",
						  //"ssid" GArray_guchar_ *
						  NM_SETTING_WIRELESS_SSID,
						  value);
	g_variant_builder_add(&connection_builder, "{sa{sv}}",
						  NM_SETTING_WIRELESS_SETTING_NAME,
						  &setting_builder);
	/* Build up the 'wifi-security' Setting */
	g_variant_builder_init(&setting_builder, G_VARIANT_TYPE("a{sv}"));
	g_variant_builder_add(&setting_builder, "{sv}",
						  NM_SETTING_WIRELESS_SECURITY_KEY_MGMT,
						  g_variant_new_string("wpa-psk"));
	g_variant_builder_add(&setting_builder, "{sv}",
						  NM_SETTING_WIRELESS_SECURITY_PSK,
						  g_variant_new_string(pw));

	g_variant_builder_add(&connection_builder, "{sa{sv}}",
						  NM_SETTING_WIRELESS_SECURITY_SETTING_NAME,
						  &setting_builder);

	/* Build up the 'ipv4' Setting */
	g_variant_builder_init(&setting_builder, G_VARIANT_TYPE("a{sv}"));
	g_variant_builder_add(&setting_builder, "{sv}",
						  NM_SETTING_IP_CONFIG_METHOD,
						  g_variant_new_string(NM_SETTING_IP4_CONFIG_METHOD_AUTO));
	/*不作为主路由*/
	if (dft_route == false)
	{
		g_variant_builder_add(&setting_builder, "{sv}",
							  NM_SETTING_IP_CONFIG_NEVER_DEFAULT,
							  g_variant_new_boolean(TRUE));
	}

	g_variant_builder_add(&connection_builder, "{sa{sv}}",
						  NM_SETTING_IP4_CONFIG_SETTING_NAME,
						  &setting_builder);

	/* Call AddConnection with the connection dictionary as argument.
	 * (g_variant_new() will consume the floating GVariant returned from
	 * &connection_builder, and g_dbus_proxy_call_sync() will consume the
	 * floating variant returned from g_variant_new(), so no cleanup is needed.
	 */
	ret = g_dbus_proxy_call_sync(proxy,
								 "AddConnection",
								 g_variant_new("(a{sa{sv}})", &connection_builder),
								 G_DBUS_CALL_FLAGS_NONE, -1,
								 NULL, &error);
	if (ret)
	{
		g_variant_get(ret, "(&o)", &new_con_path);
		g_print("Added: %s\n", new_con_path);
		memcpy(PATH, new_con_path, strlen(new_con_path));
		g_variant_unref(ret);
		g_object_unref(proxy);
		return TRUE;
	}
	else
	{
		g_dbus_error_strip_remote_error(error);
		g_print("Error adding connection: %s\n", error->message);
		strcpy(err, error->message);
		g_clear_error(&error);
		g_object_unref(proxy);
		return FALSE;
	}
}

gboolean
get_active_connection_details(const char *obj_path, const char *ssido)
{
	//wrx 	初始化变量为NULL
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
	props_proxy = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM,
												G_DBUS_PROXY_FLAGS_NONE,
												NULL,
												//"org.freedesktop.NetworkManager"
												NM_DBUS_SERVICE,
												obj_path,
												"org.freedesktop.NetworkManager.Settings.Connection",
												NULL, NULL);
	g_assert(props_proxy);

	/* Get the object path of the Connection details */
	ret = g_dbus_proxy_call_sync(props_proxy,
								 "GetSettings",
								 NULL,
								 G_DBUS_CALL_FLAGS_NONE, -1,
								 NULL, &error);
	if (!ret)
	{
		g_dbus_error_strip_remote_error(error);
		g_warning("Failed to get active connection Connection property: %s\n",
				  error->message);
		g_error_free(error);
		goto out;
	}

	g_variant_get(ret, "(@a{sa{sv}})", &path_value);
	gboolean foundTk = FALSE;

	GVariant *s_con = NULL;
	gboolean found;
	const char *type, *id;
	//const gchar *ssid;
	gchar ssid[100];
	GVariant *a = NULL;
	s_con = g_variant_lookup_value(path_value, NM_SETTING_CONNECTION_SETTING_NAME, NULL);
	found = g_variant_lookup(s_con, NM_SETTING_CONNECTION_TYPE, "&s", &type);
	g_assert(found);
	found = g_variant_lookup(s_con, NM_SETTING_CONNECTION_ID, "&s", &id);
	g_assert(found);
	int length = 0;

	//wrx	wireless变量拿到外面,最后面再去释放
	GVariant *wireless = NULL;

	if (strcmp(type, "802-11-wireless") == 0)
	{
		//得到802-11-wireless字段

		wireless = g_variant_lookup_value(path_value, NM_SETTING_WIRELESS_SETTING_NAME, NULL);
		//found = g_variant_lookup (wireless, NM_SETTING_WIRELESS_SSID, "^ay", str);
		a = g_variant_lookup_value(wireless, NM_SETTING_WIRELESS_SSID, NULL);
		//	str = g_variant_get_bytestring (ssid);
		//printf("path %s <==> ssid %s\n", obj_path, str);

		//g_assert (found);

		GVariantIter *iter;
		g_variant_get(a, "ay", &iter);
		gchar s;
		while (g_variant_iter_loop(iter, "y", &s))
		{
			ssid[length] = s;
			//g_print("%c", s);
			length++;
		}
		//g_print("length %d %d\n", i, strlen(id));

		//凡是指針都要釋放
		g_variant_iter_free(iter);
		//g_variant_get(a, "^ay", &ssid);
		//printf("path %s <==> ssid %s\n", obj_path, ssid);
		//ssid = g_variant_get_bytestring(a);
		//ssid里是字符数组，不是放的字符串，所以要循环比对
		int j = 0;
		if (length == strlen(ssido))
		{
			for (j = 0; j < length; j++)
			{
				if (ssid[j] != ssido[j])
				{
					break;
				}
			}
		}
		if (j == length)
		{
			printf("path: %s, connection name: %s, ssid: ", obj_path, id);
			for (int j = 0; j < length; j++)
				g_print("%c", ssid[j]);
			g_print("\n");
			foundTk = TRUE;
		}
	}
	//if(strcmp(id, ssid) == 0 )
	//{
	//    foundTk = TRUE;
	//}

	/* Connection setting first */
	//print_setting (NM_SETTING_CONNECTION_SETTING_NAME, s_con);

	//path = g_variant_get_string (path_value, NULL);

	/* Print out the actual connection details */
	//g_print(path);

	//g_free(str);

	//wrx		这里必须去释放掉多个变量
	if (wireless != NULL)
	{
		g_variant_unref(wireless);
	}
	if (a != NULL)
	{
		g_variant_unref(a);
	}
	if (s_con != NULL)
	{
		g_variant_unref(s_con);
	}

out:
	if (path_value)
		g_variant_unref(path_value);
	if (ret)
		g_variant_unref(ret);
	g_object_unref(props_proxy);
	return foundTk;
}

/*check_exist->find_tk_wifi*/
gboolean
find_tk_wifi(GDBusProxy *proxy, const char *ssid)
{
	//wrx  初始化变量为NULL
	int i;
	GError *error = NULL;
	GVariant *ret = NULL;
	char **paths = NULL;

	/* Call ListConnections D-Bus method */
	ret = g_dbus_proxy_call_sync(proxy,
								 "ListConnections",
								 NULL,
								 G_DBUS_CALL_FLAGS_NONE, -1,
								 NULL, &error);
	if (!ret)
	{
		g_dbus_error_strip_remote_error(error);
		g_print("ListConnections failed: %s\n", error->message);
		g_error_free(error);
		return FALSE;
	}

	g_variant_get(ret, "(^ao)", &paths);
	g_variant_unref(ret);
	gboolean found = FALSE;

	for (i = 0; paths[i]; i++)
	{
		//g_print ("%s\n", paths[i]);
		//    if(strstr(paths[i], "7") != NULL )
		{
			//g_print("%s\n", paths[i]);
			//GVariant *value = NULL;
			//返回的是Variant，这里取出来
			//g_variant_get (paths[i], "(v)", &value);
			found = get_active_connection_details(paths[i], ssid);
			if (found == TRUE)
			{
				memcpy(PATH, paths[i], strlen(paths[i]));
				//goto OUT;
				break;
			}

			/* And print out the details for each active connection */
			// for (i = 0; paths1[i]; i++) {
			//     g_print ("Active connection path: %s\n", paths1[i]);
			//     get_active_connection_details (paths1[i]);
			// }
			//  g_strfreev (paths1);
		}
	}
OUT:
	g_strfreev(paths);
	return found;
}

//最终删除连接的函数
gboolean
remove_fun(const char *obj_path, char *err)
{
	GDBusProxy *props_proxy;
	GVariant *ret = NULL, *path_value = NULL;
	//const char *path = NULL;
	GError *error = NULL;

	gboolean status = TRUE;
	props_proxy = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM,
												G_DBUS_PROXY_FLAGS_NONE,
												NULL,
												//"org.freedesktop.NetworkManager"
												NM_DBUS_SERVICE,
												obj_path,
												"org.freedesktop.NetworkManager.Settings.Connection",
												NULL, NULL);
	g_assert(props_proxy);

	/* Get the object path of the Connection details */
	ret = g_dbus_proxy_call_sync(props_proxy,
								 "Delete",
								 NULL,
								 G_DBUS_CALL_FLAGS_NONE, -1,
								 NULL, &error);
	if (!ret)
	{
		g_dbus_error_strip_remote_error(error);
		//g_warning ("Failed to delete conn: %s\n",
		// error->message);
		strcpy(err, "Failed to delete conn: ");
		strcat(err, error->message);
		g_error_free(error);
		g_print("can't remove %s\n", obj_path);
		status = FALSE;
		goto out;
	}
	g_print("remove %s\n", obj_path);

out:
	if (path_value)
		g_variant_unref(path_value);
	if (ret)
		g_variant_unref(ret);
	g_object_unref(props_proxy);

	return status;
}

/*找到硬件路径*/
gboolean
find_hw_r(const char *iface, char *device_path, char *err)
{
	GDBusProxy *proxy = NULL;
	gboolean status = FALSE, found = FALSE;

	/* Create a D-Bus proxy; NM_DBUS_* defined in nm-dbus-interface.h */
	proxy = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM,
										  G_DBUS_PROXY_FLAGS_NONE,
										  NULL,
										  // "org.freedesktop.NetworkManager"
										  NM_DBUS_SERVICE,
										  //"/org/freedesktop/NetworkManager"
										  NM_DBUS_PATH,
										  //"org.freedesktop.NetworkManager"
										  //NM_DBUS_INTERFACE,
										  "org.freedesktop.DBus.Properties",
										  NULL, NULL);

	if(proxy == NULL) return found;

	g_assert(proxy != NULL);
	int i;
	GError *error = NULL;
	GVariant *ret = NULL;
	//char **paths;
	//这里也可以用GetDevices来得到设备
	/* Call ListConnections D-Bus method */
	ret = g_dbus_proxy_call_sync(proxy,
								 "Get",
								 g_variant_new("(ss)", "org.freedesktop.NetworkManager", "AllDevices"),
								 //g_variant_new ("(ss)", "org.freedesktop.NetworkManager","NetworkingEnabled"),
								 //g_variant_new ("(ss)", "org.freedesktop.NetworkManager","Version"),
								 G_DBUS_CALL_FLAGS_NONE, -1,
								 NULL, &error);
	if (!ret)
	{
		g_dbus_error_strip_remote_error(error);
		g_print("get failed: %s\n", error->message);
		g_error_free(error);

		//wrx	这里必须去释放掉proxy
		g_object_unref(proxy);
		return FALSE;
	}

	//status = get_property_r(device_path, "org.freedesktop.DBus.Properties", "Get", "org.freedesktop.NetworkManager.Device", "state",&state, err);

	GVariant *device_value = NULL;
	g_variant_get(ret, "(v)", &device_value);

	g_variant_unref(ret);

	GVariantIter *iter;
	//GVariant * item;
	//iter = g_variant_iter_new(device_value);
	//g_variant_iter_init(&iter,device_value);
	//先變成array數組
	g_variant_get(device_value, "ao", &iter);
	const char *path = NULL;
	//int j = 0;
	//然后从数组里看
	while (g_variant_iter_loop(iter, "o", &path))
	//g_variant_get_type_string(device_value);
	//for (i = 0; i < g_variant_n_children(device_value); i++)
	{

		printf("check %s\n", path);
		char interface[100] = {};
		status = get_property(path, "org.freedesktop.DBus.Properties", "Get", "org.freedesktop.NetworkManager.Device", "Interface", interface, err);

		if (status == TRUE)
		{
			if (strcmp(interface, iface) == 0)
			{
				strcpy(device_path, path);
				g_print("get hw interface %s\n", device_path);
				found = TRUE;
				break;
			}
		}
	}
	//凡是指針都要釋放
	g_variant_iter_free(iter);

	//wrx	这里要去释放掉device_value
	if (device_value != NULL)
	{
		g_variant_unref(device_value);
	}

	g_object_unref(proxy);

	return found;
}

/*找到硬件路径*/
gboolean
find_hw(const char *iface)
{
	//wrx	指针变量初始化为NULL
	GDBusProxy *proxy = NULL;
	gboolean found = FALSE;

	/* Create a D-Bus proxy; NM_DBUS_* defined in nm-dbus-interface.h */
	proxy = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM,
										  G_DBUS_PROXY_FLAGS_NONE,
										  NULL,
										  // "org.freedesktop.NetworkManager"
										  NM_DBUS_SERVICE,
										  //"/org/freedesktop/NetworkManager"
										  NM_DBUS_PATH,
										  //"org.freedesktop.NetworkManager"
										  //NM_DBUS_INTERFACE,
										  "org.freedesktop.DBus.Properties",
										  NULL, NULL);

	if(proxy == NULL) return found;

	g_assert(proxy != NULL);
	int i;
	GError *error = NULL;

	//wrx	指针变量初始化为NULL
	GVariant *ret = NULL;
	//char **paths;
	//这里也可以用GetDevices来得到设备
	/* Call ListConnections D-Bus method */
	ret = g_dbus_proxy_call_sync(proxy,
								 "Get",
								 //g_variant_new ("(a{sa{sv}})", &connection_builder),
								 g_variant_new("(ss)", "org.freedesktop.NetworkManager", "AllDevices"),
								 //g_variant_new ("(ss)", "org.freedesktop.NetworkManager","NetworkingEnabled"),
								 //g_variant_new ("(ss)", "org.freedesktop.NetworkManager","Version"),
								 G_DBUS_CALL_FLAGS_NONE, -1,
								 NULL, &error);
	if (!ret)
	{
		g_dbus_error_strip_remote_error(error);
		g_print("get failed: %s\n", error->message);
		g_error_free(error);

		//wrx	这里必须去释放掉proxy
		g_object_unref(proxy);
		return FALSE;
	}

	//wrx	指针变量初始化为NULL
	GVariant *device_value = NULL;
	g_variant_get(ret, "(v)", &device_value);

	g_variant_unref(ret);

	GVariantIter *iter = NULL;
	//GVariant * item;
	//iter = g_variant_iter_new(device_value);
	//g_variant_iter_init(&iter,device_value);
	//先變成array數組
	g_variant_get(device_value, "ao", &iter);

	//wrx	指针变量初始化为NULL
	const char *path = NULL;
	//int j = 0;
	//然後從數組裏拿
	while (g_variant_iter_loop(iter, "o", &path))
	//g_variant_get_type_string(device_value);
	//for (i = 0; i < g_variant_n_children(device_value); i++)
	{

		//printf("check %s\n", path);
		//這裏再去確認这个path是不是wifi接口
		found = is_wifi(path, iface);

		if (found == TRUE)
		{
			memcpy(WIFIDEVICE, path, strlen(path));
			break;
		}
	}
	//凡是指針都要釋放
	g_variant_iter_free(iter);

	//wrx	这里要去释放掉device_value
	if (device_value != NULL)
	{
		g_variant_unref(device_value);
	}

	g_object_unref(proxy);

	return found;
}

gboolean
active_conn(char *err)
{
	//wrx	变量初始化!
	GDBusProxy *proxy = NULL;
	GError *error = NULL;
	GVariant *ret = NULL;

	//g_print("then connect %s\n", SSID);

	/* Create a D-Bus proxy; NM_DBUS_* defined in nm-dbus-interface.h */
	proxy = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM,
										  G_DBUS_PROXY_FLAGS_NONE,
										  NULL,
										  // "org.freedesktop.NetworkManager"
										  NM_DBUS_SERVICE,
										  //"/org/freedesktop/NetworkManager"
										  NM_DBUS_PATH,
										  "org.freedesktop.DBus.Properties",
										  NULL, NULL);
	g_assert(proxy != NULL);
	//首先检查WirelessEnabled是否为TRUE
	//无脑enable，即使原来就是True，也没有问题,set没有返回值
	//NetworkingEnabled只能读

	GVariant *value = g_variant_new("b", TRUE);

	ret = g_dbus_proxy_call_sync(proxy,
								 "Get",
								 g_variant_new("(ss)", "org.freedesktop.NetworkManager", "NetworkingEnabled"),
								 //g_variant_new ("(ss)", "org.freedesktop.NetworkManager","NetworkingEnabled"),
								 G_DBUS_CALL_FLAGS_NONE, -1,
								 NULL, &error);
	if (ret)
	{
		gboolean status = FALSE;
		GVariant *v = NULL;
		g_variant_get(ret, "(v)", &v);
		g_variant_get(v, "b", &status);
		printf("network %s\n", (status == TRUE) ? "Enabled" : "Disabled");

		//wrx
		if (v != NULL)
		{
			g_variant_unref(v);
		}
	}
	else
	{
		g_dbus_error_strip_remote_error(error);
		strcpy(err, "read network status: ");
		strcat(err, error->message);
		g_clear_error(&error);
		g_object_unref(proxy);
		return FALSE;
	}

	//wrx 	释放变量,并且赋值为NULL
	g_variant_unref(ret);
	ret = NULL;

	ret = g_dbus_proxy_call_sync(proxy,
								 "Get",
								 g_variant_new("(ss)", "org.freedesktop.NetworkManager", "WirelessEnabled"),
								 G_DBUS_CALL_FLAGS_NONE, -1,
								 NULL, &error);
	if (ret)
	{
		gboolean status = FALSE;
		GVariant *v = NULL;
		g_variant_get(ret, "(v)", &v);
		g_variant_get(v, "b", &status);
		printf("wireless %s\n", (status == TRUE) ? "Enabled" : "Disabled");

		//wrx
		if (v != NULL)
		{
			g_variant_unref(v);
		}

		if (status == FALSE)
		{
			printf("set wireless enabled\n");
			g_dbus_proxy_call_sync(proxy,
								   "Set",
								   g_variant_new("(ssv)", "org.freedesktop.NetworkManager", "WirelessEnabled", value),
								   G_DBUS_CALL_FLAGS_NONE, -1,
								   NULL, &error);
			if (error != NULL)
			{
				g_dbus_error_strip_remote_error(error);
				strcpy(err, "set wireless enable: ");
				strcat(err, error->message);
				g_clear_error(&error);
				g_object_unref(proxy);

				//wrx
				g_variant_unref(ret);
				return FALSE;
			}
			sleep(2);
		}
	}

	g_object_unref(proxy);
	/* Create a D-Bus proxy; NM_DBUS_* defined in nm-dbus-interface.h */
	proxy = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM,
										  G_DBUS_PROXY_FLAGS_NONE,
										  NULL,
										  // "org.freedesktop.NetworkManager"
										  NM_DBUS_SERVICE,
										  //"/org/freedesktop/NetworkManager"
										  NM_DBUS_PATH,
										  "org.freedesktop.NetworkManager",
										  NULL, NULL);
	g_assert(proxy != NULL);
	//接着使能连接

	//wrx 	释放内存,并初始化为NULL,
	if (ret != NULL)
	{
		g_variant_unref(ret);
		ret = NULL;
	}

	ret = g_dbus_proxy_call_sync(proxy,
								 "ActivateConnection",
								 g_variant_new("(ooo)", PATH, WIFIDEVICE, "/"),
								 G_DBUS_CALL_FLAGS_NONE, -1,
								 NULL, &error);

	if (ret)
	{
		g_variant_unref(ret);
		g_object_unref(proxy);
		return TRUE;
	}
	else
	{
		g_dbus_error_strip_remote_error(error);
		g_print("Error active connection: %s\n", error->message);
		strcpy(err, error->message);
		g_clear_error(&error);
		g_object_unref(proxy);
		return FALSE;
	}
}

gboolean
is_wifi(const char *obj_path, const char *iface)
{
	//wrx	指针变量初始化为NULL
	GDBusProxy *props_proxy = NULL;
	GVariant *ret = NULL, *path_value = NULL;
	//const char *path = NULL;
	GError *error = NULL;
	gboolean found = FALSE;

	props_proxy = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM,
												G_DBUS_PROXY_FLAGS_NONE,
												NULL,
												//"org.freedesktop.NetworkManager"
												NM_DBUS_SERVICE,
												obj_path,
												"org.freedesktop.DBus.Introspectable",
												NULL, NULL);
	g_assert(props_proxy);

	ret = g_dbus_proxy_call_sync(props_proxy,
								 "Introspect",
								 NULL,
								 G_DBUS_CALL_FLAGS_NONE, -1,
								 NULL, &error);
	if (!ret)
	{
		g_dbus_error_strip_remote_error(error);
		g_warning("Failed to get Introspect: %s\n",
				  error->message);
		g_error_free(error);
		//g_print("can't find %s\n", obj_path);
		goto out;
	}
	char *xml = NULL;
	//GVariant *value;
	g_variant_get(ret, "(s)", &xml);
	//g_variant_unref (ret);
	//xml = g_variant_get_string (value, NULL);
	//g_print(xml);

	if (strstr(xml, "WirelessCapabilities") != NULL)
	{
		//g_print ("%s is wifi interface\n", obj_path);
		//首先unref一下，接着再去重新建立
		g_object_unref(props_proxy);
		//这里也要去对比iface，要和wlp3s0一样才行
		props_proxy = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM,
													G_DBUS_PROXY_FLAGS_NONE,
													NULL,
													//"org.freedesktop.NetworkManager"
													NM_DBUS_SERVICE,
													obj_path,
													"org.freedesktop.DBus.Properties",
													NULL, NULL);
		g_assert(props_proxy);

		//wrx
		//这里必须去释放掉ret,
		//这里也去释放一下ret
		g_variant_unref(ret);
		ret = NULL;

		ret = g_dbus_proxy_call_sync(props_proxy,
									 "Get",
									 g_variant_new("(ss)", "org.freedesktop.NetworkManager.Device", "Interface"),
									 G_DBUS_CALL_FLAGS_NONE, -1,
									 NULL, &error);

		if (ret)
		{
			GVariant *v = NULL;
			char *name = NULL;
			//GVariant *value;
			g_variant_get(ret, "(v)", &v);
			g_variant_get(v, "s", &name);
			if (strcmp(iface, name) == 0)
			{
				found = TRUE;
			}

			//wrx	这里必须去释放掉两个变量
			if (NULL != name)
			{
				free(name);
			}
			if (v != NULL)
			{
				g_variant_unref(v);
			}
		}
		else
		{
			g_dbus_error_strip_remote_error(error);
			g_warning("Failed to get Interface: %s\n",
					  error->message);
			g_error_free(error);
			//g_print("can't get interface name %s\n", obj_path);
			goto out;
		}
	}

out:
	//wrx	这里必须去释放掉之前使用的xml
	if (NULL != xml)
	{
		free(xml);
	}

	if (path_value)
		g_variant_unref(path_value);
	if (ret)
		g_variant_unref(ret);
	g_object_unref(props_proxy);

	return found;
}

#include <stdio.h>

gboolean exec(const char *cmd, char *ret)
{
	char buffer[500];
	FILE *pipe = popen(cmd, "r");
	if (!pipe)
	{
		strcpy(ret, "popen error");
	}
	else
	{
		auto no_warning = fgets(buffer, sizeof buffer, pipe);
		strcpy(ret, buffer);
	}
	pclose(pipe);
	return TRUE;
}

#if 0
gboolean get_change_ap(const char *obj_path, char *err)
{
	GDBusProxy *props_proxy;
	GVariant *ret = NULL, *path_value = NULL;
	//const char *path = NULL;
	GError *error = NULL;

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
		goto out;
	}

	g_variant_get (ret, "(@a{sa{sv}})", &path_value);
	gboolean foundTk = FALSE;

	GVariant *wireless= NULL;
		GVariant *a;

	wireless = g_variant_lookup_value (path_value, NM_SETTING_WIRELESS_SETTING_NAME, NULL);
	//found = g_variant_lookup (wireless, NM_SETTING_WIRELESS_SSID, "^ay", str);  
	a = g_variant_lookup_value (wireless, "mode", NULL);
//	str = g_variant_get_bytestring (ssid);
//printf("path %s <==> ssid %s\n", obj_path, str);
	const char * mode;
	g_variant_get (a, "s", &mode);
	//g_assert (found);
	if(strcmp(mode, "ap")== 0)
	{
		//增加或者修改auto属性
	}
		
out:
	if (path_value)
		g_variant_unref (path_value);
	if (ret)
		g_variant_unref (ret);
	g_object_unref (props_proxy);
    return foundTk;
}


gboolean  find_ap(const char *iface, char *err)
{
	GDBusProxy *proxy;
    gboolean found = FALSE;

	/* Create a D-Bus proxy; NM_DBUS_* defined in nm-dbus-interface.h */
	proxy = g_dbus_proxy_new_for_bus_sync (G_BUS_TYPE_SYSTEM,
                   G_DBUS_PROXY_FLAGS_NONE,
                   NULL,
                   NM_DBUS_SERVICE,
                   NM_DBUS_PATH_SETTINGS,
                   NM_DBUS_INTERFACE_SETTINGS,
                   NULL, NULL);
	g_assert (proxy != NULL);


	int i;
	GError *error = NULL;
	GVariant *ret;
	char **paths;

	/* Call ListConnections D-Bus method */
	ret = g_dbus_proxy_call_sync (proxy,
	                              "ListConnections",
	                              NULL,
	                              G_DBUS_CALL_FLAGS_NONE, -1,
	                              NULL, &error);
	if (!ret) {
		g_dbus_error_strip_remote_error (error);
		//g_print ("ListConnections failed: %s\n", error->message);
		strcpy(err, "ListConnections failed: ");
		strcat(err, error->message);
		g_error_free (error);
		return FALSE;
	}

	g_variant_get (ret, "(^ao)", &paths);
	g_variant_unref (ret);

	for (i = 0; paths[i]; i++)
    {        
        found = get_change_ap(paths[i], err);        
    }
OUT:
	g_strfreev (paths);
    return found;
}

#endif
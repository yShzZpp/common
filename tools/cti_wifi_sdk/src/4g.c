#include "4g.h"
#include <gio/gio.h>
#include <uuid/uuid.h>
#include <string.h>
#include <stdbool.h>
#include <nm-dbus-interface.h>
#include <NetworkManager.h>
#include <stdlib.h>
#include <stdio.h>

//最复杂的一次解析
int check_4g_signal(const char * iface, char *err)
{
//	gboolean 
//get_property(const char *path, const char * if1, const char *method, const char *if2, const char* property, char *value, char *err)
	GDBusProxy *proxy = NULL;
    gboolean status = FALSE;
    GError *error = NULL;
/* Create a D-Bus proxy; NM_DBUS_* defined in nm-dbus-interface.h */
	proxy = g_dbus_proxy_new_for_bus_sync (G_BUS_TYPE_SYSTEM,
                       G_DBUS_PROXY_FLAGS_NONE,
                       NULL,
                       "org.freedesktop.ModemManager1",
                       "/org/freedesktop/ModemManager1",                                          
						"org.freedesktop.DBus.ObjectManager",
                       NULL, NULL);
	g_assert (proxy != NULL);
	GVariant *ret = NULL;

	ret = g_dbus_proxy_call_sync (proxy,
            "GetManagedObjects",
			NULL,
	        G_DBUS_CALL_FLAGS_NONE, -1,
          	NULL, &error);
   
	if (ret) 
	{
		GVariantIter  *iter1, *iter2, *iter3, *iter4;
		const gchar *string, *string2,*key;
		GVariant *value;
		GVariant *value1;
		guint32 strength;
		gboolean quilty;

		//先变成array数组
		g_variant_get (ret, "(a{oa{sa{sv}}})", &iter1);
		while(g_variant_iter_loop(iter1, "{oa{sa{sv}}}", &string, &iter2))
		{
			g_print("%s\n",string);
			
			while(g_variant_iter_loop(iter2, "{sa{sv}}", &string2, &iter3))
			{
				//g_print("%s\n", string2);
				while(g_variant_iter_loop(iter3, "{sv}", &key, &value))
				{
					//g_print("%s\n", key);
					//if(strcmp(value, iface) == 0)
					{

					}
					if(strcmp(key, "SignalQuality") == 0)
					{
						g_variant_get(value,"(ub)", &strength, &quilty);
						g_print("signal strength %u\n", strength);
						sprintf(err, "%d",strength);
						//strcpy(ret, strength);
					}

				}
			}

		}
		//s_con = g_variant_lookup_value (value, NM_SETTING_CONNECTION_SETTING_NAME, NULL);
		/*found = g_variant_lookup (s_con, NM_SETTING_CONNECTION_TYPE, "&s", &type);
		g_assert (found);
		found = g_variant_lookup (s_con, NM_SETTING_CONNECTION_ID, "&s", &id);
		g_assert (found);*/


	} else {
		g_dbus_error_strip_remote_error (error);
		g_print ("Error get signal %s\n", error->message);
		strcpy(err, error->message);
		g_clear_error (&error); 
		g_object_unref (proxy);
		return 1;
	}
	if (ret)
		g_variant_unref (ret);
	g_object_unref (proxy);

}
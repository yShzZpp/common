/*
author: matt ji
date: 2019-8-29
brief: 关于wifi控制流程的函数接口
test on ubuntu 16.04
*/
/*1. 检查所有连接，看有没有特定连接(tikong-wifi)的名字 check_exist*/

#include "wifi-fun.h"
#include "cti_wifi_sdk/wifi.h"
#include <stdbool.h>
#include <gio/gio.h>
#include <uuid/uuid.h>
#include <string.h>
#include <nm-dbus-interface.h>
#include <NetworkManager.h>
#include <stdlib.h>
#include <stdio.h>


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

bool isVaildIp(const char *ip)
{
    int dots = 0; 
    int setions = 0;  
    int num = 1;

    if (NULL == ip || *ip == '.') { 
        return false;
    }   

    while (*ip) {

        if (*ip == '.') {
            dots ++; 
            if (setions >= 0 && setions <= 255) { 
                setions = 0;
                ip++;
                if(*ip >='0' && *ip <='9')
                	num++;
                continue;
            }   
            return false;
        }   
        else if (*ip >= '0' && *ip <= '9') { 
            setions = setions * 10 + (*ip - '0');
        } else 
            return false;
        ip++;   
    }   
    if (setions >= 0 && setions <= 255) {
        if (dots == 3 && num == 3) {
            return true;
        }   
    }   

    return false;
}

//目前这个函数只作为电梯控制设备的网络查询接口
int check_signal(const char * iface, char *err)
{
	int ret = -1;
	if(err == NULL)
	{
		//strcpy(ret,"pass memory bigger than 500"); //错误1
		return 1;
	}
	if(NULL == iface || strlen(iface) < 2)
	{
		strcpy(err,"pass correct interface"); //错误1
		return 2;
	}
	char device_path[100]={};
	gboolean status = find_hw_r(iface, device_path, err);
    if(FALSE == status)
    {
    	strcpy(err, "can't find interface ");
    	strcat(err, iface);
    	return 3;
    }
    //已经找到设备，并且把设备的路径放到device_path里了
    //接着查看state
    int state = 0;
    status = get_property_r(device_path, "org.freedesktop.DBus.Properties", "Get", "org.freedesktop.NetworkManager.Device", "State",&state, err);
    g_print("state %d\n", state);
    if(FALSE == status)
    {
    	return 5;
    }
    strcpy(err, "device state: ");
    switch(state)
    {
    	case 0:
    		strcat(err,"NM_DEVICE_STATE_UNKNOWN");
    		break;
    	case 10:
    		strcat(err,"NM_DEVICE_STATE_UNMANAGED");
    		break;
    	case 20:
    		strcat(err,"NM_DEVICE_STATE_UNAVAILABLE");
    		break;
    	case 30:
    		strcat(err,"NM_DEVICE_STATE_DISCONNECTED");
    		break;
    	case 40:
    		strcat(err,"NM_DEVICE_STATE_PREPARE");
    		break;
    	case 50:
    		strcat(err,"NM_DEVICE_STATE_CONFIG");
    		break;
    	case 60:
    		strcat(err,"NM_DEVICE_STATE_NEED_AUTH");
    		break;
    	case 70:
    		strcat(err,"NM_DEVICE_STATE_IP_CONFIG");
    		break;
    	case 80:
    		strcat(err,"NM_DEVICE_STATE_IP_CHECK");
    		break;
    	case 90:
    		strcat(err,"NM_DEVICE_STATE_SECONDARIES");
    		break;
    	case 100:
    		strcat(err, "NM_DEVICE_STATE_ACTIVATED");
    		break;
    	case 110:
    		strcat(err, "NM_DEVICE_STATE_DEACTIVATING");
    		break;
    	case 120:
    		strcat(err,"NM_DEVICE_STATE_FAILED");
    		break;
    	default:
    		strcat(err, "unkonwn state");
    		break;
    }

    //如果有活动连接，找到连接的路径
    //char active_path[100]={};
    //status = get_property(active_conn, "org.freedesktop.DBus.Properties", "Get","org.freedesktop.NetworkManager.Connection.Active", "Connection", active_path, err);

    //接着看网络整体的连接情况
    //d-feet上是state小写，但是代码运行却是State大写
    status = get_property_r("/org/freedesktop/NetworkManager", "org.freedesktop.DBus.Properties", "Get", "org.freedesktop.NetworkManager", "State", &state, err);
    g_print("state %d\n", state);
    strcat(err, ", network state: ");
   	switch(state)
    {
    	case 0:
    		strcat(err, "NM_STATE_UNKNOWN");
    		break;
    	case 10:
    		strcat(err, "NM_STATE_ASLEEP");
    		break;
    	case 20:
    		strcat(err, "NM_STATE_DISCONNECTED");
    		break;
    	case 30:
    		strcat(err, "NM_STATE_DISCONNECTING");
    		break;
    	case 40:
    		strcat(err, "NM_STATE_CONNECTING");    	
    		break;
    	case 50:
    		strcat(err, "NM_STATE_CONNECTED_LOCAL");  
    		break;
    	case 60:
    		strcat(err, "NM_STATE_CONNECTED_SITE");
    		break;
    	case 70:
    		strcat(err, "NM_STATE_CONNECTED_GLOBAL");
    		ret = 70;
    		break;
    	default:
    		strcat(err, "unkonwn state");
    		break;
    }
   
    return ret;
}


/*3. 删除连接 remove_conn()*/
int
remove_conn(const char *ssid, char *err)
{
		//wrx	这里先加到这里进行测试
	while (g_main_context_pending(NULL))
		g_main_context_iteration(NULL, TRUE);



	//if(err == NULL || sizeof(err) < 500)
	if(err == NULL)
	{
		//strcpy(ret,"pass memory bigger than 500"); //错误1
		return 1;
	}
	if(ssid == NULL || strlen(ssid) < 2)
	{
		strcpy(err,"pass correct ssid"); //错误1
		return 2;
	}

    GDBusProxy *proxy = NULL;
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
    
    //remove_tk_wifi (proxy);
    int i;
	GError *error = NULL;
	GVariant *ret = NULL;
	char **paths = NULL;

	/* Call ListConnections D-Bus method */
	//这里把所有的连接都会列出来
	ret = g_dbus_proxy_call_sync (proxy,
	                              "ListConnections",
	                              NULL,
	                              G_DBUS_CALL_FLAGS_NONE, -1,
	                              NULL, &error);
	if (!ret) {
		g_dbus_error_strip_remote_error (error);
		g_print ("ListConnections failed: %s\n", error->message);
		strcpy(err, "ListConnections: ");
		strcat(err, error->message);
		g_error_free (error);


		//wrx	释放掉proxy
		g_object_unref (proxy);
		return 1;
	}

	g_variant_get (ret, "(^ao)", &paths);
	g_variant_unref (ret);

//这里删除所有这个ssid上的连接
	//int match = 0;
	gboolean status = TRUE;
	for (i = 0; paths[i]; i++)
    {        
        found = get_active_connection_details (paths[i], ssid);
        if(found == TRUE)
        {
            status = remove_fun(paths[i], err);
            if(status == FALSE)
            {
            	break;
            }
        }       
        
    }
	g_strfreev (paths);
    
	g_object_unref (proxy);

	return ((status == TRUE) ? 0: 2);
}


 gboolean
check_exist (const char *ssid)
{
	//wrx	指针变量初始化为NULL
	GDBusProxy *proxy = NULL;
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

	/* List connections of system settings service */
    //check if exist    
	found = find_tk_wifi (proxy, ssid);
    if(found == TRUE)
    {
        //g_print("ok, find it\n");
        found = TRUE;
    }
    else
    {
        //创建连接
        //g_print("well, not found, create it then\n");
        found = FALSE;
        
    }
    
	g_object_unref (proxy);

	return found;
}



/*5. 断开wifi函数 disconn_wifi()*/
 int
disconnect_wifi(const char *iface, char *err)
{
	//wrx	这里先加到这里进行测试
	while (g_main_context_pending(NULL))
		g_main_context_iteration(NULL, TRUE);



	//if(err == NULL || sizeof(err) < 500)
	if(err == NULL)
	{
		//strcpy(err,"pass memory bigger than 500"); //错误1
		return 1;
	}
	if(iface == NULL || strlen(iface) < 2)
	{
		strcpy(err,"pass correct interface"); //错误1
		return 2;
	}

    gboolean found = find_hw(iface);
    gboolean status = FALSE;
    if(found == TRUE)
    {
    	status = disc_wifi_fun(err);
    	return ((status == TRUE) ? 0: 4);
    }
    else
    {
    	strcpy(err, "can't find interface ");
    	strcat(err, iface);
    	return 3;
    }
	
}
int list_connections(const char *ssid, char *err)
{
	//取得这个连接的所有信息
	GDBusProxy *proxy = NULL;
    gboolean found = FALSE;

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
		g_print ("ListConnections failed: %s\n", error->message);
		strcpy(err, "ListConnections: ");
		strcat(err, error->message);
		g_error_free (error);
		g_object_unref (proxy);
		return 1;
	}

	g_variant_get (ret, "(^ao)", &paths);
	g_variant_unref (ret);

	gboolean status = TRUE;
	int length = 0;
	for (i = 0; paths[i]; i++)
    {        
    	//如果这个路径的ssid是要找的
        found = get_active_connection_details (paths[i], ssid);
        if(found == TRUE)
        {
        	length +=strlen(paths[i]);
        }       
        
    }
    if(length < 200)
	{
		for (i = 0; paths[i]; i++)
    	{        
	    	//如果这个路径的ssid是要找的
	        found = get_active_connection_details (paths[i], ssid);
	        if(found == TRUE)
	        {
	        	strcat(err, paths[i]);
	        	strcat(err,",");
	        }        
    	}
	}
	else
	{
		strcpy(err, "too many connections on this ssid, need to remove all");
		status = FALSE;
	}
	g_strfreev (paths);
    
	g_object_unref (proxy);

	return ((status == TRUE) ? 0: 2);
}

/*6. 测试网络连通性函数 check_connectivity()*/
//这个函数先判断目前这个网卡上是否有活动连接，如果有，就去ping一个固定ip

//这里如果一连上就去ping会出现connect: Network is unreachable

int
check_connectivity(const char *iface, const char * ssid, const char *ip, char * err)
{
	//wrx	这里先加到这里进行测试
	while (g_main_context_pending(NULL))
		g_main_context_iteration(NULL, TRUE);

/*	int ret = -1;
	ret = system("ping 192.168.1.1");
//	ret = system("ping 192.168.1.1 -c 1");
	if(ret == 0)
	{
		strcpy(err, "network ok");
		return TRUE;
	}
	else
	{
		strcpy(err, "network not working");
		return FALSE;
	}
	//增加ping外网的需求，ping150主控
	*/
	//一种方法，一直去查询dbus的某一个值
	/*
	string cmd = "nmcli c add type wifi con-name tikong-wifi ifname ";
    	cmd.append(WLAN);
    	cmd.append(" ssid tikong");
        //system("nmcli c add type wifi con-name tikong-wifi ifname wlp3s0 ssid tikong");
        system(cmd.c_str());
	*/
	if(err == NULL)
	{
		//strcpy(ret,"pass memory bigger than 500"); //错误1
		return 1;
	}
	if(iface == NULL || ssid == NULL || ip == NULL)
	{
		strcpy(err,"some paramter is null"); //错误1
		return 2;
	}
	if(strlen(iface) < 2 || strlen(ssid) < 2 || strlen(ip) < 2)
	{
		strcpy(err,"pass correct paramter"); //错误1
		return 3;
	}
	if (isVaildIp(ip)) {
        
    } else {
        strcpy(err, "not valid network segment");
        return 4;
    }   
	//第一步检测是否连接上
	//找到iface对于的物理网卡
	char wifi_path[200] = {};
    gboolean found = find_hw(iface);
    gboolean status = FALSE;
    //get_property("/org/freedesktop/NetworkManager", "org.freedesktop.NetworkManager","AllDevices", wifi_path, err);
    if(found == TRUE)
    {
    	//接着找这个物理网卡所对应的活动连接
    	//status = check_online(err);
    	char active_conn[200] ={};
    	//这里可以用get state来得知是否连接上，但是下面的检测是否是目的ssid还是需要用到
    	//active connection path
    	status = get_property(WIFIDEVICE, "org.freedesktop.DBus.Properties", "Get", "org.freedesktop.NetworkManager.Device", "ActiveConnection",active_conn, err);
    	if(status == FALSE)
    	{
    		strcpy(err, "no active connection now");
    		return 1;
    	}
    	else
    	{
    		//如果目前有活动连接，检测出目前连接的path
    		char active_path[200]={};
    		status = get_property(active_conn, "org.freedesktop.DBus.Properties", "Get","org.freedesktop.NetworkManager.Connection.Active", "Connection", active_path, err);
    		if(status == TRUE)
    		{
    			//拿到ssid
    			status = get_active_connection_details (active_path, ssid);
    			if(status == TRUE)
    			{
    				strcpy(err, iface);
    				strcat(err, " ");
    				strcat(err, ssid);
    				strcat(err, " is active, ");
    			}
    			else
    			{
    				strcpy(err, "current active connection is not ");
    				strcat(err, ssid);
    				return 2;
    			}
    		}
    		else
    		{
    			return 3;
    		}
    	}
    }
    else
    {
    	return 4;
    }

	char cmd[200]="";
	strcpy(cmd, "ifconfig | grep ");
	strcat(cmd, iface);
	strcat(cmd, " -A1 | grep -v Link");
	//exec("ifconfig | grep wlp2s0 -A1 | grep -v Link", err);


	//这里原来是把命令行的输出结果放到err里面,因此这个参数就不应该命名为err,差点理解错了
	//!!!!!!!!!!!!!!!!!!!!!!
	exec(cmd, err);
	//exec("ifconfig | grep wlp3s0 -A1 | grep -v Link");
	//printf("%s\n", err);
	if(strstr(err, ip) != NULL)
	{
		memset(cmd, 0, sizeof cmd);
		strcpy(cmd, "ping ");
		strcat(cmd, ip);
		strcat(cmd, "1 -c 1");
		int ret = -1;
		ret = system(cmd);
		if(ret == 0)
		{
			strcat(err, "ping network ok");
			return 0;
		}
		else
		{
			strcpy(err, "ping network not working");
			return 5;
		}
	}
	else
	{
		strcat(err, "not get ip");
		return 6;
	}
	//这里应该提供信号强度
} 


int connect_wifi(const char *iface, const char *ssid, const char *pw, const int dft_route, char *ret)
{ 
	//wrx	这里先加到这里进行测试
	while (g_main_context_pending(NULL))
		g_main_context_iteration(NULL, TRUE);

	//if(ret ==NULL || sizeof(ret) < 500)
	if(ret == NULL)
	{
		//strcpy(ret,"pass memory bigger than 500"); //错误1
		return 1;
	}
	if(iface == NULL || ssid == NULL || pw == NULL)
	{
		strcpy(ret,"some paramter is null"); //错误1
		return 2;
	}
	if(strlen(iface) < 2 || strlen(ssid) < 2 || strlen(pw) < 2)
	{
		strcpy(ret,"pass correct paramter"); //错误1
		return 3;
	}
	
	if(dft_route != 0 && dft_route != 1)
	{
		strcpy(ret, "set dft_route either 0 or 1");
		return 4;
	}
    gboolean status = FALSE;
    status = find_hw(iface);
    if(status == TRUE)
    {
        //printf("found wifi hardware\n");  
        //strcpy(ret,"found wifi hardware"); 
    }
    else if(status == FALSE)
    {
        strcpy(ret,"cant't fine interface "); //错误1
        strcat(ret, iface);
        return 5;
    }

    //check_exist函数里会把连接的PATH找到
    status = check_exist(ssid);
    if(status == TRUE)
    {
        printf("found wifi ssid\n");  
    }
    else if(status == FALSE)
    {
    	printf("need establish %s connection\n", ssid);
    	if(dft_route == 0)
    	{
    		status = add_connection (iface, ssid, pw, false,ret);
    	}
    	else if(dft_route == 1)
    	{
    		status = add_connection (iface, ssid, pw, true,ret);
    	}
        if(status == FALSE)
	    {
	    	printf("%s\n", ret); //错误2 已经拷贝进去
	        return 2;
	    }
	    else if(status == TRUE)
	    {
	    	printf("add connection ok\n"); 
    	}
    }
    //enable wifi connection
    status = active_conn(ret); //错误3
    if(status == TRUE)
    {
    	strcpy(ret,"connection enabled"); //正确1
    }
    else
    {
    	//错误信息已经在enable_conn函数里拷贝到ret里了  //错误3
    	return 3;
    }
    //sleep(8);
    return 0;

}

int get_version(char *ver)
{
	if(ver == NULL)
	{
		return 1;
	}
	if(sizeof(ver) < 5)
	{
		return 2;
	}
	strcpy(ver, "1.3");
	return 0;
}


#if 0
int disable_auto(const char *iface, char *err)
{
	gboolean status = FALSE;
	status = find_ap(iface, err);

	return ((status == TRUE)?0:1);
}
#endif

//修改某一个属性的值
#if 0
int update_property(const char *path, const char* property, const char* value, char * err)
{
	GDBusProxy *props_proxy;
	GVariant *ret = NULL, *setting = NULL;
	//const char *path = NULL;
	GError *error = NULL;
	gboolean status = TRUE;


	/* Create a D-Bus object proxy for the active connection object's properties */
	props_proxy = g_dbus_proxy_new_for_bus_sync (G_BUS_TYPE_SYSTEM,
                                 G_DBUS_PROXY_FLAGS_NONE,
                                 NULL,
                                 //"org.freedesktop.NetworkManager"
                                 NM_DBUS_SERVICE,
                                 path,
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
		status = FALSE;
		goto out;
	}

	g_variant_get (ret, "(@a{sa{sv}})", &setting);

	GVariant *connection = NULL;
	gboolean found;
	const char  *auto, *id;
	//const gchar *ssid;
	gchar ssid[100];
	GVariant *a;
	connection = g_variant_lookup_value (setting, NM_SETTING_CONNECTION_SETTING_NAME, NULL);
	found = g_variant_lookup (connection, "autoconnect", "&s", &auto);
	if(found == NULL)
	{
		//增加auto = false选项
	}
	//g_assert (found);
	else
	{
		if(strcmp(auto, "true") == 0)
		{
			//修改为false
		}
		else if(strcmp(auto, "false") == 0)
		{
			//不需要做什么动作
		}
	}

out:
	if (setting)
		g_variant_unref (setting);
	if (ret)
		g_variant_unref (ret);
	g_object_unref (props_proxy);
    return ((status == TRUE)?0:1);
	//修改值
	//写回
}
#endif
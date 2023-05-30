#include "dbus.h"
#include "wifi-fun.h"
#include "cti_wifi_sdk/ap.h"
// 增加设置热点功能
int connect_ap(const char *iface, const char *ssid, const char *pw, char *ret)
{
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
	
    gboolean status = FALSE;
    char device_path[100]={};
    //device path是硬件的地址
    status = find_hw_r(iface, device_path, ret);
    if(FALSE == status)
    {
        strcpy(ret, "can't find interface ");
        strcat(ret, iface);
        return 4;
    }

    if(status == TRUE)
    {
        //printf("found wifi hardware\n");  
        //strcpy(ret,"found wifi hardware"); 
    }
    else if(status == FALSE)
    {
        strcpy(ret,"cant't fine interface "); //错误1
        strcat(ret, iface);
        return 4;
    }
    //check_exist函数里会把连接的PATH找到,放到link_path
    char link_path[100]={};
    status = check_ap_exist(ssid, link_path, ret);
    if(status == TRUE)
    {
        printf("found ap ssid\n");  
    }
    else if(status == FALSE)
    {
    	printf("need establish %s connection\n", ssid);

    	status = add_ap_connection(iface, ssid, pw, link_path, ret);

        if(status == FALSE)
	    {
	    	printf("%s\n", ret); //错误2 已经拷贝进去
	        return 5;
	    }
	    else if(status == TRUE)
	    {
	    	printf("add connection ok\n"); 
    	}
    }
    //enable wifi connection
    status = active_ap_conn(device_path, link_path, ret); 
    if(status == TRUE)
    {
    	strcpy(ret,"connection enabled"); //正确1
    }
    else
    {
    	//错误信息已经在enable_conn函数里拷贝到ret里了  //错误3
    	return 6;
    }
    //sleep(8);
    return 0;
}
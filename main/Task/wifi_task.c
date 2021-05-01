/*
 * wifi_task.c
 *
 *  Created on: Sep 3, 2020
 *      Author: Yolo
 */
#include "../Common.h"
#include "wifi_task.h"
#include "../Interface/UserFile.h"
#include "../Task/tcp_server.h"
#include "../data_base/data_base.h"

static const char *TAG = "example";

/* FreeRTOS event group to signal when we are connected & ready to make a request */
EventGroupHandle_t wifi_event_group;

const int IPV4_GOTIP_BIT = BIT0;
const int IPV6_GOTIP_BIT = BIT1;

int WIFI_CONNECTED_BIT = BIT0; // @suppress("Symbol is not resolved")

flag_t xFlag =
	{
		.eWiFi_Status = eDisconnect,
		.eMQTT_Status = eDisconnect,
};

WifiStation_Info _WifiInfo = {
	.UserName = "thao123",
	.Password = "123456789",
};

//extern int Counter_msec;	// conter millisec

static void WifiReConnect(void);

static void wifi_init_sta(void);

static esp_err_t event_handler_wifi(void *ctx, system_event_t *event);

static void wait_for_ip(void);

void wifi_task(void *pvParameters)
{
	APP_LOGD("wifi task call");
	sprintf(DeviceData.WifiStation_Info.MAC_ADD, "%s", wifi_get_mac());
	wifi_init_sta();
	wait_for_ip();
	APP_LOGD("--- WifiTask: Station User name = %s, Password = %s\r\n", DeviceData.WifiStation_Info.UserName, DeviceData.WifiStation_Info.Password);

}

static esp_err_t event_handler_wifi(void *ctx, system_event_t *event)
{
	switch (event->event_id)
	{
	case SYSTEM_EVENT_STA_START:
		APP_LOGD("Wifi start.........");
		esp_wifi_connect();
		break;
	case SYSTEM_EVENT_STA_GOT_IP:
		printf("\r\n---  got ip:%s", ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip)); // @suppress("Field cannot be resolved")
		xEventGroupSetBits(wifi_event_group, IPV4_GOTIP_BIT);
		ESP_LOGI(TAG, "SYSTEM_EVENT_STA_GOT_IP");
		break;
	case SYSTEM_EVENT_STA_CONNECTED:
		APP_LOGD("--- wifi connected");
		DataBase_SetDeviceState(E_DEVICE_WIFI_CONNECTED);
		xEventGroupSetBits(wifi_event_group, IPV4_GOTIP_BIT);
		xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
		tcpip_adapter_create_ip6_linklocal(TCPIP_ADAPTER_IF_STA);
		break;
	case SYSTEM_EVENT_STA_DISCONNECTED:
		APP_LOGD("Wifi disconnect");
		DataBase_SetDeviceState(E_DEVICE_WIFI_RECONNECT);
		esp_wifi_connect();
		xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED_BIT);
		break;
	case SYSTEM_EVENT_AP_STA_GOT_IP6:
		xEventGroupSetBits(wifi_event_group, IPV6_GOTIP_BIT);
		ESP_LOGI(TAG, "SYSTEM_EVENT_STA_GOT_IP6");
		char *ip6 = ip6addr_ntoa(&event->event_info.got_ip6.ip6_info);
		ESP_LOGI(TAG, "IPv6: %s", ip6);
		break;
	default:
		break;
	}
	return ESP_OK;
}

char *wifi_get_mac(void)
{
	char *mac_add;
	mac_add = (char*) malloc (18+1);
	//Get the derived MAC address for each network interface
	uint8_t derived_mac_addr[6] = {0};
	//Get MAC address for WiFi Station interface
	ESP_ERROR_CHECK(esp_read_mac(derived_mac_addr, ESP_MAC_WIFI_STA));
	sprintf(mac_add, "%x:%x:%x:%x:%x:%x", derived_mac_addr[0], derived_mac_addr[1], derived_mac_addr[2], derived_mac_addr[3],
			derived_mac_addr[4], derived_mac_addr[5]);
	APP_LOGD("wifi_get_mac end = %s", mac_add);
	return mac_add;
}

static void wifi_init_sta(void)
{
	wifi_event_group = xEventGroupCreate();
	tcpip_adapter_init();
	ESP_ERROR_CHECK(esp_event_loop_init(event_handler_wifi, NULL)); // @suppress("Symbol is not resolved")
	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();			// @suppress("Symbol is not resolved")
	ESP_ERROR_CHECK(esp_wifi_init(&cfg));
	wifi_config_t xWifi_Config =
		{
			.sta =
				{
					.scan_method = WIFI_FAST_SCAN,
					.sort_method = WIFI_CONNECT_AP_BY_SIGNAL,
					.threshold.rssi = -127,
					.threshold.authmode = WIFI_AUTH_WPA2_PSK,
				},
		};

	strcpy((char *)xWifi_Config.sta.ssid, DeviceData.WifiStation_Info.UserName);
	strcpy((char *)xWifi_Config.sta.password, DeviceData.WifiStation_Info.Password);

	APP_LOGI("--- main: Station copy = %s, Password = %s", xWifi_Config.sta.ssid, xWifi_Config.sta.password);

	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
	ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &xWifi_Config));
	ESP_ERROR_CHECK(esp_wifi_start());
	vTaskDelay(100);
	esp_err_t ret = tcpip_adapter_set_hostname(TCPIP_ADAPTER_IF_STA, "ir_blaster");
	if (ret != ESP_OK)
	{
		APP_LOGE("failed to set hostname:%d", ret);
	}
}

static void wait_for_ip(void)
{
	uint32_t bits = WIFI_CONNECTED_BIT;

	ESP_LOGI(TAG, "Waiting for AP connection...");
	xEventGroupWaitBits(wifi_event_group, bits, false, true, portMAX_DELAY);
	ESP_LOGI(TAG, "Connected to AP");
}

static void WifiReConnect(void)
{
	esp_wifi_connect();
	xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED_BIT);
}

void wifi_disconnect(void)
{
	esp_wifi_disconnect();
}


void esp32_user_restart( void )
{
	esp_restart();
}

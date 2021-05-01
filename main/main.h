/*
 * ESP32_AC.h
 *
 *  Created on: Mar 4, 2019
 *      Author: giangnv3
 */

#ifndef MAIN_MAIN_H_
#define MAIN_MAIN_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_event_legacy.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "nvs.h"

#include "lwip/err.h"
#include "lwip/sys.h"
#include "tcpip_adapter.h"
#include "driver/gpio.h"

//===================


typedef enum
{
	eIdle = 0,
	eConnected,
	eDisconnect
}eStatus;



typedef struct
{
	eStatus eWiFi_Status;
	eStatus eMQTT_Status;

}flag_t;


#endif /* MAIN_MAIN_H_ */








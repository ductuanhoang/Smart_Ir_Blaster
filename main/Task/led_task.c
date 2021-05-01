/*
 * led_task.c
 *
 *  Created on: Sep 4, 2020
 *      Author: Yolo
 */
#include "../Common.h"
#include "led_task.h"
#include "../Led_driver/LedStatus.h"
#include "../Interface/UserTimer.h"
#include "../Interface/Logger_File/logger_file.h"
#include "../Interface/shell/app_cli.h"

static void led_task(void *pvParameters);

extern void SmartConf_EventStart( void );
extern void ble_create_task(void);


#define DELAY_SETUP 10
/***********************************************************************************************************************
* Function Name:
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void led_task_creat(void)
{
	xTaskCreatePinnedToCore(led_task, "led_task", 2 * 1024, NULL, 5 | portPRIVILEGE_BIT, NULL, 1);
}

static void led_blue_status_on(void)
{
	gpio_set_level(GPIO_OUTPUT_LED_BLUE, 1); /* On led*/
}

static void led_blue_status_off(void)
{
	gpio_set_level(GPIO_OUTPUT_LED_BLUE, 0); /* On led*/
}

static void led_red_status_on(void)
{
	gpio_set_level(GPIO_OUTPUT_LED_RED, 1); /* On led*/
}

static void led_red_status_off(void)
{
	gpio_set_level(GPIO_OUTPUT_LED_RED, 0); /* On led*/
}

static void led_green_status_on(void)
{
	gpio_set_level(GPIO_OUTPUT_LED_GREEN, 1); /* On led*/
}

static void led_green_status_off(void)
{
	gpio_set_level(GPIO_OUTPUT_LED_GREEN, 0); /* On led*/
}

static void led_task(void *pvParameters)
{

	//	vTaskDelay(5000);
	APP_LOGI("led task call");
	// init led
	vLedStatusSetGetTickCallback(usertimer_gettick);
	vLedStatusSetOnCallback(0, led_blue_status_on);
	vLedStatusSetOffCallback(0, led_blue_status_off);

	vLedStatusSetOnCallback(1, led_red_status_on);
	vLedStatusSetOffCallback(1, led_red_status_off);

	vLedStatusSetOnCallback(2, led_green_status_on);
	vLedStatusSetOffCallback(2, led_green_status_off);

	vLedStatusStart();

	led_blue_status_off();
	led_red_status_off();
	led_green_status_off();

	uint16_t time_check_button = 0;
	uint16_t time_check_nano_timer = 0;
	bool nanotimer_doggle = false;
	bool doing = false;
	while (1)
	{
		if ((DeviceData.eStateDevice == E_DEVICE_WIFI_CONNECTED) || (DeviceData.eStateDevice == E_DEVICE_NORMAL))
		{
			vLedStatusBlink(0, 10, 1000, 1000);
			led_green_status_off();
		}

		else if( (DeviceData.eStateDevice == E_DEVICE_WIFI_RECONNECT) && (DeviceData.SmartConfig_State == false))
		{
			vLedStatusBlink(0, 10, 100, 100);
			led_green_status_off();
		}

		else if (DeviceData.SmartConfig_State == true)
		{
			led_blue_status_off();
			vLedStatusBlink(2, 10, 100, 100);
		}


		if (gpio_get_level(GPIO_USER_BUTTON) == 0)
		{
			if ((time_check_button > (2000/DELAY_SETUP)) && (doing == false))
			{
				DeviceData.SmartConfig_State = true;
				DeviceData.eStateDevice = E_DEVICE_SMART_CONFIG;
				APP_LOGD("smart config");
				doing = true;
				ble_create_task();
			}
			time_check_button++;
		}


		if( (time_check_nano_timer > (1000/DELAY_SETUP)) && (time_check_nano_timer < (1055/DELAY_SETUP)))
		{
			gpio_set_level(GPIO_OUTPUT_NANO_TIMER, 1);
		}
		else if( time_check_nano_timer > (1055/DELAY_SETUP))
		{
			gpio_set_level(GPIO_OUTPUT_NANO_TIMER, 0);
			time_check_nano_timer = 0;
		}
		time_check_nano_timer++;

		vLedStatusTask();
		vTaskDelay(DELAY_SETUP);
	}
}

/* Simple WiFi Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include "main.h"
#include "Interface/UserTimer.h"
#include "Task/wifi_task.h"
#include "Interface/UserFile.h"
#include "Interface/UserUart.h"
#include "esp_spiffs.h"
#include <sys/unistd.h>
#include <sys/stat.h>

#include "Common.h"
#include "Task/tcp_server.h"
#include "Task/led_task.h"
#include "Task/bluetooth.h"
#include "Task/uart_task.h"

#include "Task/plantTaskCtrl.h"
#include "driver/uart.h"

#include "Ir_driver/ir_driver.h"

//include shell
#include "Interface/shell/vsm_retarget.h"
#include "Interface/shell/app_cli.h"
#include "Interface/UserUart.h"
#include "Interface/Logger_File/logger_file.h"


bluetooth_status_t bluetooth_status;
Device_Data_t DeviceData;

TaskHandle_t tcp_handle_task;
TaskHandle_t mqtt_handle_task;
TaskHandle_t ir_tx_handle_task;
TaskHandle_t ir_rx_handle_task;
TaskHandle_t bluetooth_handle_task;

/*****************************************************************************
 *
 * ****************************************************************************/
void GPIO_Init(void)
{
	gpio_config_t io_conf;
	//disable interrupt
	io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
	//set as output mode
	io_conf.mode = GPIO_MODE_OUTPUT;
	//bit mask of the pins that you want to set,e.g.GPIO18/19
	io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
	//disable pull-down mode
	io_conf.pull_down_en = 0;
	//disable pull-up mode
	io_conf.pull_up_en = 0;
	//configure GPIO with the given settings
	gpio_config(&io_conf);

	//interrupt of rising edge
	io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
	//bit mask of the pins, use GPIO0 here
	io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
	//set as input mode
	io_conf.mode = GPIO_MODE_INPUT;
	//enable pull-up mode
	io_conf.pull_up_en = 1;
	esp_err_t error = gpio_config(&io_conf); //configure GPIO with the given settings

	if (error != ESP_OK)
	{
		APP_LOGE("error configuring inputs\n");
	}
}
/*****************************************************************************
 *
 * ****************************************************************************/

void app_main()
{
	DeviceData.eCtrlStateDevice = E_DEVICE_CONTROL_LOCAL;
	DeviceData.eStateDevice = E_DEVICE_WIFI_AP_DISCONNECT;
	DeviceData.eIrState = E_DEVICE_IR_NORMAL;

	APP_LOGI("--- APP_MAIN: Smart Ir Blaster Update 24/02/2021......");
	APP_LOGI("--- APP_MAIN: Free memory: %d bytes", esp_get_free_heap_size());
	//Initialize NVS
	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
	{
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret);

//	//GPIO
	GPIO_Init();
//
	logger_file_init();
	logger_LoadInfo_WifiStation(&DeviceData, false);
//
//	/* Configure parameters of an UART driver,
//	* communication pins and install the driver */
//	// init shell
//	retarget_init_fifo();
//	vsm_cli_start();

//	/*2. Timer init and start*/

	UserTimer_Init();
	ir_driver_create_rx_task();
	ir_driver_create_tx_task();
//	xTaskCreatePinnedToCore(uart_task, "uart_task", 6 * 1024, NULL, 4 | portPRIVILEGE_BIT, NULL, 1);
	xTaskCreatePinnedToCore(PlantControl_Task, "plant_task", 6 * 1024, NULL, 2 | portPRIVILEGE_BIT, NULL, 0);
	led_task_creat();
//	xTaskCreate(SmartConf_Task, "SmartConf_Task", 2 * 1024, NULL, 2 | portPRIVILEGE_BIT, NULL);
	// vTaskSuspend(bluetooth_handle_task);
}

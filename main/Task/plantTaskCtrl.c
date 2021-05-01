/*
 * plantTaskCtrl.c
 *
 *  Created on: Nov 19, 2020
 *      Author: Yolo
 */
/***********************************************************************************************************************
* Pragma directive
***********************************************************************************************************************/

/***********************************************************************************************************************
* Includes <System Includes>
***********************************************************************************************************************/
#include "plantTaskCtrl.h"

#include "wifi_task.h"
#include "bluetooth.h"
#include "led_task.h"
#include "tcp_server.h"
#include "uart_task.h"
#include "../Ir_driver/ir_driver.h"

#include "../mqtt_driver/mqtt_driver.h"
/***********************************************************************************************************************
* Macro definitions
***********************************************************************************************************************/

//	xTaskCreate(led_task, "led_task", 4 * 1024, NULL, 2 | portPRIVILEGE_BIT, NULL);
// xTaskCreate(wifi_task, "wifi_task", 3 *2024, NULL, 2 | portPRIVILEGE_BIT, NULL);
//	xTaskCreate(ir_driver_rx_task, "ir_driver_rx_task", 3 *2024, NULL, 2 | portPRIVILEGE_BIT, NULL);
//	xTaskCreatePinnedToCore(ble_task, "ble_task", 3 * 1024, NULL, 2 | portPRIVILEGE_BIT, NULL, 0);

/***********************************************************************************************************************
* Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
* Private global variables and functions
***********************************************************************************************************************/
static bool check_change_state(E_StateDevice state);
static void change_next_state(E_StateDevice next_state);
static void plant_check_irState(E_IrState ir_state);
static void plant_check_StateDevice(E_StateDevice device_state);

/***********************************************************************************************************************
* Exported global variables and functions (to be accessed by other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
* Imported global variables and functions (from other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: 
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void PlantControl_Task(void *pvParameters)
{
    // create task in here
    while (1)
    {
        // check status
        plant_check_irState(DeviceData.eIrState);
        plant_check_StateDevice(DeviceData.eStateDevice);
        vTaskDelay(500);
    }
}

/***********************************************************************************************************************
* static functions
***********************************************************************************************************************/
static void plant_check_irState(E_IrState ir_state)
{
    switch (ir_state)
    {
		case E_DEVICE_IR_NORMAL: // send ir
			/* code */
			break;
		case E_DEVICE_IR_LEARN_CMD: // check rx enable
		{
//			ir_driver_create_rx_task();
			ir_driver_start();
			break;
		}
		case E_DEVICE_IR_END_LEARN_CMD:
//			ir_driver_delete_rx_task();
			ir_driver_stop();
			break;
		default:
			break;
    }
}

/***********************************************************************************************************************
* Function Name: 
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
bool change_device_state = true;

static void plant_check_StateDevice(E_StateDevice device_state)
{
    if (device_state == E_DEVICE_NORMAL)
    {
    }
    else if (check_change_state(E_DEVICE_SMART_CONFIG)) // task bluetooth on
    {
    	DeviceData.eStateDevice = E_DEVICE_SMART_CONFIG_DOING;
    	change_device_state = false;

    }
    else if( DeviceData.eStateDevice == E_DEVICE_SMART_CONFIG_DOING)
    {

    }
    else if (device_state == E_DEVICE_WIFI_RECONNECT)
    {

    }
    else if (device_state == E_DEVICE_WIFI_CONNECTED)
    {

    }
    else if (check_change_state(E_DEVICE_WIFI_AP_DISCONNECT))
    {
    	APP_LOGD("go to here");
        // kiem tra task socket neu ton tai thi xoa di
        tcp_server_delete_task();
        change_device_state = false;
        // goi bat wifi
        wifi_task(NULL);
//    	xTaskCreate(wifi_task, "wifi_task", 2 * 1024, NULL, 2 | portPRIVILEGE_BIT, NULL);
        //khi wifi connected

        if( DeviceData.eCtrlStateDevice == E_DEVICE_CONTROL_LOCAL)
        {
        	tcp_server_create_task();
        }
        else  if( DeviceData.eCtrlStateDevice == E_DEVICE_CONTROL_ONLINE)
        {
        	vMQTTClient_Start();
        }
        else if( DeviceData.eCtrlStateDevice  == E_DEVICE_CONTROL_UART)
        {
        	uart_command_start();
        }

        change_next_state(E_DEVICE_NORMAL);
    }
}
/***********************************************************************************************************************
* Function Name: 
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
static bool check_change_state(E_StateDevice state)
{
    if ((DeviceData.eStateDevice == state) && (change_device_state == true))
    {
        change_device_state = false;
        return true;
    }
    else
        return false;
}
/***********************************************************************************************************************
* Function Name: 
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
static void change_next_state(E_StateDevice next_state)
{
    DeviceData.eStateDevice = next_state;
    change_device_state = true;
}
/***********************************************************************************************************************
* End of file
***********************************************************************************************************************/

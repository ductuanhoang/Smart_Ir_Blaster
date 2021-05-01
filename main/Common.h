/*
 * Common.h
 *
 *  Created on: Aug 19, 2020
 *      Author: Yolo
 */

#ifndef MAIN_COMMON_H_
#define MAIN_COMMON_H_

#ifdef __cplusplus
extern "C"
{
#endif



#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "Ir_driver/ir_protocol_cpp/ir_remote_define.h"
#include "main.h"
enum
{
	E_LOG_LVL_NONE,
	E_LOG_LVL_INFO,
	E_LOG_LVL_ERROR,
	E_LOG_LVL_WARNING,
	E_LOG_LVL_DEBUG,
	E_LOG_LVL_NEVER
};

/* Console color */
#define RESET "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"
/*Background colors*/
#define BG_KOLORS_BLK "\x1b[40m" //Black
#define BG_KOLORS_RED "\x1b[41m" //Red
#define BG_KOLORS_GRN "\x1b[42m" //Green
#define BG_KOLORS_YEL "\x1b[43m" //Yellow
#define BG_KOLORS_BLU "\x1b[44m" //Blue

#define LOG_SHOULD_I(level) (level <= LOG_BUILD_LEVEL && level <= E_LOG_LVL_DEBUG)
#define LOG(level, tag, ...)                                       \
	do                                                             \
	{                                                              \
		if (LOG_SHOULD_I(level))                                   \
		{                                                          \
			printf("[%s] %s:%d: " RESET, tag, __func__, __LINE__); \
			printf(__VA_ARGS__);                                   \
			printf("\r\n");                                        \
		}                                                          \
	} while (0)

#define APP_LOGE(...) LOG(E_LOG_LVL_ERROR, KRED "ERROR" RESET, __VA_ARGS__)
#define APP_LOGI(...) LOG(E_LOG_LVL_INFO, KGRN "INFOR" RESET, __VA_ARGS__)
#define APP_LOGD(...) LOG(E_LOG_LVL_DEBUG, KYEL "DEBUG" RESET, __VA_ARGS__)
#define APP_LOGW(...) LOG(E_LOG_LVL_WARNING, BG_KOLORS_YEL "ALARM" RESET, __VA_ARGS__)

#define LOG_BUILD_LEVEL E_LOG_LVL_DEBUG

#define GPIO_OUTPUT_LED_RED 	19
#define GPIO_OUTPUT_LED_GREEN 	18
#define GPIO_OUTPUT_LED_BLUE 	5
#define GPIO_OUTPUT_NANO_TIMER	23


#define GPIO_OUTPUT_PIN_SEL (1ULL << GPIO_OUTPUT_LED_RED) | (1ULL << GPIO_OUTPUT_LED_GREEN) | (1ULL << GPIO_OUTPUT_LED_BLUE) | (1ULL << GPIO_OUTPUT_NANO_TIMER)

#define GPIO_USER_BUTTON 0
#define GPIO_INPUT_PIN_SEL (1ULL << GPIO_USER_BUTTON)

#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))


typedef enum
{
	E_BLUETOOTH_DISCONNECT,
	E_BLUETOOTH_CONNECT
} bluetooth_status_t;

extern bluetooth_status_t bluetooth_status;

#define BOS_FREQ_MAX_SETUP 500
#define BOS_FREQ_MIN_SETUP 50
#define BOS_FREQ_DEFAULT_SETUP 300

typedef enum
{
	E_DEVICE_CONTROL_LOCAL,
	E_DEVICE_CONTROL_ONLINE,
	E_DEVICE_CONTROL_UART,
} E_StateCtrlDevice;

typedef enum
{
	E_DEVICE_NORMAL,
	E_DEVICE_SMART_CONFIG,
	E_DEVICE_SMART_CONFIG_DOING,
	E_DEVICE_WIFI_AP_DISCONNECT,
	E_DEVICE_WIFI_CONNECTED,
	E_DEVICE_WIFI_RECONNECT,

} E_StateDevice;

typedef enum
{
	E_DEVICE_IR_NORMAL,
	E_DEVICE_IR_LEARN_CMD,
	E_DEVICE_IR_END_LEARN_CMD
} E_IrState;

typedef enum
{
	E_SOURCE_FROM_BLE = 0,
	E_SOURCE_FROM_TCP = 1,
	E_SOURCE_FROM_UART = 1,
} E_Source;

typedef enum
{
	E_CMD_NONE,
	E_CMD_CTRL,
	E_CMD_LEARN,
	E_CMD_LEARN_NEW_RAW_IR,
	E_CMD_LEARN_WATTING,
	E_CMD_LEARN_ACCEPT,
	E_CMD_END_LEARN
} E_TypeNew_CMD;

typedef struct
{
	char UserName[32];
	char Password[32];
	char MAC_ADD[18];
} WifiStation_Info_t;


typedef struct
{
	uint8_t Power;
	uint8_t Mode;
	uint8_t FanSpeed;
	uint8_t Temp;
} irAcValue_t;

typedef struct
{
	uint16_t type_protocol;
	uint16_t frequency;
	uint16_t number_bits;
}ir_base_send_t;


typedef struct
{
	char file_name[20]; // name of remote
//	eProtocol_type remote_type; // type of remote NEC/SONY ...
	char function_name[10];
	char type_cmd[10];
	char value_cmd[1024];
	int ir_raw_data[512];
	int ir_raw_length;
	E_TypeNew_CMD new_ctrl_cmd;
	bool action2tcp;
	irAcValue_t irAcValue;
	ir_base_send_t ir_base_send;
} IrMessageControl_t;

/*
 * struct device data
 */
typedef struct
{
	E_StateCtrlDevice eCtrlStateDevice; // ctrl local, ctrl online
	E_StateDevice eStateDevice;			//	E_DEVICE_NORMAL, E_DEVICE_SMART_CONFIG, E_DEVICE_WIFI_AP_DISCONNECT,
	E_IrState eIrState;					//E_DEVICE_IR_NORMAL, E_DEVICE_IR_LEARN_CMD
	WifiStation_Info_t WifiStation_Info;
//	MQTTClient_Info  MqttConfig_Info;
	IrMessageControl_t IrMessageCtr_Info;
	bool SmartConfig_State;
} Device_Data_t;

extern Device_Data_t DeviceData;

extern TaskHandle_t tcp_handle_task;
extern TaskHandle_t mqtt_handle_task;
extern TaskHandle_t ir_tx_handle_task;
extern TaskHandle_t ir_rx_handle_task;
extern TaskHandle_t bluetooth_handle_task;



#ifdef __cplusplus
}
#endif

#endif /* MAIN_COMMON_H_ */

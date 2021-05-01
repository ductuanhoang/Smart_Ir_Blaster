/*
 * json_parser.c
 *
 *  Created on: Nov 16, 2020
 *      Author: Yolo
 */

/***********************************************************************************************************************
* Pragma directive
***********************************************************************************************************************/

/***********************************************************************************************************************
* Includes <System Includes>
***********************************************************************************************************************/
#include "json_parser.h"
#include "cJson_lib/cJSON.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "../data_base/data_base.h"
#include "../Interface/Logger_File/logger_file.h"
/***********************************************************************************************************************
* Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
* Typedef definitions
***********************************************************************************************************************/

#define TYPE_COMMAND_SMART_CONFIG "smart_config"
#define TYPE_COMMAND_CONTROL "control"
#define TYPE_COMMAND_LEARN_CMD "learn_cmd"
#define TYPE_COMMAND_CANCEL_CMD "cancel_cmd"
#define TYPE_COMMAND_END_LEARN "end_learn"
#define TYPE_COMMAND_TEST_DEVICE "check_device"
#define TYPE_COMMAND_READ_FILE "read_file"
#define TYPE_COMMAMD_ADD_CMD "new_cmd"
#define TYPE_COMMAND_SAVE_FILE  "save_file" // truyen cmd theo dang json qua uart va tcp
/***********************************************************************************************************************
* Private global variables and functions
***********************************************************************************************************************/
static bool json_parser_smartconfig(const char *message);
static bool json_parser_command_contrl(const char *message);
static void json_parser_command_learn_cmd(const char *message);
static void json_parser_command_test_device(const char *message);
static void json_parser_command_add_cmd(const char *message);
static bool json_parser_command_save_file( const char *message);
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
bool json_parser_message(const char *message, uint8_t source)
{
	APP_LOGD("Serialize.....");
	APP_LOGI("message = %s", message);
	bool status = false;
	//	const char * message = "{\"type\": \"telemetry\",\"thing_token\" : \"{device id}\",\"entity_type\": \"smart_ir_<version>\", \"command1\" : \"smart_config\",\"wifi_ssid\" : \"my_wifi\",\"wifi_password\":\"12345@123\",\"address\" : \"https:my_address:1080\"}";
	cJSON *root2 = cJSON_Parse(message);
	cJSON *command = cJSON_GetObjectItem(root2, "command");
	cJSON *thing_token = cJSON_GetObjectItem(root2, "thing_token");

	if (thing_token)
	{
		char *value_thing_token = cJSON_GetObjectItem(root2, "thing_token")->valuestring;
		if (strcmp(value_thing_token, "bluetooth") == 0)
		{
			APP_LOGI("thing token smart config");
		}
		else if (strcmp(value_thing_token, DeviceData.WifiStation_Info.MAC_ADD) != 0)
		{
			APP_LOGE("Err thing token go to exit message");
			return status;
		}
	}
	if (command)
	{
		char *value_type_cmd = cJSON_GetObjectItem(root2, "command")->valuestring;
		sprintf(DeviceData.IrMessageCtr_Info.type_cmd, "%s", value_type_cmd);
		if ((strcmp(value_type_cmd, TYPE_COMMAND_SMART_CONFIG) == 0) && (source == E_SOURCE_FROM_BLE))
		{
			status = json_parser_smartconfig(message);
		}
		else if ((strcmp(value_type_cmd, TYPE_COMMAND_CONTROL) == 0) && (source == E_SOURCE_FROM_TCP))
		{
			status = json_parser_command_contrl(message);
			DeviceData.IrMessageCtr_Info.new_ctrl_cmd = E_CMD_CTRL;
		}
		else if ((strcmp(value_type_cmd, TYPE_COMMAND_LEARN_CMD) == 0) && (source == E_SOURCE_FROM_TCP)) // learn message ir start
		{
			json_parser_command_learn_cmd(message);
			DataBase_SetIrState(E_DEVICE_IR_LEARN_CMD);
			DeviceData.IrMessageCtr_Info.new_ctrl_cmd = E_CMD_LEARN;
			status = true;
		}
		else if ((strcmp(value_type_cmd, TYPE_COMMAND_TEST_DEVICE) == 0) && (source == E_SOURCE_FROM_TCP))
		{
			json_parser_command_test_device(message);
			status = true;
		}
		else if ((strcmp(value_type_cmd, TYPE_COMMAND_END_LEARN) == 0) && (source == E_SOURCE_FROM_TCP))
		{
			DeviceData.IrMessageCtr_Info.new_ctrl_cmd = E_CMD_NONE;
			DataBase_SetIrState(E_DEVICE_IR_END_LEARN_CMD);
			status = true;
		}
		else if ((strcmp(value_type_cmd, TYPE_COMMAND_CANCEL_CMD) == 0) && (source == E_SOURCE_FROM_TCP))
		{
			status = true;
		}
		else if ((strcmp(value_type_cmd, TYPE_COMMAND_READ_FILE) == 0) && (source == E_SOURCE_FROM_TCP))
		{
			status = true;
		}
		else if ((strcmp(value_type_cmd, TYPE_COMMAMD_ADD_CMD) == 0) && (source == E_SOURCE_FROM_TCP))
		{
			json_parser_command_add_cmd(message);
			status = true;
		}
		else if ((strcmp(value_type_cmd, TYPE_COMMAND_SAVE_FILE) == 0) && (source == E_SOURCE_FROM_TCP))
        {
			status = json_parser_command_save_file(message);
			DeviceData.IrMessageCtr_Info.new_ctrl_cmd = E_CMD_CTRL;
        }
	}
	else
	{
		APP_LOGD("thing_token have'nt");
		status = false;
	}

	cJSON_Delete(root2);

	return status;
}
/***********************************************************************************************************************
* Function Name:
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
char *json_packet_message(char *cmd, bool status_response)
{
	cJSON *root = NULL;
	root = cJSON_CreateObject();
	cJSON_AddItemToObject(root, "type", cJSON_CreateString("device_response"));
	cJSON_AddStringToObject(root, "thing_token", DeviceData.WifiStation_Info.MAC_ADD);
	cJSON_AddNumberToObject(root, "code", status_response);
	cJSON_AddStringToObject(root, "trans_code", cmd);
	//	return out = cJSON_Print(root);
	APP_LOGD("message = %s", cJSON_PrintUnformatted(root));
	return cJSON_PrintUnformatted(root);
}

/***********************************************************************************************************************
* static functions
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name:
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
static bool json_parser_smartconfig(const char *message)
{
	bool status = false;
	cJSON *root2 = cJSON_Parse(message);
	cJSON *wifi_name = cJSON_GetObjectItem(root2, "wifi_ssid");
	cJSON *wifi_password = cJSON_GetObjectItem(root2, "wifi_password");
	cJSON *direction_ctrl = cJSON_GetObjectItem(root2, "direction_ctrl");

	if (wifi_name && wifi_password)
	{
		char *type_wifi_name = cJSON_GetObjectItem(root2, "wifi_ssid")->valuestring;
		char *type_wifi_password = cJSON_GetObjectItem(root2, "wifi_password")->valuestring;
		int type_direction_ctrl = cJSON_GetObjectItem(root2, "direction_ctrl")->valueint;
		APP_LOGI("type_wifi_name = %s", type_wifi_name);
		APP_LOGI("type_wifi_password = %s", type_wifi_password);
		APP_LOGI("type_direction_ctrl = %d", type_direction_ctrl);

		sprintf(DeviceData.WifiStation_Info.UserName, "%s", type_wifi_name);
		sprintf(DeviceData.WifiStation_Info.Password, "%s", type_wifi_password);
		DeviceData.eCtrlStateDevice = type_direction_ctrl;
		logger_LoadInfo_WifiStation(&DeviceData.WifiStation_Info, true);
		status = true;
	}
	else
	{
		APP_LOGE("parser smart config err");
	}

	cJSON_Delete(root2);
	return status;
}
/***********************************************************************************************************************
* Function Name:
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
static bool json_parser_command_save_file( const char *message)
{
    bool status = false;
    cJSON *root2 = cJSON_Parse(message);
    cJSON *remote_name = cJSON_GetObjectItem(root2, "remote");
    cJSON *trans_code = cJSON_GetObjectItem(root2, "trans_code");
    if (remote_name && trans_code)
    {
		status = true;
		char *type_remote_name = cJSON_GetObjectItem(root2, "remote")->valuestring;
		char *type_trans_code = cJSON_GetObjectItem(root2, "trans_code")->valuestring;

		if(strcmp( type_trans_code, "LEARN") == 0)
		{
			cJSON *encode = cJSON_GetObjectItem(root2, "encode");
	        char *type_command_value = cJSON_GetObjectItem(encode, "Command")->valuestring;
	        APP_LOGI("type_remote_name = %s", type_remote_name);
	        APP_LOGI("type_command_value = %s", type_command_value);

	        sprintf(DeviceData.IrMessageCtr_Info.function_name, "%s", type_trans_code);
	        sprintf(DeviceData.IrMessageCtr_Info.file_name, "%s", type_remote_name);
	        sprintf(DeviceData.IrMessageCtr_Info.value_cmd, "%s", type_command_value);
		}
        // save to file
    }
    return status;
}
/***********************************************************************************************************************
* Function Name:
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
static bool json_parser_command_contrl(const char *message)
{
	bool status = false;
	cJSON *root2 = cJSON_Parse(message);
	cJSON *remote_name = cJSON_GetObjectItem(root2, "remote");
	cJSON *trans_code = cJSON_GetObjectItem(root2, "trans_code");

	if (remote_name && trans_code)
	{
		status = true;
		char *type_remote_name = cJSON_GetObjectItem(root2, "remote")->valuestring;
		char *type_trans_code = cJSON_GetObjectItem(root2, "trans_code")->valuestring;
		APP_LOGI("type_remote_name = %s", type_remote_name);
		if(strcmp( type_trans_code, "HAVC") == 0)
		{
			cJSON *encode = cJSON_GetObjectItem(root2, "encode");
			sprintf(DeviceData.IrMessageCtr_Info.function_name, "%s", type_trans_code);
			sprintf(DeviceData.IrMessageCtr_Info.file_name, "%s", type_remote_name);
			DeviceData.IrMessageCtr_Info.irAcValue.Power = cJSON_GetObjectItem(encode, "Power")->valueint;
			DeviceData.IrMessageCtr_Info.irAcValue.Mode = cJSON_GetObjectItem(encode, "Mode")->valueint;
			DeviceData.IrMessageCtr_Info.irAcValue.FanSpeed = cJSON_GetObjectItem(encode, "FanSpeed")->valueint;
			DeviceData.IrMessageCtr_Info.irAcValue.Temp = cJSON_GetObjectItem(encode, "Temp")->valueint;

			APP_LOGI("Power = %d", DeviceData.IrMessageCtr_Info.irAcValue.Power);
			APP_LOGI("Mode = %d", DeviceData.IrMessageCtr_Info.irAcValue.Mode);
			APP_LOGI("FanSpeed = %d", DeviceData.IrMessageCtr_Info.irAcValue.FanSpeed);
			APP_LOGI("Temp = %d", DeviceData.IrMessageCtr_Info.irAcValue.Temp);
		}
		else if(strcmp( type_trans_code, "NORMAL") == 0)
		{
			cJSON *encode = cJSON_GetObjectItem(root2, "encode");
			APP_LOGI("type_trans_code = %s", type_trans_code);
			sprintf(DeviceData.IrMessageCtr_Info.function_name, "%s", type_trans_code);
			sprintf(DeviceData.IrMessageCtr_Info.file_name, "%s", type_remote_name);
			sprintf(DeviceData.IrMessageCtr_Info.type_cmd, "%s", cJSON_GetObjectItem(encode, "Command")->valuestring);

		}
		else if(strcmp( type_trans_code, "RAW") == 0)
		{
			cJSON *encode = cJSON_GetObjectItem(root2, "encode");
			APP_LOGI("type_trans_code = %s", type_trans_code);
			sprintf(DeviceData.IrMessageCtr_Info.file_name, "%s", type_remote_name);
			sprintf(DeviceData.IrMessageCtr_Info.function_name, "%s", type_trans_code);
			// cJSON_GetObjectItem(encode, "Command")->valuestring
			sprintf(DeviceData.IrMessageCtr_Info.value_cmd, "%s", cJSON_GetObjectItem(encode, "Command")->valuestring);

		}

	}
	else
	{
		APP_LOGE("parser command control err");
	}

	cJSON_Delete(root2);
	return status;
}
/***********************************************************************************************************************
* Function Name:
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
static void json_parser_command_learn_cmd(const char *message)
{
	cJSON *root2 = cJSON_Parse(message);
	cJSON *remote = cJSON_GetObjectItem(root2, "remote");
	cJSON *trans_code = cJSON_GetObjectItem(root2, "trans_code");

	if (remote && trans_code)
	{
		char *type_remote = cJSON_GetObjectItem(root2, "remote")->valuestring;
		char *type_trans_code = cJSON_GetObjectItem(root2, "trans_code")->valuestring;

		// DeviceData.IrMessageCtr_Info.new_ctrl_cmd = E_CMD_LEARN_ACCEPT;
		APP_LOGD("type_remote = %s", type_remote);
		APP_LOGD("type_trans_code = %s", type_trans_code);
	}

	cJSON_Delete(root2);
}
/***********************************************************************************************************************
* Function Name:
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
static void json_parser_command_test_device(const char *message)
{
	// cJSON *root2 = cJSON_Parse(message);
	// char *type_wifi_name = cJSON_GetObjectItem(root2, "wifi_ssid")->valuestring;
	// char *type_wifi_password = cJSON_GetObjectItem(root2, "wifi_password")->valuestring;

	// APP_LOGI("type_wifi_name = %s", type_wifi_name);
	// APP_LOGI("type_wifi_password = %s", type_wifi_password);
	// cJSON_Delete(root2);
}
/***********************************************************************************************************************
* Function Name:
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
static void json_parser_command_add_cmd(const char *message)
{
	cJSON *root2 = cJSON_Parse(message);

	cJSON *remote_name = cJSON_GetObjectItem(root2, "remote");
	cJSON *trans_code = cJSON_GetObjectItem(root2, "trans_code");

	if (remote_name && trans_code)
	{
		char *type_remote_name = cJSON_GetObjectItem(root2, "remote")->valuestring;
		char *type_trans_code = cJSON_GetObjectItem(root2, "trans_code")->valuestring;
		APP_LOGI("type_remote_name = %s", type_remote_name);
		APP_LOGI("type_trans_code = %s", type_trans_code);

		sprintf(DeviceData.IrMessageCtr_Info.file_name, "%s", type_remote_name);
		sprintf(DeviceData.IrMessageCtr_Info.function_name, "%s", type_trans_code);
		
		if (DeviceData.IrMessageCtr_Info.new_ctrl_cmd == E_CMD_LEARN_WATTING)
			DeviceData.IrMessageCtr_Info.new_ctrl_cmd = E_CMD_LEARN_ACCEPT;
	}

	cJSON_Delete(root2);
}

/***********************************************************************************************************************
* Function Name:
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
static void json_parser_delete_command(char *file_name, char *cmd)
{
	// neu command da ton tai trong file thi xoa
}
/***********************************************************************************************************************
* Function Name:
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
static bool json_parser_check_id(const char *message, char *mac_id)
{
	bool status = false;
	cJSON *root2 = cJSON_Parse(message);

	return status;
}

/***********************************************************************************************************************
* End of file
***********************************************************************************************************************/

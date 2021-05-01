/*
 * app_test.c
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
#include "app_test.h"

#include "../Interface/Logger_File/logger_file.h"
#include "../json_parser/json_parser.h"
#include "../Task/bluetooth.h"
#include "../Ir_driver/ir_driver.h"
#include "../Task/wifi_task.h"


#include "../Ir_driver/ir_protocol_cpp/IrBase.h"
/***********************************************************************************************************************
* Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
* Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
* Private global variables and functions
***********************************************************************************************************************/

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
int32_t list_all_file(p_shell_context_t context, int32_t argc, char **argv)
{
	logger_list_file(NULL, "/spiffs/");
    return 0;
}

int32_t get_mac(p_shell_context_t context, int32_t argc, char **argv)
{
//	APP_LOGI("MAC = %s", wifi_get_mac());
	return 0;
}

int32_t file_test(p_shell_context_t context, int32_t argc, char **argv)
{
	logger_file_test();
	return 0;
}
/***********************************************************************************************************************
* Function Name:
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/

int32_t read_file(p_shell_context_t context, int32_t argc, char **argv)
{
//	logger_file_save("/spiffs/");
	char buffer[20];
	sprintf(buffer, "/spiffs/%s", argv[1]);

	logger_read_number_line(buffer);
	return 0;
}


int32_t read_cmd(p_shell_context_t context, int32_t argc, char **argv)
{
	if (argc < 1)
    {
        printf("Missing parameter\r\n");
        return -1;
    }
	APP_LOGD("cmd = %s", argv[1]);
//	logger_file_read_cmd("hello2.txt", argv[1]);
	return 0;
}

int32_t json_test(p_shell_context_t context, int32_t argc, char **argv)
{
	json_packet_message("aaaa", 1);
	return 0;
}

int32_t go_smart_config(p_shell_context_t context, int32_t argc, char **argv)
{
	bluetooth_init();
	return 0;
}

int32_t ir_test(p_shell_context_t context, int32_t argc, char **argv)
{
	return 0;
}
int32_t ir_save(p_shell_context_t context, int32_t argc, char **argv)
{
//	int data[] = {4406,4319,568,1598,567,546,547,1597,569,1597,567,546,547,541,542,1623,542,547,546,540,543,1622,543,545,548,539,543,1623,542,1597,568,546,547,1625,551,1587,567,547,546,1597,568,1598,578,1589,576,1590,575,1591,574,1593,572,540,542,1628,548,539,543,545,569,518,575,513,570,517,576,513,569,517,576,1593,572,516,577,510,573,515,578,509,574,513,569,521,572,1591,574,514,569,1601,574,1565,600,1566,599,1567,598,1568,597,1569,596,5145,4440,4291,596,1570,595,518,575,1595,570,1570,595,518,575,513,570,1595,570,520,573,512,570,1596,569,518,575,513,570,1595,570,1570,605,509,574,1597,578,1560,595,519,574,1595,570,1570,605,1561,604,1562,603,1563,602,1566,599,512,570,1600,576,511,571,517,576,511,572,516,577,510,572,517,576,510,573,1596,569,519,574,513,570,518,575,512,570,517,576,514,569,1594,571,517,576,1593,572,1568,597,1569,596,1570,626,1540,625,1543,570,0};
//	char data_hex[20] = "0x01FE40BF"; //
//	int length = sizeof(data) / sizeof(int);
//	if(atoi(argv[3]) == 0)
//	{
//		APP_LOGD("go save raw");
//		logger_file_save_cmd(argv[1], argv[2], 0,data, length);
//	}
//	else
//	{
//		APP_LOGD("go save hex");
//		logger_file_save_cmd(argv[1], argv[2], 2, data_hex, 4);
//	}


	return 0;
}

int32_t ir_send(p_shell_context_t context, int32_t argc, char **argv)
{
//	int* data;
//	int length;
////	data = logger_file_read_cmd(argv[1], argv[2], &length);
//	APP_LOGD("length = %d", length);
//	for (int i = 0; i < length; i++)
//	{
//		APP_LOGD("%d", data[i]);
//	}
//	sendIR(data, length);
	// set flag control ir
//	DeviceData.IrMessageCtr_Info.new_ctrl_cmd = E_CMD_CTRL;
	return 0;
}
int32_t logger_list_cmd(p_shell_context_t context, int32_t argc, char **argv)
{

	bool status = logger_file_check_cmd(argv[1], NULL);
	return 0;
}
int32_t logger_delete_file(p_shell_context_t context, int32_t argc, char **argv)
{
	logger_file_delete(argv[1]);
	return 0;
}

int32_t ir_new_cmd(p_shell_context_t context, int32_t argc, char **argv)
{
	ir_simulation_new_ir();
	return 0;
}

int32_t ir_NEC_test(p_shell_context_t context, int32_t argc, char **argv)
{
	uint8_t type_remote = atoi(argv[1]);
	bool test_normal = atoi(argv[2]);
	IrBase_Send(type_remote, test_normal, 0x00, 0, 0); //test normal
//	IrBase_Send(type_remote, eCOMMAND_NONE, 0x00); //test AC

//	switch (type_remote)
//	{
//	case ePROTOCOL_NEC_01:
//	{
////		ir_NEC_send_test(0x1FE807F, 32, 2); // OK
//		break;
//	}
//	case ePROTOCOL_RC5:
//	{
//		ir_RC5_send(0x175, kRC5Bits); //OK
//		break;
//	}
//	case ePROTOCOL_RC6:
//	{
////		ir_RC6_send(data, length);
//		break;
//	}
//
//	case ePROTOCOL_SONY:
//	{
////		ir_Sony_Send(0xA90, 12);
//		sendSony38(0xA90, 12, 2);
//		break;
//	}
//	case ePROTOCOL_PANASONIC:
//		ir_PanasonicSendAC_1(0);
//		break;
//	case ePROTOCOL_JVC:
//	{
//		ir_JVC_send(0xC2B8, 16); //OK
//		break;
//	}
//	case ePROTOCOL_SAMSUNG:
//	{
////		ir_SamSung_Send(0xE0E09966, 32); //OK
//		sendSamsung36(0x400E00FF, kSamsung36Bits, 1);
//		break;
//	}
//	case ePROTOCOL_RCMM:
//	{
//		ir_RCMM_send(0xe0a600, 24);
//		break;
//	}
//	case ePROTOCOL_LUTRON:
//		break;
//	case ePROTOCOL_DAIKIN152x:
//		ir_Daikin152_Send(0);
//		break;
//	case ePROTOCOL_DAIKINESP:
//		ir_DaikinESPSend(0);
//		break;
//	default:
//		break;
//	}
	return 0;
}


/***********************************************************************************************************************
* static functions
***********************************************************************************************************************/

/***********************************************************************************************************************
* End of file
***********************************************************************************************************************/

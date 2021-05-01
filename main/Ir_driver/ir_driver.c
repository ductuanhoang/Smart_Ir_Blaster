/*
 * ir_driver.c
 *
 *  Created on: Nov 15, 2020
 *      Author: Yolo
 */
/***********************************************************************************************************************
* Pragma directive
***********************************************************************************************************************/

/***********************************************************************************************************************
* Includes <System Includes>
***********************************************************************************************************************/
#include "ir_driver.h"
#include "driver/rmt.h"
#include "../Task/tcp_server.h"
#include "../json_parser/json_parser.h"
#include "../Interface/Logger_File/logger_file.h"

#include "ir_protocol_cpp/IrBase.h"
#include "../Interface/UserUart.h"
/***********************************************************************************************************************
* Macro definitions
***********************************************************************************************************************/
#define RMT_DEFAULT_CONFIG_RX(gpio, channel_id) \
    {                                           \
        .rmt_mode = RMT_MODE_RX,                \
        .channel = channel_id,                  \
        .gpio_num = gpio,                       \
        .clk_div = 80,                          \
        .mem_block_num = 6,                     \
        .rx_config = {                          \
            .idle_threshold = 30000,            \
            .filter_ticks_thresh = 100,         \
            .filter_en = true,                  \
        }                                       \
    }
/***********************************************************************************************************************
* Typedef definitions
***********************************************************************************************************************/
#define IR_GPIO_RX_1 GPIO_NUM_25
#define IR_GPIO_RX_2 GPIO_NUM_25

/***********************************************************************************************************************
* Private global variables and functions
***********************************************************************************************************************/
static void ir_driver_rx_task(void *pvParameters);
static void ir_driver_tx_task(void *pvParameters);
static int duration_raw(uint32_t level, uint32_t duration);
static void buildItem(rmt_item32_t *item, unsigned int high_us, int low_us);
static uint16_t *parser_message(char* data, uint16_t *frequency, uint16_t *length);
bool ir_driver_rx_task_enable = false;
bool ir_driver_rx_enable = false;
bool new_cmd = false;
/***********************************************************************************************************************
* Exported global variables and functions (to be accessed by other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
* Imported global variables and functions (from other files)
***********************************************************************************************************************/
void ir_driver_create_rx_task(void)
{
	if( ir_driver_rx_task_enable == false)
		xTaskCreate(ir_driver_rx_task, "ir_driver_rx_task", 4 * 2024, NULL, 8 | portPRIVILEGE_BIT, ir_rx_handle_task);
}

void ir_driver_delete_rx_task(void)
{
	if( ir_driver_rx_task_enable == true)
	{
		APP_LOGD("end ir_recive");
		ir_driver_rx_task_enable = false;
		new_cmd = false;
		DeviceData.IrMessageCtr_Info.new_ctrl_cmd = E_CMD_NONE;
		ir_driver_uninstall(CHANNEL_IR_RMT_RX);
	    if (ir_rx_handle_task != NULL)
	        vTaskDelete(ir_rx_handle_task);
	}
}

void ir_driver_start(void)
{
	if( ir_driver_rx_enable == false)
	{
		APP_LOGD("start ir_recive");
//		new_cmd = false;
//		DeviceData.IrMessageCtr_Info.new_ctrl_cmd = E_CMD_NONE;
		ir_driver_rx_enable = true;
		rmt_rx_start(CHANNEL_IR_RMT_RX, true);

	}
}

void ir_driver_stop(void)
{
	if( ir_driver_rx_enable == true)
	{
		ir_driver_rx_enable = false;
		rmt_rx_stop(CHANNEL_IR_RMT_RX);
	}
}

void ir_driver_create_tx_task(void)
{
//    xTaskCreate(ir_driver_tx_task, "ir_driver_tx_task", 10 * 2024, NULL, 2 | portPRIVILEGE_BIT, ir_tx_handle_task);
	xTaskCreatePinnedToCore(ir_driver_tx_task, "ir_driver_tx_task", 10 * 1024, NULL, 2 | portPRIVILEGE_BIT, NULL, 1);
}

void ir_drvier_delete_tx_task(void)
{
    if (ir_tx_handle_task != NULL)
        vTaskDelete(ir_tx_handle_task);
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
void ir_driver_rx_init(gpio_num_t gpio, rmt_channel_t channel)
{
    rmt_config_t rmt_rx_config = RMT_DEFAULT_CONFIG_RX(gpio, channel); //GPIO_NUM_21
    rmt_config(&rmt_rx_config);
}
/***********************************************************************************************************************
* Function Name:
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
bool ir_driver_tx_init(rmt_channel_t channel, uint32_t freq, uint32_t duty_percent)
{
	APP_LOGD("ir_driver_tx_init fre = %d--- duty = %d", freq, duty_percent);
    bool status = false;
    rmt_config_t config;
    config.channel = (rmt_channel_t)CHANNEL_IR_RMT_TX;
    config.gpio_num = (gpio_num_t)GPIO_NUM_21; //GPIO_NUM_19;
    config.mem_block_num = 6;                  //how many memory blocks 64 x N (0-7)
    config.clk_div = 80;
    config.tx_config.loop_en = false;
    config.tx_config.carrier_duty_percent = duty_percent;
    config.tx_config.carrier_freq_hz = freq;
    config.tx_config.carrier_level = (rmt_carrier_level_t)1;
    config.tx_config.carrier_en = 1;
    config.tx_config.idle_level = (rmt_idle_level_t)0;
    config.tx_config.idle_output_en = true;
    config.rmt_mode = (rmt_mode_t)0; //RMT_MODE_TX;
    rmt_config(&config);
    if (rmt_driver_install(config.channel, 0, 0) == ESP_OK) //19     /*!< RMT interrupt number, select from soc.h */
        status = true;
    else
        status = false;

    return status;
}
/***********************************************************************************************************************
* Function Name:
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void ir_driver_tx_task(void *pvParameters)
{
    while (1)
    {
        if (DeviceData.IrMessageCtr_Info.new_ctrl_cmd == E_CMD_CTRL)
        {

            if (strcmp(DeviceData.IrMessageCtr_Info.function_name, "HAVC") == 0)
            {
            	APP_LOGI("new contrl HAVC call");
            	// AC control type_remote: name remote, type_command: eCOMMAND_IR_NORMAL
            	IrBase_Send(atoi(DeviceData.IrMessageCtr_Info.file_name), eCOMMAND_HAVC, 0, 0, 0);
            }
            else if (strcmp(DeviceData.IrMessageCtr_Info.function_name, "NORMAL") == 0)
            {
            	APP_LOGI("new contrl NORMAL call");
            	uint64_t data = 0;
            	uint16_t protocol_type = 0; // none use
            	uint16_t protocol_freq = 0;// none use
            	uint16_t protocol_number_bits = 0;// none use
            	uint16_t length = 0;

				uint16_t *hex_data = 0;
				// read data from flash
				hex_data = logger_file_read_cmd(DeviceData.IrMessageCtr_Info.file_name,
												DeviceData.IrMessageCtr_Info.type_cmd, &protocol_type, &protocol_freq, &protocol_number_bits, &length);

				APP_LOGD("protocol_type = %d", protocol_type);
				APP_LOGD("length = %d", length);
				APP_LOGD("protocol_freq = %d", protocol_freq);

				if ( protocol_type == eCOMMAND_IR_NORMAL)
            	{
					if( length != 0)
					{
						for (int i = 0; i < length - 1; i++) // ghep data sang hex64
						{
							data |= hex_data[i] << (8 * (length - 1 - 1 - i));
						}
						APP_LOGI("Send remote = %d -- data = %lld", atoi(DeviceData.IrMessageCtr_Info.file_name), data);

						IrBase_Send(atoi(DeviceData.IrMessageCtr_Info.file_name), eCOMMAND_IR_NORMAL, data, 0, 0);
					}
					else APP_LOGE("No command");
            	}
            	else if ( protocol_type == eCOMMAND_IR_RAW)
            	{
                	if( length != 0)
                	{
                		IrBase_SendRaw(hex_data, length, protocol_freq, 50, 0);
                	}
                	else APP_LOGE("No command");
            	}

            }
            else if (strcmp(DeviceData.IrMessageCtr_Info.function_name, "RAW") == 0)
            {
				uint16_t protocol_freq = 0;// none use
				uint16_t length = 0;
//				uint16_t *data_raw = 0;
				// phan tach chuoi string
				uint16_t *data_raw = parser_message(DeviceData.IrMessageCtr_Info.value_cmd
							, &protocol_freq, &length);
				APP_LOGD("protocol_freq = %d", protocol_freq);
				APP_LOGD("length = %d", length);
				for( int i = 0; i < length; i++)
				{
					printf("data_hex[%d]=%d", i, data_raw[i]);
				}
//				int raw[] = {9092,4412,678,456,652,508,652,482,646,460,678,482,626,506,626,508,628,1612,630,1612,652,1590,628,1614,654,1588,626,1616,630,1612,652,1590,628,506,626,1614,626,510,652,482,626,508,626,508,626,508,626,506,626,506,652,480,654,1588,628,1612,654,1590,626,1612,656,1586,628,1612,630,1614,628};

				IrBase_SendRaw(data_raw, length, protocol_freq, 50, 0);


            	memset(DeviceData.IrMessageCtr_Info.function_name, 0x00, sizeof(DeviceData.IrMessageCtr_Info.function_name));
            	memset(DeviceData.IrMessageCtr_Info.file_name, 0x00, sizeof(DeviceData.IrMessageCtr_Info.file_name));
            	memset(DeviceData.IrMessageCtr_Info.value_cmd, 0x00, sizeof(DeviceData.IrMessageCtr_Info.value_cmd));
            	memset(DeviceData.IrMessageCtr_Info.type_cmd, 0x00, sizeof(DeviceData.IrMessageCtr_Info.type_cmd));
            }
            else if (strcmp(DeviceData.IrMessageCtr_Info.function_name, "LEARN") == 0)
            {
            	bool status = false;
            	status = logger_file_save_string_cmd(DeviceData.IrMessageCtr_Info.file_name,
    					DeviceData.IrMessageCtr_Info.value_cmd);
            	if( status == true) APP_LOGD("save new cmd ok");

            	memset(DeviceData.IrMessageCtr_Info.function_name, 0x00, sizeof(DeviceData.IrMessageCtr_Info.function_name));
            	memset(DeviceData.IrMessageCtr_Info.file_name, 0x00, sizeof(DeviceData.IrMessageCtr_Info.file_name));
            	memset(DeviceData.IrMessageCtr_Info.value_cmd, 0x00, sizeof(DeviceData.IrMessageCtr_Info.value_cmd));
            	memset(DeviceData.IrMessageCtr_Info.type_cmd, 0x00, sizeof(DeviceData.IrMessageCtr_Info.type_cmd));

            }

            memset(DeviceData.IrMessageCtr_Info.file_name, 0x00, sizeof(DeviceData.IrMessageCtr_Info.file_name));
            memset(DeviceData.IrMessageCtr_Info.function_name, 0x00, sizeof(DeviceData.IrMessageCtr_Info.function_name));

            APP_LOGI("new contrl end");
            DeviceData.IrMessageCtr_Info.new_ctrl_cmd = E_CMD_NONE;
        }

        vTaskDelay(100);
    }
}

/***********************************************************************************************************************
* Function Name:
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/

static void ir_driver_rx_task(void *pvParameters)
{
	ir_driver_rx_task_enable = true;
//    ir_driver_tx_init(CHANNEL_IR_RMT_TX, 38000, 50);
    vTaskDelay(10);
    APP_LOGD("ir_driver_rx_task");
    uint32_t length = 0;
    rmt_item32_t *items = NULL;
    RingbufHandle_t rb = NULL;
    ir_driver_rx_init(IR_GPIO_RX_1,CHANNEL_IR_RMT_RX);

    rmt_driver_install(CHANNEL_IR_RMT_RX, 2000, 0);

    //get RMT RX ringbuffer
    rmt_get_ringbuf_handle(CHANNEL_IR_RMT_RX, &rb);
    // Start receive
//    rmt_rx_start(CHANNEL_IR_RMT_RX, true);

    while (1)
    {
        items = (rmt_item32_t *)xRingbufferReceive(rb, &length, 1000);
        if (items)
        {
        	APP_LOGD("DeviceData.IrMessageCtr_Info.new_ctrl_cmd = %d", DeviceData.IrMessageCtr_Info.new_ctrl_cmd);
            if (DeviceData.IrMessageCtr_Info.new_ctrl_cmd == E_CMD_LEARN)
            {
                new_cmd = true;
                int x = 0, y = 0;
                DeviceData.IrMessageCtr_Info.ir_raw_length = length / 2;
                if (DeviceData.IrMessageCtr_Info.ir_raw_length >= 4)
                {
                    APP_LOGI("DeviceData.IrMessageCtr_Info.ir_raw_length = %d", DeviceData.IrMessageCtr_Info.ir_raw_length);
                    while (x < DeviceData.IrMessageCtr_Info.ir_raw_length)
                    {
                        DeviceData.IrMessageCtr_Info.ir_raw_data[x] = items[y].duration0;
                        DeviceData.IrMessageCtr_Info.ir_raw_data[x + 1] = items[y].duration1;
                        x = x + 2;
                        y = y + 1;
                    }
                }
                else
                {
                    new_cmd = false;
                    DeviceData.IrMessageCtr_Info.ir_raw_length = 0;
                }
            	if( DeviceData.eCtrlStateDevice == E_DEVICE_CONTROL_LOCAL)
            	{
            		tcp_server_transmit(json_packet_message("new_cmd", 1), strlen(json_packet_message("new_cmd", 1)));
            	}
            	else if( DeviceData.eCtrlStateDevice == E_DEVICE_CONTROL_UART)
            	{
            		UserUart_WriteData((unsigned char *)json_packet_message("new_cmd", 1), strlen(json_packet_message("new_cmd", 1)));
            	}

            }

            length /= 4; // one RMT = 4 Bytes
            if (length > 4)
            {
                APP_LOGI("length = %d", length);
                // print the RMT received durations to the monitor
                for (int i = 0; i < length; i++)
                {
                    if (i > 0)
                    {
                        printf(",");
                    }
                    printf("%i", duration_raw(items[i].level0, items[i].duration0));
                    printf(",%i", duration_raw(items[i].level1, items[i].duration1));
                }
                printf("\n");
            }

            //after parsing the data, return spaces to ringbuffer.
            vRingbufferReturnItem(rb, (void *)items);
        }

        switch (DeviceData.IrMessageCtr_Info.new_ctrl_cmd)
        {
        case E_CMD_LEARN:
            if (new_cmd == true)
            {
                new_cmd = false;
                APP_LOGI("E_CMD_LEARN");
                DeviceData.IrMessageCtr_Info.new_ctrl_cmd = E_CMD_LEARN_WATTING;
                // send message to ...
                DeviceData.IrMessageCtr_Info.action2tcp = true;
            }
            else
            {
                memset(DeviceData.IrMessageCtr_Info.ir_raw_data, 0x00, sizeof(DeviceData.IrMessageCtr_Info.ir_raw_data) / sizeof(int));
                DeviceData.IrMessageCtr_Info.ir_raw_length = 0;
            }
            break;
        case E_CMD_LEARN_WATTING: //TODO

            break;
        case E_CMD_LEARN_ACCEPT:
            APP_LOGD(" DeviceData.IrMessageCtr_Info.file_name = %s", DeviceData.IrMessageCtr_Info.file_name);
            APP_LOGD(" DeviceData.IrMessageCtr_Info.function_name = %s", DeviceData.IrMessageCtr_Info.function_name);
            if (DeviceData.IrMessageCtr_Info.file_name == NULL)
            {
                APP_LOGI("ERR E_CMD_LEARN_ACCEPT");
                memset(DeviceData.IrMessageCtr_Info.ir_raw_data, 0x00, sizeof(DeviceData.IrMessageCtr_Info.ir_raw_data) / sizeof(int));
                DeviceData.IrMessageCtr_Info.ir_raw_length = 0;
            }
            else if (DeviceData.IrMessageCtr_Info.function_name == NULL)
            {
                APP_LOGI("ERR E_CMD_LEARN_ACCEPT");
                memset(DeviceData.IrMessageCtr_Info.ir_raw_data, 0x00, sizeof(DeviceData.IrMessageCtr_Info.ir_raw_data) / sizeof(int));
                DeviceData.IrMessageCtr_Info.ir_raw_length = 0;
            }
            else
            {
                APP_LOGI("E_CMD_LEARN_ACCEPT");
//                for( int i=0; i< DeviceData.IrMessageCtr_Info.ir_raw_length; i++)
//                {
//                	printf("raw = %d\r\n", DeviceData.IrMessageCtr_Info.ir_raw_data[i]);
//                }
//                printf("\r\n");

                bool status = logger_file_save_cmd(DeviceData.IrMessageCtr_Info.file_name,
                                     DeviceData.IrMessageCtr_Info.function_name,
                                     ePROTOCOL_RAW,
                                     &DeviceData.IrMessageCtr_Info.ir_raw_data,
                                     DeviceData.IrMessageCtr_Info.ir_raw_length);
                if( status == false)
                {
                	if( DeviceData.eCtrlStateDevice == E_DEVICE_CONTROL_LOCAL)
                	{
                		tcp_server_transmit(json_packet_message("new_cmd", 0), strlen(json_packet_message("new_cmd", 0)));
                	}
                	else if( DeviceData.eCtrlStateDevice == E_DEVICE_CONTROL_UART)
                	{
                		UserUart_WriteData((unsigned char *)json_packet_message("new_cmd", 0), strlen(json_packet_message("new_cmd", 0)));
                	}
                }
            }
            memset(DeviceData.IrMessageCtr_Info.ir_raw_data, 0x00, sizeof(DeviceData.IrMessageCtr_Info.ir_raw_data) / sizeof(int));
            DeviceData.IrMessageCtr_Info.ir_raw_length = 0;
            DeviceData.IrMessageCtr_Info.new_ctrl_cmd = E_CMD_LEARN;
            break;
        default:
            break;
        }

        vTaskDelay(100);
    }
}
/***********************************************************************************************************************
* Function Name:
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void ir_driver_send_raw(uint16_t *data, int IRlength)
{
    rmt_config_t config;
    config.channel = CHANNEL_IR_RMT_TX;
    //build item
    size_t size = (sizeof(rmt_item32_t) * IRlength);
    rmt_item32_t *item = (rmt_item32_t *)malloc(size); //allocate memory
    memset((void *)item, 0, size);                     //wipe current data in memory
    int i = 0;
    int x = 0;
    APP_LOGD("Sending.....: %d", IRlength);
    while (x < IRlength)
    {
        printf("%d", data[x]);
        printf(",");
        printf("%d", data[x + 1]);
        printf(",");
        //    To build a series of waveforms.
        buildItem(&item[i], data[x], data[x + 1]);
        x = x + 2;
        //        x++;
        i++;
    }
    printf("\n");
    //To send data according to the waveform items.
    rmt_write_items(config.channel, item, IRlength, true);
    //Wait until sending is done.
    rmt_wait_tx_done(config.channel, 100);
    //before we free the data, make sure sending is already done.
    free(item);
}

/***********************************************************************************************************************
* Function Name:
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
bool ir_driver_uninstall(rmt_channel_t channel)
{
	APP_LOGD("ir_driver_uninstall = %d", channel);
	rmt_rx_stop(CHANNEL_IR_RMT_RX);
    if (rmt_driver_uninstall(channel) == ESP_OK)
        return true;
    else
        return false;
}
/***********************************************************************************************************************
* static functions
***********************************************************************************************************************/
/* Converts the RMT level, duration into a positive or negative integer
 * Compatible with the ESP32-RMT-server application
 * Note: most IR receivers have active-low outputs, where the
 *   ESP32-RMT-server application has active-high oututs
 * This function inverts the RMT receive level so the text output is
 *   compatible with ESP32-RMT-server application
 */
static int duration_raw(uint32_t level, uint32_t duration)
{
    if (level == 0)
    {
        return duration;
    }
    else
    {
        //        return -1.0 * duration;
        return duration;
    }
}

/***********************************************************************************************************************
* Function Name: u
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
static void buildItem(rmt_item32_t *item, unsigned int high_us, int low_us)
{
    item->level0 = 1;
    item->duration0 = high_us;
    item->level1 = 0;
    item->duration1 = low_us;
}

/***********************************************************************************************************************
* End of file
***********************************************************************************************************************/
static uint16_t *parser_message(char* data, uint16_t *frequency, uint16_t *length)
{
	static uint16_t data_ir[400];
	char *p = strtok((char*)data, "!,"); //cat chuoi bang cac ky tu ,. va space
	uint8_t index = 0;
	uint16_t buffer_index = 0;
	while (p != NULL)
	{
		if (index == 0)
		{
			//TODO
			*frequency = atoi(p);
		}
		else
		{
			data_ir[buffer_index++] = atoi(p);
		}
		index++;
		p = strtok(NULL, "!,"); //cat chuoi tu vi tri dung lai truoc do
	}
	*length = buffer_index;
	return data_ir;
}

/*
 * mqtt_driver.c
 *
 *  Created on: Nov 17, 2020
 *      Author: Yolo
 */

/***********************************************************************************************************************
* Pragma directive
***********************************************************************************************************************/

/***********************************************************************************************************************
* Includes <System Includes>
***********************************************************************************************************************/
#include "mqtt_driver.h"
/***********************************************************************************************************************
* Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
* Typedef definitions
***********************************************************************************************************************/
typedef struct
{
	esp_mqtt_client_config_t 	xClientInfo;
	char 						PublishTopic[64];
	char						SubcribeTopic[64];
}MQTTClient_Info;


esp_mqtt_client_handle_t xMqttClient;
/***********************************************************************************************************************
* Private global variables and functions
***********************************************************************************************************************/
uint8_t Mqtt_DataRev[MQTT_DATA_LENGTH_MAX] = {0};

/***********************************************************************************************************************
* Exported global variables and functions (to be accessed by other files)
***********************************************************************************************************************/
static esp_err_t xMQTTClient_Event_Handler(esp_mqtt_event_handle_t event);


/***********************************************************************************************************************
* Imported global variables and functions (from other files)
***********************************************************************************************************************/
MQTTClient_Info xMQTTClient_Info =
{
	.xClientInfo =
	{
		.host			= "m13.cloudmqtt.com",
		.port			= 11734,
		.client_id		= "1234adc",
		.username		= "wcewiofp",
		.password		= "fyFZMCLNvoD9",
		.keepalive		= 60,
		.event_handle	= xMQTTClient_Event_Handler,
	},

	.PublishTopic	= "/ir_blaster/sub01",
	.SubcribeTopic	= "/ir_blaster/pub01",
};
/*******************************************************************************
 * Function		: esp_err_t xMQTTClient_Event_Handler(esp_mqtt_event_handle_t xEvent)
 * Description	: mqtt client event
 * Param		: None
 * Return		: None
 ******************************************************************************/
static esp_err_t xMQTTClient_Event_Handler(esp_mqtt_event_handle_t xEvent)
{
    int msg_id;

    // your_context_t *context = event->context;
    switch (xEvent->event_id)
    {
        case MQTT_EVENT_CONNECTED:
            printf("\r\n--- MQTTClient: MQTT_EVENT_CONNECTED\r\n");
            printf("--- MQTTClient: subcribe topic = %s\r\n", xMQTTClient_Info.SubcribeTopic);
            msg_id = esp_mqtt_client_subscribe(xMqttClient, xMQTTClient_Info.SubcribeTopic, 0);
            printf("--- MQTTClient: mqtt client sent subscribe at topic \"%s\", msg_id = %d\r\n", xMQTTClient_Info.SubcribeTopic, msg_id);
            break;

        case MQTT_EVENT_DISCONNECTED:
            printf("\r\n--- MQTTClient: MQTT_EVENT_DISCONNECTED\r\n");
            break;

        case MQTT_EVENT_SUBSCRIBED:
            printf("\r\n--- MQTTClient: MQTT_EVENT_SUBSCRIBED, msg_id = %d\r\n", xEvent->msg_id);
//            msg_id = esp_mqtt_client_publish(xMqttClient, xMQTTClient_Info.PublishTopic, "Hello. I'm esp32", 0, 0, 0);
//            printf("--- MQTTClient: sent publish, msg_id = %d\r\n", msg_id);
            break;

        case MQTT_EVENT_UNSUBSCRIBED:
            printf("\r\n--- MQTTClient: MQTT_EVENT_UNSUBSCRIBED, msg_id = %d\r\n", xEvent->msg_id);
            break;

        case MQTT_EVENT_PUBLISHED:
            printf("\r\n--- MQTTClient: MQTT_EVENT_PUBLISHED, msg_id = %d\r\n", xEvent->msg_id);
            break;

        case MQTT_EVENT_DATA:
            printf("\r\n--- MQTTClient: MQTT_EVENT_DATA\r\n");
            printf("--- MQTTClient: Received Data = %.*s, lenght = %d at Topic = %.*s\r\n", xEvent->data_len, xEvent->data, \
            							xEvent->data_len, xEvent->topic_len, xEvent->topic);
            if(xEvent->data_len > MQTT_DATA_LENGTH_MAX)
            	break;
//            memset(Mqtt_DataRev, 0x00, strlen((char*)Mqtt_DataRev));
//            strncpy((char*)Mqtt_DataRev, (char*)xEvent->data, xEvent->data_len);
//            MQTT_RxCallBack(Mqtt_DataRev, strlen((char*)Mqtt_DataRev), false);
            break;

        case MQTT_EVENT_ERROR:
            printf("\r\n--- MQTTClient: MQTT_EVENT_ERROR\r\n");
            break;
    }
    return ESP_OK;
}

/*******************************************************************************
 * Function		: void vMQTTClient_Start(void)
 * Description	: mqtt client start
 * Param		: None
 * Return		: None
 ******************************************************************************/
void vMQTTClient_Start(void)
{
	printf("\r\n--- MQTTClient: Mqtt client start...\r\n");
	xMqttClient = esp_mqtt_client_init(&xMQTTClient_Info.xClientInfo);
    esp_mqtt_client_start(xMqttClient);
}

/*******************************************************************************
 * Function		: void vMQTTClient_Stop(void)
 * Description	: mqtt client stop
 * Param		: None
 * Return		: None
 ******************************************************************************/
void vMQTTClient_Stop(void)
{
	printf("--- MQTTClient: Mqtt client stop\r\n");
    esp_mqtt_client_stop(xMqttClient);
    esp_mqtt_client_destroy(xMqttClient);
}
/*******************************************************************************

 ******************************************************************************/
void Mqtt_ReceiveData(unsigned char* data_rx, unsigned char data_len, bool from_uart)
{
	printf("\r\n--- Mqtt_ReceiveData: data_rev = %s, len = %d\r\n", data_rx, data_len);
}

/*******************************************************************************

 ******************************************************************************/
void Mqtt_SendData(unsigned char* data_send, unsigned char data_len)
{
	printf("\r\n--- Mqtt_SendData: send data: %s to %s\r\n", data_send, xMQTTClient_Info.PublishTopic);
	esp_mqtt_client_publish(xMqttClient, (const char*)xMQTTClient_Info.PublishTopic, (const char*)data_send, 0, 0, 0);
}
/***********************************************************************************************************************
* static functions
***********************************************************************************************************************/

/***********************************************************************************************************************
* End of file
***********************************************************************************************************************/

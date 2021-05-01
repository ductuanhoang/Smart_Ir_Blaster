/*
 * mqtt_driver.h
 *
 *  Created on: Nov 17, 2020
 *      Author: Yolo
 */

#ifndef MAIN_MQTT_DRIVER_MQTT_DRIVER_H_
#define MAIN_MQTT_DRIVER_MQTT_DRIVER_H_
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "../Common.h"
#include "MqttLib/mqtt_client.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

#define MQTT_DATA_LENGTH_MAX			128


/****************************************************************************/
/***         Exported global functions                                     ***/
/****************************************************************************/






/*******************************************************************************
 * Function		: void vMQTTClient_Start(void)
 * Description	: mqtt client start
 * Param		: None
 * Return		: None
 ******************************************************************************/
void vMQTTClient_Start(void);





/*******************************************************************************
 * Function		: void vMQTTClient_Stop(void)
 * Description	: mqtt client stop
 * Param		: None
 * Return		: None
 ******************************************************************************/
void vMQTTClient_Stop(void);



/*******************************************************************************

 ******************************************************************************/
void (*MQTT_RxCallBack)(unsigned char*/*data_rx*/, unsigned char/*data_len*/, bool/*uart_des*/);

//void (*MQTT_PubLishCallBack)(unsigned char*/*data_publish*/, unsigned char/*data_len*/);



/*******************************************************************************
for user
 ******************************************************************************/
void Mqtt_ReceiveData(unsigned char* data_rx, unsigned char data_len, bool from_uart);

void Mqtt_SendData(unsigned char* data_send, unsigned char data_len);

#endif /* MAIN_MQTT_DRIVER_MQTT_DRIVER_H_ */

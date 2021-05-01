/*
 * bluetooth.h
 *
 *  Created on: Sep 14, 2020
 *      Author: Yolo
 */

#ifndef MAIN_TASK_BLUETOOTH_H_
#define MAIN_TASK_BLUETOOTH_H_

void bluetooth_init( void );

void ble_task(void *pvParameters);

void ble_create_task( void );

void ble_delete_task( void );

#endif /* MAIN_TASK_BLUETOOTH_H_ */

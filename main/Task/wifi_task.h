/*
 * wifi_task.h
 *
 *  Created on: Sep 3, 2020
 *      Author: Yolo
 */

#ifndef MAIN_TASK_WIFI_TASK_H_
#define MAIN_TASK_WIFI_TASK_H_


void wifi_task(void *pvParameters);

char *wifi_get_mac(void);

void wifi_disconnect(void);

void esp32_user_restart( void );

#endif /* MAIN_TASK_WIFI_TASK_H_ */

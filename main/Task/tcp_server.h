/*
 * tcp_server.h
 *
 *  Created on: Aug 26, 2020
 *      Author: Yolo
 */

#ifndef MAIN_TASK_TCP_SERVER_H_
#define MAIN_TASK_TCP_SERVER_H_


void tcp_server_task(void *pvParameters);

void tcp_server_delete_task( void );


void tcp_server_create_task( void );


void tcp_server_transmit(char* DataTx, uint16_t LenTx);

#endif /* MAIN_TASK_TCP_SERVER_H_ */

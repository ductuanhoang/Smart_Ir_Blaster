/*
 * UserTimer.h
 *
 *  Created on: Mar 6, 2019
 *      Author: giangnv3
 */

#ifndef MAIN_INTERFACE_USERTIMER_H_
#define MAIN_INTERFACE_USERTIMER_H_


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "esp_timer.h"
#include "esp_log.h"
#include "esp_sleep.h"

#define TIMER_INTERVAL			1000 /*1ms*/

#define TIMER_INTERVAL_PIEZO	125 /* 0.125ms */
#define TIMER_INTERVAL_PIEZO_CLEAR    125 /* 0.125ms */


void UserTimer_Init(void);


uint32_t usertimer_gettick( void );


void piezo_timer_init( void );

void piezo_timer_disable( void );

void piezo_timer_enable( void );


void piezo_timer_clear_init( void );

void piezo_timer_clear_disable( void );

void piezo_timer_clear_enable( void );

#endif /* MAIN_INTERFACE_USERTIMER_H_ */

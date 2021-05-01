/*
 * ir_driver.h
 *
 *  Created on: Nov 15, 2020
 *      Author: Yolo
 */

#ifndef MAIN_IR_DRIVER_IR_DRIVER_H_
#define MAIN_IR_DRIVER_IR_DRIVER_H_

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "../Common.h"
#include "driver/rmt.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define CHANNEL_IR_RMT_TX			RMT_CHANNEL_2
#define CHANNEL_IR_RMT_RX			RMT_CHANNEL_1

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
#define RMT_CLK_DIV      80    /*!< RMT counter clock divider */
#define RMT_TICK_10_US    (80000000/RMT_CLK_DIV/100000)   /*!< RMT counter value for 10 us.(Source clock is APB clock) */
#define RMT_RX_ACTIVE_LEVEL  1   /*!< Data bit is active high for self test mode */
#define RMT_ITEM_DURATION(d)  	((d & 0x7fff)*10/RMT_TICK_10_US)  /*!< Parse duration time from memory register value */


/****************************************************************************/
/***         Exported global functions                                     ***/
/****************************************************************************/
void ir_driver_create_rx_task(void);

void ir_driver_delete_rx_task(void);

void ir_driver_create_tx_task(void);

void ir_drvier_delete_tx_task(void);

void ir_driver_start(void);

void ir_driver_stop(void);


void ir_simulation_new_ir( void );

bool ir_driver_uninstall(rmt_channel_t channel);

bool ir_driver_tx_init(rmt_channel_t channel, uint32_t freq, uint32_t duty_percent);

void ir_driver_send_raw(uint16_t *data, int IRlength);


#ifdef __cplusplus
}
#endif

#endif /* MAIN_IR_DRIVER_IR_DRIVER_H_ */

/*
 * ir_driver_basic.h
 *
 *  Created on: Jan 7, 2021
 *      Author: ductu
 */

#ifndef MAIN_IR_DRIVER_IR_DRIVER_BASIC_H_
#define MAIN_IR_DRIVER_IR_DRIVER_BASIC_H_
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/


#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "ir_base_config.h"
#include "../Common.h"
#include "ir_driver.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/


/****************************************************************************/
/***         Exported global functions                                     ***/
/****************************************************************************/
void ir_basic_mask_space(uint32_t mask, uint32_t space);
void ir_basic_install_tx_channel( uint32_t freq, uint32_t duty_percent);
void ir_basic_uninstall_tx_channel(void);
void ir_basic_space(uint32_t space);
void ir_basic_mark(uint16_t mask);
#ifdef __cplusplus
}
#endif
#endif /* MAIN_IR_DRIVER_IR_DRIVER_BASIC_H_ */

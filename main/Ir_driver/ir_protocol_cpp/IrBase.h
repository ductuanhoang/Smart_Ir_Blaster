/*
 * IrBase.h
 *
 *  Created on: Jan 11, 2021
 *      Author: ductu
 */

#ifndef MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRBASE_H_
#define MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRBASE_H_


#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/


/****************************************************************************/
/***         Exported global functions                                     ***/
/****************************************************************************/
void IrBase_Send(eProtocol_type type_remote, eCommand_type type_command, uint64_t data, uint8_t nbits, uint8_t repeat);

void IrBase_SendRaw(uint16_t *data_raw, uint16_t length, uint16_t frequency, uint16_t dutycycle, uint8_t repeat);

#ifdef __cplusplus
}
#endif


#endif /* MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRBASE_H_ */

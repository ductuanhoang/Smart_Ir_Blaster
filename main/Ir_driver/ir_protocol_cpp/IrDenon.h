/*
 * IrDenon.h
 *
 *  Created on: Mar 9, 2021
 *      Author: ductu
 */

#ifndef MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRDENON_H_
#define MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRDENON_H_


/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "IrSend.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***         Exported global functions                                     ***/
/****************************************************************************/
class Ir_Denon {
public:
	Ir_Denon();
	virtual ~Ir_Denon();

	void sendDenon(uint64_t data, uint16_t nbits = kDenonBits,
	                 uint16_t repeat = kNoRepeat);
};
#endif /* MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRDENON_H_ */

/*
 * IrNEC.h
 *
 *  Created on: Jan 11, 2021
 *      Author: ductu
 */

#ifndef MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRNEC_H_
#define MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRNEC_H_
#include "IrSend.h"
class Ir_NEC
{
public:
	Ir_NEC(uint32_t frequency, uint16_t dutycycle);
	virtual ~Ir_NEC();

	void sendNEC(uint64_t data, uint16_t nbits = kNECBits,
            uint16_t repeat = kNoRepeat);
private:
	Ir_Send _ir_nec_send;
};
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

#endif /* MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRNEC_H_ */

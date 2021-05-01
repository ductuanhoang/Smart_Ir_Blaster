/*
 * IrGICable.h
 *
 *  Created on: Mar 9, 2021
 *      Author: ductu
 */

#ifndef MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRGICABLE_H_
#define MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRGICABLE_H_


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
class Ir_GICable {
public:
	Ir_GICable(uint32_t frequency, uint16_t dutycycle);
	virtual ~Ir_GICable();
	void sendGICable(uint64_t data, uint16_t nbits = kGicableBits,
            uint16_t repeat = kGicableMinRepeat);
private:
	Ir_Send _ir_GICable;
};
#endif /* MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRGICABLE_H_ */

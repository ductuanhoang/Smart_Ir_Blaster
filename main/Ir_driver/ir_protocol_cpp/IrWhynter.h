/*
 * IrWhynter.h
 *
 *  Created on: Jan 17, 2021
 *      Author: ductu
 */

#ifndef MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRWHYNTER_H_
#define MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRWHYNTER_H_

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

class Ir_Whynter {
public:
	Ir_Whynter(uint32_t frequency, uint16_t dutycycle);
	virtual ~Ir_Whynter();
	void sendWhynter(const uint64_t data, const uint16_t nbits = kWhynterBits,
	                   const uint16_t repeat = kNoRepeat);
private:
	Ir_Send _ir_whynter_send;
};
#endif /* MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRWHYNTER_H_ */

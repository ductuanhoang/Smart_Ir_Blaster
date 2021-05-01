/*
 * IrPioneer.h
 *
 *  Created on: Mar 9, 2021
 *      Author: ductu
 */

#ifndef MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRPIONEER_H_
#define MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRPIONEER_H_


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
class Ir_Pioneer {
public:
	Ir_Pioneer(uint32_t frequency, uint16_t dutycycle);
	virtual ~Ir_Pioneer();
	void sendPioneer(const uint64_t data, const uint16_t nbits = kPioneerBits,
	                   const uint16_t repeat = kNoRepeat);
private:
	Ir_Send _ir_pioneer;
};
#endif /* MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRPIONEER_H_ */

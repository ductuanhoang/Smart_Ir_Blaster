/*
 * IrLasertag.h
 *
 *  Created on: Mar 9, 2021
 *      Author: ductu
 */

#ifndef MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRLASERTAG_H_
#define MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRLASERTAG_H_


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
class Ir_Lasertag {
public:
	Ir_Lasertag(uint32_t frequency, uint16_t dutycycle);
	virtual ~Ir_Lasertag();
	void sendLasertag(uint64_t data, uint16_t nbits = kLasertagBits,
	                    uint16_t repeat = kLasertagMinRepeat);
private:
	Ir_Send _ir_lasertag;
};
#endif /* MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRLASERTAG_H_ */

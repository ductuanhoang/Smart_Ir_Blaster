/*
 * IrLutron.h
 *
 *  Created on: Jan 17, 2021
 *      Author: ductu
 */

#ifndef MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRLUTRON_H_
#define MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRLUTRON_H_


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
class Ir_Lutron {
public:
	Ir_Lutron(uint32_t frequency, uint16_t dutycycle);
	virtual ~Ir_Lutron();
	void sendLutron(uint64_t data, uint16_t nbits = kLutronBits,
	                  uint16_t repeat = kNoRepeat);
private:
	Ir_Send _ir_lutron_send;
};
#endif /* MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRLUTRON_H_ */

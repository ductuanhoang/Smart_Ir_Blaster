/*
 * IrMWM.h
 *
 *  Created on: Jan 17, 2021
 *      Author: ductu
 */

#ifndef MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRMWM_H_
#define MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRMWM_H_

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

class Ir_MWM {
public:
	Ir_MWM(uint32_t frequency, uint16_t dutycycle);
	virtual ~Ir_MWM();
	void sendMWM(const unsigned char data[], const uint16_t nbytes,
	               const uint16_t repeat = kNoRepeat);
private:
	  Ir_Send _ir_mwm_send;
};
#endif /* MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRMWM_H_ */

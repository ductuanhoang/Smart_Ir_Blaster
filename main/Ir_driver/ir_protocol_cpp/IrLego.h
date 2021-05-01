/*
 * IrLego.h
 *
 *  Created on: Mar 3, 2021
 *      Author: ductu
 */

#ifndef MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRLEGO_H_
#define MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRLEGO_H_
#include "IrSend.h"
class IrLego {
public:
	IrLego(uint32_t frequency, uint16_t dutycycle);
	virtual ~IrLego();

	void sendLegoPf(const uint64_t data, const uint16_t nbits = kLegoPfBits,
            const uint16_t repeat = kLegoPfMinRepeat);
private:
	Ir_Send _ir_legopf_send;
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

#endif /* MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRLEGO_H_ */

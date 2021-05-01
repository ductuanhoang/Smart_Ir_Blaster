/*
 * IrRCMM.h
 *
 *  Created on: Jan 19, 2021
 *      Author: ductu
 */

#ifndef MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRRCMM_H_
#define MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRRCMM_H_


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
class Ir_RCMM {
public:
	Ir_RCMM(uint32_t frequency, uint16_t dutycycle);
	virtual ~Ir_RCMM();
	void sendRCMM(uint64_t data, uint16_t nbits = kRCMMBits,
	                uint16_t repeat = kNoRepeat);
private:
	Ir_Send _ir_rcmm_send;
};
#endif /* MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRRCMM_H_ */

/*
 * IrNikai.h
 *
 *  Created on: Mar 3, 2021
 *      Author: ductu
 */

#ifndef MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRNIKAI_H_
#define MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRNIKAI_H_
#include "IrSend.h"

class IrNikai {
public:
	IrNikai(uint32_t frequency, uint16_t dutycycle);
	virtual ~IrNikai();
	void sendNikai(uint64_t data, uint16_t nbits = kNikaiBits,
            uint16_t repeat = kNoRepeat);
private:
	Ir_Send _ir_remote_nikai;
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

#endif /* MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRNIKAI_H_ */

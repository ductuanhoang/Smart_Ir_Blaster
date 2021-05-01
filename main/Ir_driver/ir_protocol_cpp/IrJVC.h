/*
 * IrJVC.h
 *
 *  Created on: Jan 12, 2021
 *      Author: ductu
 */

#ifndef MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRJVC_H_
#define MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRJVC_H_

#include "IrSend.h"

class Ir_JVC
{
public:
	Ir_JVC(uint32_t frequency, uint16_t dutycycle);
	virtual ~Ir_JVC();

	void sendJVC(uint64_t data, uint16_t nbits = kJvcBits,
            uint16_t repeat = kNoRepeat);
private:
	Ir_Send _jvc_ir_send;
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

#endif /* MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRJVC_H_ */

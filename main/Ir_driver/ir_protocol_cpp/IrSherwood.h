/*
 * IrSherwood.h
 *
 *  Created on: Mar 3, 2021
 *      Author: ductu
 */

#ifndef MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRSHERWOOD_H_
#define MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRSHERWOOD_H_
#include "IrSend.h"
class IrSherwood {
public:
	IrSherwood();
	virtual ~IrSherwood();

	void sendSherwood(uint64_t data, uint16_t nbits = kSherwoodBits,
	                    uint16_t repeat = kSherwoodMinRepeat);
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

#endif /* MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRSHERWOOD_H_ */

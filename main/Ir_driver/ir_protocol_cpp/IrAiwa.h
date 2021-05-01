/*
 * IrAiwa.h
 *
 *  Created on: Jan 13, 2021
 *      Author: ductu
 */

#ifndef MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRAIWA_H_
#define MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRAIWA_H_


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
class Ir_Aiwa {
public:
	Ir_Aiwa();
	virtual ~Ir_Aiwa();
	void sendAiwaRCT501(uint64_t data, uint16_t nbits = kAiwaRcT501Bits,
            uint16_t repeat = kAiwaRcT501MinRepeats);
};
#endif /* MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRAIWA_H_ */

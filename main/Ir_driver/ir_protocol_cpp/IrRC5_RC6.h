/*
 * IrRC5_RC6.h
 *
 *  Created on: Jan 16, 2021
 *      Author: ductu
 */

#ifndef MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRRC5_RC6_H_
#define MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRRC5_RC6_H_
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
class Ir_RC5_RC6 {
public:
	Ir_RC5_RC6(uint32_t frequency, uint16_t dutycycle);
	virtual ~Ir_RC5_RC6();
	void sendRC5(const uint64_t data, uint16_t nbits = kRC5XBits,
                 const uint16_t repeat = kNoRepeat);
	void sendRC6(const uint64_t data, const uint16_t nbits = kRC6Mode0Bits,
                 const uint16_t repeat = kNoRepeat);
private:
    Ir_Send _ir_rc5_rc6_send;
};
#endif /* MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRRC5_RC6_H_ */

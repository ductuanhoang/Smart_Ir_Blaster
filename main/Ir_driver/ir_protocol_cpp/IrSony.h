/*
 * IrSony.h
 *
 *  Created on: Jan 16, 2021
 *      Author: ductu
 */

#ifndef MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRSONY_H_
#define MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRSONY_H_
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
class Ir_Sony {
public:
	Ir_Sony(uint32_t frequency, uint16_t dutycycle);
	virtual ~Ir_Sony();
    // sendSony() should typically be called with repeat=2 as Sony devices
    // expect the code to be sent at least 3 times. (code + 2 repeats = 3 codes)
    // Legacy use of this procedure was to only send a single code so call it with
    // repeat=0 for backward compatibility. As of v2.0 it defaults to sending
    // a Sony command that will be accepted be a device.
    void sendSony(const uint64_t data, const uint16_t nbits = kSony20Bits,
                  const uint16_t repeat = kSonyMinRepeat);
    void sendSony38(const uint64_t data, const uint16_t nbits = kSony20Bits,
                    const uint16_t repeat = kSonyMinRepeat + 1);

private:
    void _sendSony(const uint64_t data, const uint16_t nbits,
                   const uint16_t repeat, const uint16_t freq);
    Ir_Send _ir_sony_send;
};
#endif /* MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRSONY_H_ */

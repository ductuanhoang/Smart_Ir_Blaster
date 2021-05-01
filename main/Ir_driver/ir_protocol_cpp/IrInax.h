/*
 * IrInax.h
 *
 *  Created on: Jan 17, 2021
 *      Author: ductu
 */

#ifndef MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRINAX_H_
#define MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRINAX_H_


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
class Ir_Inax {
public:
	Ir_Inax(uint32_t frequency, uint16_t dutycycle);
	virtual ~Ir_Inax();
    void sendInax(const uint64_t data, const uint16_t nbits = kInaxBits,
             const uint16_t repeat = kInaxMinRepeat);
private:
    Ir_Send _ir_inax_send;
};
#endif /* MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRINAX_H_ */

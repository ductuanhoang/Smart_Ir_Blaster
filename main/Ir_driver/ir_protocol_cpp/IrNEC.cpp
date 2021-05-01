/*
 * IrNEC.cpp
 *
 *  Created on: Jan 11, 2021
 *      Author: ductu
 */

/***********************************************************************************************************************
 * Pragma directive
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes <System Includes>
 ***********************************************************************************************************************/
#include "IrNEC.h"

/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/
#define kNecTick 560
#define kNecHdrMarkTicks 16
#define kNecHdrMark kNecHdrMarkTicks *kNecTick
#define kNecHdrSpaceTicks 8
#define kNecHdrSpace kNecHdrSpaceTicks *kNecTick
#define kNecBitMarkTicks 1
#define kNecBitMark kNecBitMarkTicks *kNecTick
#define kNecOneSpaceTicks 3
#define kNecOneSpace kNecOneSpaceTicks *kNecTick
#define kNecZeroSpaceTicks 1
#define kNecZeroSpace kNecZeroSpaceTicks *kNecTick
#define kNecRptSpaceTicks 4
#define kNecRptSpace kNecRptSpaceTicks *kNecTick
#define kNecRptLength 4
#define kNecMinCommandLengthTicks 193
#define kNecMinCommandLength kNecMinCommandLengthTicks *kNecTick
#define kNecMinGap kNecMinCommandLength - (kNecHdrMark + kNecHdrSpace + kNECBits * (kNecBitMark + kNecOneSpace) + kNecBitMark)
#define kNecMinGapTicks kNecMinCommandLengthTicks - (kNecHdrMarkTicks + kNecHdrSpaceTicks + kNECBits * (kNecBitMarkTicks + kNecOneSpaceTicks) + kNecBitMarkTicks)

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables and functions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global variables and functions (to be accessed by other files)
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Imported global variables and functions (from other files)
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: 
 * Description  :
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
Ir_NEC::Ir_NEC(uint32_t frequency, uint16_t dutycycle)
				:_ir_nec_send(frequency, dutycycle)
{
	// TODO Auto-generated constructor stub
}

Ir_NEC::~Ir_NEC()
{
	// TODO Auto-generated destructor stub
}

void Ir_NEC::sendNEC(uint64_t data, uint16_t nbits, uint16_t repeat)
{
	_ir_nec_send.sendGeneric(kNecHdrMark, kNecHdrSpace, kNecBitMark, kNecOneSpace, kNecBitMark,
				kNecZeroSpace, kNecBitMark, kNecMinGap, kNecMinCommandLength,
				data, nbits, 38, true, 0, // Repeats are handled later.
				33);
	// Optional command repeat sequence.
	if (repeat)
		_ir_nec_send.sendGeneric(kNecHdrMark, kNecRptSpace, 0, 0, 0, 0, // No actual data sent.
					kNecBitMark, kNecMinGap, kNecMinCommandLength, 0,
					0,					  // No data to be sent.
					38, true, repeat - 1, // We've already sent a one message.
					33);
}
/***********************************************************************************************************************
 * Static Functions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/

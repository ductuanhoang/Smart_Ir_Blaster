/*
 * IrWhynter.cpp
 *
 *  Created on: Jan 17, 2021
 *      Author: ductu
 */




/***********************************************************************************************************************
 * Pragma directive
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes <System Includes>
 ***********************************************************************************************************************/
#include "IrWhynter.h"

/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/
// Constants
const uint16_t kWhynterTick = 50;
const uint16_t kWhynterHdrMarkTicks = 57;
const uint16_t kWhynterHdrMark = kWhynterHdrMarkTicks * kWhynterTick;
const uint16_t kWhynterHdrSpaceTicks = 57;
const uint16_t kWhynterHdrSpace = kWhynterHdrSpaceTicks * kWhynterTick;
const uint16_t kWhynterBitMarkTicks = 15;
const uint16_t kWhynterBitMark = kWhynterBitMarkTicks * kWhynterTick;
const uint16_t kWhynterOneSpaceTicks = 43;
const uint16_t kWhynterOneSpace = kWhynterOneSpaceTicks * kWhynterTick;
const uint16_t kWhynterZeroSpaceTicks = 15;
const uint16_t kWhynterZeroSpace = kWhynterZeroSpaceTicks * kWhynterTick;
const uint16_t kWhynterMinCommandLengthTicks = 2160;  // Totally made up value.
const uint32_t kWhynterMinCommandLength =
    kWhynterMinCommandLengthTicks * kWhynterTick;
const uint16_t kWhynterMinGapTicks =
    kWhynterMinCommandLengthTicks -
    (2 * (kWhynterBitMarkTicks + kWhynterZeroSpaceTicks) +
     kWhynterBits * (kWhynterBitMarkTicks + kWhynterOneSpaceTicks));
const uint16_t kWhynterMinGap = kWhynterMinGapTicks * kWhynterTick;
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
Ir_Whynter::Ir_Whynter(uint32_t frequency, uint16_t dutycycle): _ir_whynter_send (frequency, dutycycle) {
	// TODO Auto-generated constructor stub

}

Ir_Whynter::~Ir_Whynter() {
	// TODO Auto-generated destructor stub
}

void Ir_Whynter::sendWhynter(const uint64_t data, const uint16_t nbits,
        const uint16_t repeat) {
	// Set IR carrier frequency
//	enableIROut(38);
	for (uint16_t i = 0; i <= repeat; i++) {
	// (Pre-)Header
		_ir_whynter_send.mark(kWhynterBitMark);
		_ir_whynter_send.space(kWhynterZeroSpace);
		_ir_whynter_send.sendGeneric(
	kWhynterHdrMark, kWhynterHdrSpace, kWhynterBitMark, kWhynterOneSpace,
	kWhynterBitMark, kWhynterZeroSpace, kWhynterBitMark, kWhynterMinGap,
	kWhynterMinCommandLength - (kWhynterBitMark + kWhynterZeroSpace), data,
	nbits, 38, true, 0,  // Repeats are already handled.
	50);
	}
}
/***********************************************************************************************************************
 * Static Functions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/


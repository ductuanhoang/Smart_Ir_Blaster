/*
 * IrRC5_RC6.cpp
 *
 *  Created on: Jan 16, 2021
 *      Author: ductu
 */
/***********************************************************************************************************************
 * Pragma directive
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes <System Includes>
 ***********************************************************************************************************************/
#include "IrRC5_RC6.h"
/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/

// Constants
// RC-5/RC-5X
const uint16_t kRc5T1 = 889;
const uint32_t kRc5MinCommandLength = 113778;
const uint32_t kRc5MinGap = kRc5MinCommandLength - kRC5RawBits * (2 * kRc5T1);
const uint16_t kRc5ToggleMask = 0x800;  // The 12th bit.
const uint16_t kRc5SamplesMin = 11;

// RC-6
const uint16_t kRc6Tick = 444;
const uint16_t kRc6HdrMarkTicks = 6;
const uint16_t kRc6HdrMark = kRc6HdrMarkTicks * kRc6Tick;
const uint16_t kRc6HdrSpaceTicks = 2;
const uint16_t kRc6HdrSpace = kRc6HdrSpaceTicks * kRc6Tick;
const uint16_t kRc6RptLengthTicks = 187;
const uint32_t kRc6RptLength = kRc6RptLengthTicks * kRc6Tick;
const uint32_t kRc6ToggleMask = 0x10000UL;  // The 17th bit.
const uint16_t kRc6_36ToggleMask = 0x8000;  // The 16th bit.

// Common (getRClevel())
const int16_t kMark = 0;
const int16_t kSpace = 1;
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
Ir_RC5_RC6::Ir_RC5_RC6(uint32_t frequency, uint16_t dutycycle): _ir_rc5_rc6_send(frequency, dutycycle){
	// TODO Auto-generated constructor stub
}

Ir_RC5_RC6::~Ir_RC5_RC6() {
	// TODO Auto-generated destructor stub
}

void Ir_RC5_RC6::sendRC5(const uint64_t data, uint16_t nbits, const uint16_t repeat) {
//    if (nbits > sizeof(data) * 8) return;  // We can't send something that big.
//    bool skipSpace = true;
//    bool field_bit = true;
//field_bit    // Set 36kHz IR carrier frequency & a 1/4 (25%) duty cycle.
//    enableIROut(36, 25);
//    if (nbits >= kRC5XBits) {  // Is this a RC-5X message?
//        // field bit is the inverted MSB of RC-5X data.
//        field_bit = ((data >> (nbits - 1)) ^ 1) & 1;
//        nbits--;
//    }

//    for (uint16_t i = 0; i <= repeat; i++) {
//        // Header
//        // First start bit (0x1). space, then mark.
//        if (skipSpace)
//            skipSpace = false;  // First time through, we assume the leading space().
//        else
//            space(kRc5T1);
//        mark(kRc5T1);
//        // Field/Second start bit.
//        if (field_bit) {  // Send a 1. Normal for RC-5.
//            space(kRc5T1);
//            mark(kRc5T1);
//        } else {  // Send a 0. Special case for RC-5X. Means 7th command bit is 1.
//            mark(kRc5T1);
//            space(kRc5T1);
//        }
//
//        // Data
//        for (uint64_t mask = 1ULL << (nbits - 1); mask; mask >>= 1)
//            if (data & mask) {  // 1
//                _ir_rc5_rc6_send.mask_space(kRc5T1, kRc5T1);
//            } else {         // 0
//                _ir_rc5_rc6_send.mask_space(kRc5T1, kRc5T1);
//            }
//        // Footer
//        _ir_rc5_rc6_send.mask_space(kRc5MinGap, 0x7FFF);
//    }
}

void Ir_RC5_RC6::sendRC6(const uint64_t data, const uint16_t nbits, const uint16_t repeat) {
    // Check we can send the number of bits requested.
    if (nbits > sizeof(data) * 8) return;
    // Set 36kHz IR carrier frequency & a 1/3 (33%) duty cycle.
    for (uint16_t r = 0; r <= repeat; r++) {
//        // Header
    	_ir_rc5_rc6_send.mark_space(kRc6HdrMark, kRc6HdrSpace);
        // Start bit.
    	_ir_rc5_rc6_send.mark_space(kRc6Tick, kRc6Tick);
        // Data
        uint16_t bitTime;
        for (uint64_t i = 1, mask = 1ULL << (nbits - 1); mask; i++, mask >>= 1) {
            if (i == 4)  // The fourth bit we send is a "double width trailer bit".
                bitTime = 2 * kRc6Tick;  // double-wide trailer bit
            else
                bitTime = kRc6Tick;  // Normal bit
            if (data & mask) {     // 1
            	_ir_rc5_rc6_send.mark_space(bitTime, bitTime);
            } else {  // 0
            	_ir_rc5_rc6_send.mark_space(bitTime, bitTime);
            }
        }
        // Footer
        _ir_rc5_rc6_send.mark_space(0, 0);
    }
}
/***********************************************************************************************************************
 * Static Functions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/


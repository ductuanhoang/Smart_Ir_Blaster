/*
 * IrRCMM.cpp
 *
 *  Created on: Jan 19, 2021
 *      Author: ductu
 */
/***********************************************************************************************************************
 * Pragma directive
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes <System Includes>
 ***********************************************************************************************************************/
#include "IrRCMM.h"

/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/
// Constants
const uint16_t kRcmmTick = 28;  // Technically it would be 27.777*
const uint16_t kRcmmHdrMarkTicks = 15;
const uint16_t kRcmmHdrMark = 416;
const uint16_t kRcmmHdrSpaceTicks = 10;
const uint16_t kRcmmHdrSpace = 277;
const uint16_t kRcmmBitMarkTicks = 6;
const uint16_t kRcmmBitMark = 166;
const uint16_t kRcmmBitSpace0Ticks = 10;
const uint16_t kRcmmBitSpace0 = 277;
const uint16_t kRcmmBitSpace1Ticks = 16;
const uint16_t kRcmmBitSpace1 = 444;
const uint16_t kRcmmBitSpace2Ticks = 22;
const uint16_t kRcmmBitSpace2 = 611;
const uint16_t kRcmmBitSpace3Ticks = 28;
const uint16_t kRcmmBitSpace3 = 777;
const uint16_t kRcmmRptLengthTicks = 992;
const uint32_t kRcmmRptLength = 27778;
const uint16_t kRcmmMinGapTicks = 120;
const uint32_t kRcmmMinGap = 3360;
// Use a tolerance of +/-10% when matching some data spaces.
const uint8_t kRcmmTolerance = 10;
const uint16_t kRcmmExcess = 50;
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
Ir_RCMM::Ir_RCMM(uint32_t frequency, uint16_t dutycycle): _ir_rcmm_send(frequency, dutycycle){
	// TODO Auto-generated constructor stub

}

Ir_RCMM::~Ir_RCMM() {
	// TODO Auto-generated destructor stub
}

/***********************************************************************************************************************
 * Static Functions
 ***********************************************************************************************************************/
void Ir_RCMM::sendRCMM(uint64_t data, uint16_t nbits, uint16_t repeat) {
  // Set 36kHz IR carrier frequency & a 1/3 (33%) duty cycle.
//  enableIROut(36, 33);
//  IRtimer usecs = IRtimer();

  for (uint16_t r = 0; r <= repeat; r++) {
//    usecs.reset();
    // Header
	_ir_rcmm_send.mark_space(kRcmmHdrMark, kRcmmHdrSpace);
    // Data
    uint64_t mask = 0b11ULL << (nbits - 2);
    // RC-MM sends data 2 bits at a time.
    for (int32_t i = nbits; i > 0; i -= 2) {
    	_ir_rcmm_send.mark(kRcmmBitMark);
      // Grab the next Most Significant Bits to send.
      switch ((data & mask) >> (i - 2)) {
        case 0b00:
        	_ir_rcmm_send.space(kRcmmBitSpace0);
          break;
        case 0b01:
        	_ir_rcmm_send.space(kRcmmBitSpace1);
          break;
        case 0b10:
        	_ir_rcmm_send.space(kRcmmBitSpace2);
          break;
        case 0b11:
        	_ir_rcmm_send.space(kRcmmBitSpace3);
          break;
      }
      mask >>= 2;
    }
    // Footer
    _ir_rcmm_send.mark(kRcmmBitMark);
    // Protocol requires us to wait at least kRcmmRptLength usecs from the
    // start or kRcmmMinGap usecs.
//    space(std::max(kRcmmRptLength - usecs.elapsed(), kRcmmMinGap));
  }
}
/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/


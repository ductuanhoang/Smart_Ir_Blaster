/*
 * IrLasertag.cpp
 *
 *  Created on: Mar 9, 2021
 *      Author: ductu
 */




/***********************************************************************************************************************
 * Pragma directive
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes <System Includes>
 ***********************************************************************************************************************/
#include "IrLasertag.h"

/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/
// Constants
const uint16_t kLasertagMinSamples = 13;
const uint16_t kLasertagTick = 333;
const uint32_t kLasertagMinGap = kDefaultMessageGap;  // Just a guess.
const uint8_t kLasertagTolerance = 0;     // Percentage error margin.
const uint16_t kLasertagExcess = 0;       // See kMarkExcess.
const uint16_t kLasertagDelta = 150;  // Use instead of Excess and Tolerance.
const int16_t kSpace = 1;
const int16_t kMark = 0;
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
Ir_Lasertag::Ir_Lasertag(uint32_t frequency, uint16_t dutycycle):_ir_lasertag(frequency, dutycycle) {
	// TODO Auto-generated constructor stub

}

Ir_Lasertag::~Ir_Lasertag() {
	// TODO Auto-generated destructor stub
}
/***********************************************************************************************************************
 * Static Functions
 ***********************************************************************************************************************/
void Ir_Lasertag::sendLasertag(uint64_t data, uint16_t nbits, uint16_t repeat) {
  if (nbits > sizeof(data) * 8) return;  // We can't send something that big.

  // Set 36kHz IR carrier frequency & a 1/4 (25%) duty cycle.
  // NOTE: duty cycle is not confirmed. Just guessing based on RC5/6 protocols.

  for (uint16_t i = 0; i <= repeat; i++) {
    // Data
    for (uint64_t mask = 1ULL << (nbits - 1); mask; mask >>= 1)
      if (data & mask) {       // 1
    	  _ir_lasertag.space(kLasertagTick);  // 1 is space, then mark.
    	  _ir_lasertag.mark(kLasertagTick);
      } else {                // 0
    	  _ir_lasertag.mark(kLasertagTick);  // 0 is mark, then space.
    	  _ir_lasertag.space(kLasertagTick);
      }
    // Footer
    _ir_lasertag.space(kLasertagMinGap);
  }
}
/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/


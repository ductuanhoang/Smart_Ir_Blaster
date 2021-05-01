/*
 * IrPioneer.cpp
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
#include "IrPioneer.h"

/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/
// Constants
// Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1220
const uint16_t kPioneerTick = 534;  ///< uSeconds.
const uint16_t kPioneerHdrMark = 8506;  ///< uSeconds.
const uint16_t kPioneerHdrSpace = 4191;  ///< uSeconds.
const uint16_t kPioneerBitMark = 568;  ///< uSeconds.
const uint16_t kPioneerOneSpace = 1542;  ///< uSeconds.
const uint16_t kPioneerZeroSpace = 487;  ///< uSeconds.
const uint32_t kPioneerMinCommandLength = 84906;  ///< uSeconds.
const uint32_t kPioneerMinGap = 25181;  ///< uSeconds.
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

Ir_Pioneer::Ir_Pioneer(uint32_t frequency, uint16_t dutycycle):_ir_pioneer(frequency, dutycycle) {
	// TODO Auto-generated constructor stub

}

Ir_Pioneer::~Ir_Pioneer() {
	// TODO Auto-generated destructor stub
}

void Ir_Pioneer::sendPioneer(const uint64_t data, const uint16_t nbits,
                         const uint16_t repeat) {
	  // If nbits is to big, abort.
	  if (nbits > sizeof(data) * 8) return;
	  for (uint16_t r = 0; r <= repeat; r++) {
	    // don't use NEC repeat but repeat the whole sequence
	    if (nbits > 32) {
	    	_ir_pioneer.sendGeneric(kPioneerHdrMark, kPioneerHdrSpace,
	                  kPioneerBitMark, kPioneerOneSpace,
	                  kPioneerBitMark, kPioneerZeroSpace,
	                  kPioneerBitMark, kPioneerMinGap,
	                  kPioneerMinCommandLength,
	                  data >> 32, nbits - 32, 40, true, 0, 33);
	    }
	    _ir_pioneer.sendGeneric(kPioneerHdrMark, kPioneerHdrSpace,
	                kPioneerBitMark, kPioneerOneSpace,
	                kPioneerBitMark, kPioneerZeroSpace,
	                kPioneerBitMark, kPioneerMinGap,
	                kPioneerMinCommandLength,
	                data, nbits > 32 ? 32 : nbits, 40, true, 0, 33);
	  }
}
/***********************************************************************************************************************
 * Static Functions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/


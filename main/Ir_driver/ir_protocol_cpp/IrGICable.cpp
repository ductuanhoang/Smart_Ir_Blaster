/*
 * IrGICable.cpp
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
#include "IrGICable.h"

/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/
// Constants
const uint16_t kGicableHdrMark = 9000;
const uint16_t kGicableHdrSpace = 4400;
const uint16_t kGicableBitMark = 550;
const uint16_t kGicableOneSpace = 4400;
const uint16_t kGicableZeroSpace = 2200;
const uint16_t kGicableRptSpace = 2200;
const uint32_t kGicableMinCommandLength = 99600;
const uint32_t kGicableMinGap =
    kGicableMinCommandLength -
    (kGicableHdrMark + kGicableHdrSpace +
     kGicableBits * (kGicableBitMark + kGicableOneSpace) + kGicableBitMark);
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
Ir_GICable::Ir_GICable(uint32_t frequency, uint16_t dutycycle):_ir_GICable(frequency, dutycycle) {
	// TODO Auto-generated constructor stub

}

Ir_GICable::~Ir_GICable() {
	// TODO Auto-generated destructor stub
}
/***********************************************************************************************************************
 * Static Functions
 ***********************************************************************************************************************/
void Ir_GICable::sendGICable(uint64_t data, uint16_t nbits, uint16_t repeat)
{
	_ir_GICable.sendGeneric(kGicableHdrMark, kGicableHdrSpace, kGicableBitMark,
	              kGicableOneSpace, kGicableBitMark, kGicableZeroSpace,
	              kGicableBitMark, kGicableMinGap, kGicableMinCommandLength, data,
	              nbits, 39, true, 0,  // Repeats are handled later.
	              50);
	  // Message repeat sequence.
	  if (repeat)
		  _ir_GICable.sendGeneric(kGicableHdrMark, kGicableRptSpace, 0, 0, 0,
	                0,  // No actual data sent.
	                kGicableBitMark, kGicableMinGap, kGicableMinCommandLength, 0,
	                0,  // No data to be sent.
	                39, true, repeat - 1, 50);
}
/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/


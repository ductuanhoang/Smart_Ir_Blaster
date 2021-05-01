/*
 * IrJVC.cpp
 *
 *  Created on: Jan 12, 2021
 *      Author: ductu
 */
/***********************************************************************************************************************
 * Pragma directive
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes <System Includes>
 ***********************************************************************************************************************/
#include "IrJVC.h"

/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/
// Constants
const uint16_t kJvcTick = 75;
const uint16_t kJvcHdrMarkTicks = 112;
const uint16_t kJvcHdrMark = kJvcHdrMarkTicks * kJvcTick;
const uint16_t kJvcHdrSpaceTicks = 56;
const uint16_t kJvcHdrSpace = kJvcHdrSpaceTicks * kJvcTick;
const uint16_t kJvcBitMarkTicks = 7;
const uint16_t kJvcBitMark = kJvcBitMarkTicks * kJvcTick;
const uint16_t kJvcOneSpaceTicks = 23;
const uint16_t kJvcOneSpace = kJvcOneSpaceTicks * kJvcTick;
const uint16_t kJvcZeroSpaceTicks = 7;
const uint16_t kJvcZeroSpace = kJvcZeroSpaceTicks * kJvcTick;
const uint16_t kJvcRptLengthTicks = 800;
const uint16_t kJvcRptLength = kJvcRptLengthTicks * kJvcTick;
const uint16_t kJvcMinGapTicks =
	kJvcRptLengthTicks -
	(kJvcHdrMarkTicks + kJvcHdrSpaceTicks +
	 kJvcBits * (kJvcBitMarkTicks + kJvcOneSpaceTicks) + kJvcBitMarkTicks);
const uint16_t kJvcMinGap = kJvcMinGapTicks * kJvcTick;
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
Ir_JVC::Ir_JVC(uint32_t frequency, uint16_t dutycycle) :_jvc_ir_send(frequency, dutycycle)
{

}

Ir_JVC::~Ir_JVC()
{
	// TODO Auto-generated destructor stub
}

void Ir_JVC::sendJVC(uint64_t data, uint16_t nbits, uint16_t repeat)
{
	// Header
	// Only sent for the first message.
	_jvc_ir_send.mark_space(kJvcHdrMark, kJvcHdrSpace);

	// We always send the data & footer at least once, hence '<= repeat'.
	for (uint16_t i = 0; i <= repeat; i++)
	{
		_jvc_ir_send.sendGeneric(0, 0, // No Header
					kJvcBitMark, kJvcOneSpace, kJvcBitMark, kJvcZeroSpace,
					kJvcBitMark, kJvcMinGap, data, nbits, 38, true,
					0, // Repeats are handles elsewhere.
					33);
	}
}
/***********************************************************************************************************************
 * Static Functions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/

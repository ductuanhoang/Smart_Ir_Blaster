/*
 * IrNikai.cpp
 *
 *  Created on: Mar 3, 2021
 *      Author: ductu
 */

/***********************************************************************************************************************
 * Pragma directive
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes <System Includes>
 ***********************************************************************************************************************/
#include "IrNikai.h"

/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/
// Constants
const uint16_t kNikaiTick = 500;
const uint16_t kNikaiHdrMarkTicks = 8;
const uint16_t kNikaiHdrMark = kNikaiHdrMarkTicks * kNikaiTick;
const uint16_t kNikaiHdrSpaceTicks = 8;
const uint16_t kNikaiHdrSpace = kNikaiHdrSpaceTicks * kNikaiTick;
const uint16_t kNikaiBitMarkTicks = 1;
const uint16_t kNikaiBitMark = kNikaiBitMarkTicks * kNikaiTick;
const uint16_t kNikaiOneSpaceTicks = 2;
const uint16_t kNikaiOneSpace = kNikaiOneSpaceTicks * kNikaiTick;
const uint16_t kNikaiZeroSpaceTicks = 4;
const uint16_t kNikaiZeroSpace = kNikaiZeroSpaceTicks * kNikaiTick;
const uint16_t kNikaiMinGapTicks = 17;
const uint16_t kNikaiMinGap = kNikaiMinGapTicks * kNikaiTick;
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
IrNikai::IrNikai(uint32_t frequency, uint16_t dutycycle):_ir_remote_nikai(frequency, dutycycle) {
	// TODO Auto-generated constructor stub

}

IrNikai::~IrNikai() {
	// TODO Auto-generated destructor stub
}
/***********************************************************************************************************************
 * Static Functions
 ***********************************************************************************************************************/
/// Send a Nikai formatted message.
/// Status: STABLE / Working.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IrNikai::sendNikai(uint64_t data, uint16_t nbits, uint16_t repeat) {
  _ir_remote_nikai.sendGeneric(kNikaiHdrMark, kNikaiHdrSpace, kNikaiBitMark, kNikaiOneSpace,
              kNikaiBitMark, kNikaiZeroSpace, kNikaiBitMark, kNikaiMinGap, data,
              nbits, 38, true, repeat, 33);
}
/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/


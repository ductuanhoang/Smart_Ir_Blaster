/*
 * IrSony.cpp
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
#include "IrSony.h"
/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/
// Constants
const uint16_t kSonyTick = 200;
const uint16_t kSonyHdrMarkTicks = 12;
const uint16_t kSonyHdrMark = kSonyHdrMarkTicks * kSonyTick;
const uint16_t kSonySpaceTicks = 3;
const uint16_t kSonySpace = kSonySpaceTicks * kSonyTick;
const uint16_t kSonyOneMarkTicks = 6;
const uint16_t kSonyOneMark = kSonyOneMarkTicks * kSonyTick;
const uint16_t kSonyZeroMarkTicks = 3;
const uint16_t kSonyZeroMark = kSonyZeroMarkTicks * kSonyTick;
const uint16_t kSonyRptLengthTicks = 225;
const uint16_t kSonyRptLength = kSonyRptLengthTicks * kSonyTick;
const uint16_t kSonyMinGapTicks = 50;
const uint16_t kSonyMinGap = kSonyMinGapTicks * kSonyTick;
const uint16_t kSonyStdFreq = 40000;  // kHz
const uint16_t kSonyAltFreq = 38000;  // kHz
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
Ir_Sony::Ir_Sony(uint32_t frequency, uint16_t dutycycle): _ir_sony_send(frequency, dutycycle) {
	// TODO Auto-generated constructor stub

}

Ir_Sony::~Ir_Sony() {
	// TODO Auto-generated destructor stub
}

void Ir_Sony::sendSony(const uint64_t data, const uint16_t nbits, const uint16_t repeat) {
    _sendSony(data, nbits, repeat, kSonyStdFreq);
}

void Ir_Sony::sendSony38(const uint64_t data, const uint16_t nbits, const uint16_t repeat) {
    _sendSony(data, nbits, repeat, kSonyAltFreq);
}
/***********************************************************************************************************************
 * Static Functions
 ***********************************************************************************************************************/
void Ir_Sony::_sendSony(const uint64_t data, const uint16_t nbits, const uint16_t repeat, const uint16_t freq) {
	_ir_sony_send.sendGeneric(kSonyHdrMark, kSonySpace, kSonyOneMark, kSonySpace, kSonyZeroMark,
                kSonySpace,
                0,  // No Footer mark.
                kSonyMinGap, kSonyRptLength, data, nbits, freq, true, repeat, 33);
}
/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/


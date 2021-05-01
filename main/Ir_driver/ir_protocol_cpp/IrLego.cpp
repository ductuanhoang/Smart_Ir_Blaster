/*
 * IrLego.cpp
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
#include "IrLego.h"
#include "IRutils.h"
/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/
// Constants
const uint16_t kLegoPfBitMark = 158;
const uint16_t kLegoPfHdrSpace = 1026;
const uint16_t kLegoPfZeroSpace = 263;
const uint16_t kLegoPfOneSpace = 553;
const uint32_t kLegoPfMinCommandLength = 16000;  // 16ms
/***********************************************************************************************************************
 * Private global variables and functions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global variables and functions (to be accessed by other files)
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Imported global variables and functions (from other files)
 ***********************************************************************************************************************/
//using std::max;
/***********************************************************************************************************************
 * Function Name: 
 * Description  :
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
IrLego::IrLego(uint32_t frequency, uint16_t dutycycle): _ir_legopf_send(frequency, dutycycle) {
	// TODO Auto-generated constructor stub

}

IrLego::~IrLego() {
	// TODO Auto-generated destructor stub
}

void IrLego::sendLegoPf(const uint64_t data, const uint16_t nbits,
                        const uint16_t repeat) {
  uint8_t channelid = ((data >> (nbits - 4)) & 0b11) + 1;
  APP_LOGD("channelid = %d", channelid);
  if (repeat) {
    // We are in repeat mode.
    // Spec says a pause before transmittion.
    if (channelid < 4) _ir_legopf_send.space((4 - channelid) * kLegoPfMinCommandLength);
    // Spec says there are a minimum of 5 message repeats.
    for (uint16_t r = 0; r < std::max(repeat, (uint16_t)5); r++)
    {
      // Lego has a special repeat mode which repeats a message with varying
      // start to start times.
    	_ir_legopf_send.sendGeneric(kLegoPfBitMark, kLegoPfHdrSpace,
                  kLegoPfBitMark, kLegoPfOneSpace,
                  kLegoPfBitMark, kLegoPfZeroSpace,
                  kLegoPfBitMark, kLegoPfHdrSpace,
                  ((r < 2) ? 5 : (6 + 2 * channelid)) * kLegoPfMinCommandLength,
                  data, nbits, 38000, true, 0, 50);
    }
  } else {  // No repeat, just a simple message.
	  printf("go here \r\n");
	  _ir_legopf_send.sendGeneric(kLegoPfBitMark, kLegoPfHdrSpace,
                kLegoPfBitMark, kLegoPfOneSpace,
                kLegoPfBitMark, kLegoPfZeroSpace,
                kLegoPfBitMark, kLegoPfHdrSpace,
                kLegoPfMinCommandLength * 5,
                data, nbits, 38000, true, 0, 50);
  }
}

/***********************************************************************************************************************
 * Static Functions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/


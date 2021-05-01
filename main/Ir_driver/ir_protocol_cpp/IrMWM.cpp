/*
 * IrMWM.cpp
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
#include "IrMWM.h"

/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/
// Constants
const uint16_t kMWMMinSamples = 6;  // Msgs are >=3 bytes, bytes have >=2
                                    // samples
const uint16_t kMWMTick = 417;
const uint32_t kMWMMinGap = 30000;  // Typical observed delay b/w commands
const uint8_t kMWMTolerance = 0;    // Percentage error margin.
const uint16_t kMWMExcess = 0;      // See kMarkExcess.
const uint16_t kMWMDelta = 150;     // Use instead of Excess and Tolerance.
const uint8_t kMWMMaxWidth = 9;     // Maximum number of successive bits at a
                                    // single level - worst case
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

Ir_MWM::Ir_MWM(uint32_t frequency, uint16_t dutycycle): _ir_mwm_send(frequency, dutycycle) {
	// TODO Auto-generated constructor stub

}

Ir_MWM::~Ir_MWM() {
	// TODO Auto-generated destructor stub
}

/// Send a MWM packet/message.
/// Status: Implemented.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @note This protocol is 2400 bps serial, 1 start bit (mark),
///   1 stop bit (space), no parity
void Ir_MWM::sendMWM(const uint8_t data[], const uint16_t nbytes,
                     const uint16_t repeat) {
  if (nbytes < 3) return;  // Shortest possible message is 3 bytes

  // Set 38kHz IR carrier frequency & a 1/4 (25%) duty cycle.
  // NOTE: duty cycle is not confirmed. Just guessing based on RC5/6 protocols.

  for (uint16_t r = 0; r <= repeat; r++) {
    // Data
    for (uint16_t i = 0; i < nbytes; i++) {
      uint8_t byte = data[i];
      // Start bit
      _ir_mwm_send.mark(kMWMTick);

      // LSB first, space=1
      for (uint8_t mask = 0x1; mask; mask <<= 1) {
        if (byte & mask) {  // 1
        	_ir_mwm_send.space(kMWMTick);
        } else {  // 0
        	_ir_mwm_send.mark(kMWMTick);
        }
      }
      // Stop bit
      _ir_mwm_send.space(kMWMTick);
    }
    // Footer
    _ir_mwm_send.space(kMWMMinGap);
  }
}
/***********************************************************************************************************************
 * Static Functions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/


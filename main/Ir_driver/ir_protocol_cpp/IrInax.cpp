/*
 * IrInax.cpp
 *
 *  Created on: Jan 17, 2021
 *      Author: ductu
 */

#include "IrInax.h"



/***********************************************************************************************************************
 * Pragma directive
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes <System Includes>
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/
// Constants
const uint16_t kInaxTick = 500;
const uint16_t kInaxHdrMark = 9000;
const uint16_t kInaxHdrSpace = 4500;
const uint16_t kInaxBitMark = 560;
const uint16_t kInaxOneSpace = 1675;
const uint16_t kInaxZeroSpace = kInaxBitMark;
const uint16_t kInaxMinGap = 40000;
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

Ir_Inax::Ir_Inax(uint32_t frequency, uint16_t dutycycle):_ir_inax_send(frequency, dutycycle) {
	// TODO Auto-generated constructor stub

}

Ir_Inax::~Ir_Inax() {
	// TODO Auto-generated destructor stub
}
/// Send a Inax Toilet formatted message.
/// Status: STABLE / Working.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/706
void Ir_Inax::sendInax(const uint64_t data, const uint16_t nbits, const uint16_t repeat) {
    _ir_inax_send.sendGeneric(kInaxHdrMark, kInaxHdrSpace,
                kInaxBitMark, kInaxOneSpace,
                kInaxBitMark, kInaxZeroSpace,
                kInaxBitMark, kInaxMinGap,
                data, nbits, 38, true, repeat, 50);
}
/***********************************************************************************************************************
 * Static Functions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/


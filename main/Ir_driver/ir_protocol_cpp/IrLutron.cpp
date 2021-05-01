/*
 * IrLutron.cpp
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
#include "IrLutron.h"

/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/
// Constants
const uint16_t kLutronTick = 2288;
const uint32_t kLutronGap = 150000;  // Completely made up value.
const uint16_t kLutronDelta = 400;   // +/- 300 usecs.
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
Ir_Lutron::Ir_Lutron(uint32_t frequency, uint16_t dutycycle): _ir_lutron_send(frequency, dutycycle) {
	// TODO Auto-generated constructor stub

}

Ir_Lutron::~Ir_Lutron() {
	// TODO Auto-generated destructor stub
}


void Ir_Lutron::sendLutron(uint64_t data, uint16_t nbits, uint16_t repeat) {
////    enableIROut(40000, 40);  // 40Khz & 40% dutycycle.
    for (uint16_t r = 0; r <= repeat; r++) {
//    	_ir_lutron_send.mark(kLutronTick);  // 1st bit is always '1'.
    	_ir_lutron_send.mark_space(kLutronTick, 0);
//        // Send the supplied data in MSB First order.
//        for (uint64_t mask = 1ULL << (nbits - 1); mask; mask >>= 1)
//            if (data & mask)
//                _ir_lutron_send.mark(kLutronTick);// Send a 1
//            else
//                _ir_lutron_send.space(kLutronTick);// Send a 0
//        _ir_lutron_send.mark_space(0, 0);

    	uint64_t mask = 0x01;
		mask <<= nbits - 1 - 1;
		for (int j = 0; j < nbits - 1; j++)
		{
			if (data & mask)
			{
				_ir_lutron_send.mark(kLutronTick);// Send a 1
			}
			else
			{
				_ir_lutron_send.space(kLutronTick);// Send a 0

			}
			mask >>= 1;
		}
    }
}
/***********************************************************************************************************************
 * Static Functions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/


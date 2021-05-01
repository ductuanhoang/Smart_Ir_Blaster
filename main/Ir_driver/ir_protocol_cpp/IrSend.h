/*
 * IrSend.h
 *
 *  Created on: Jan 11, 2021
 *      Author: ductu
 */

#ifndef MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRSEND_H_
#define MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRSEND_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include "../../Common.h"
#include "../ir_base_config.h"

	class Ir_Send
	{
	public:
		Ir_Send(uint32_t frequency, uint16_t dutycycle);
		virtual ~Ir_Send();
		void sendGeneric(const uint16_t headermark, const uint32_t headerspace,
						 const uint16_t onemark, const uint32_t onespace,
						 const uint16_t zeromark, const uint32_t zerospace,
						 const uint16_t footermark, const uint32_t gap,
						 const uint64_t data, const uint16_t nbits,
						 const uint16_t frequency, const bool MSBfirst,
						 const uint16_t repeat, const uint8_t dutycycle);
		void sendGeneric(const uint16_t headermark, const uint32_t headerspace,
						 const uint16_t onemark, const uint32_t onespace,
						 const uint16_t zeromark, const uint32_t zerospace,
						 const uint16_t footermark, const uint32_t gap,
						 const uint32_t mesgtime, const uint64_t data,
						 const uint16_t nbits, const uint16_t frequency,
						 const bool MSBfirst, const uint16_t repeat,
						 const uint8_t dutycycle);
		void sendGeneric(const uint16_t headermark, const uint32_t headerspace,
						 const uint16_t onemark, const uint32_t onespace,
						 const uint16_t zeromark, const uint32_t zerospace,
						 const uint16_t footermark, const uint32_t gap,
						 const uint8_t *dataptr, const uint16_t nbytes,
						 const uint16_t frequency, const bool MSBfirst,
						 const uint16_t repeat, const uint8_t dutycycle);

		void sendGeneric(uint16_t *data_raw, uint16_t length);

		void sendManchester(const uint16_t headermark, const uint32_t headerspace,
							const uint16_t half_period, const uint16_t footermark,
							const uint32_t gap, const uint64_t data,
							const uint16_t nbits, const uint16_t frequency = 38000,
							const bool MSBfirst = true,
							const uint16_t repeat = kNoRepeat,
							const uint8_t dutycycle = 50,
							const bool GEThomas = true);
		void mark_space(uint32_t mask, uint32_t space);
		void mark(uint16_t mask);
		void space(uint32_t space);

        void sendData(uint16_t onemark, uint32_t onespace, uint16_t zeromark,
                uint32_t zerospace, uint64_t data, uint16_t nbits,
                bool MSBfirst = true);
	private:

		void sendRaw(uint16_t *data_raw, uint16_t length);
		void sendManchesterData(const uint16_t half_period, const uint64_t data,
								const uint16_t nbits, const bool MSBfirst = true,
								const bool GEThomas = true);
		uint32_t _frequency;
		uint16_t _dutycycle;
	};
	/****************************************************************************/
	/***        Include files                                                 ***/
	/****************************************************************************/
	/// Enumerators and Structures for the Common A/C API.
	namespace stdAc {
	  /// Common A/C settings for A/C operating modes.
	  enum class opmode_t {
	    kOff  = -1,
	    kAuto =  0,
	    kCool =  1,
	    kHeat =  2,
	    kDry  =  3,
	    kFan  =  4,
	    // Add new entries before this one, and update it to point to the last entry
	    kLastOpmodeEnum = kFan,
	  };

	  /// Common A/C settings for Fan Speeds.
	  enum class fanspeed_t {
	    kAuto =   0,
	    kMin =    1,
	    kLow =    2,
	    kMedium = 3,
	    kHigh =   4,
	    kMax =    5,
	    // Add new entries before this one, and update it to point to the last entry
	    kLastFanspeedEnum = kMax,
	  };

	  /// Common A/C settings for Vertical Swing.
	  enum class swingv_t {
	    kOff =    -1,
	    kAuto =    0,
	    kHighest = 1,
	    kHigh =    2,
	    kMiddle =  3,
	    kLow =     4,
	    kLowest =  5,
	    // Add new entries before this one, and update it to point to the last entry
	    kLastSwingvEnum = kLowest,
	  };

	  /// Common A/C settings for Horizontal Swing.
	  enum class swingh_t {
	    kOff =     -1,
	    kAuto =     0,  // a.k.a. On.
	    kLeftMax =  1,
	    kLeft =     2,
	    kMiddle =   3,
	    kRight =    4,
	    kRightMax = 5,
	    kWide =     6,  // a.k.a. left & right at the same time.
	    // Add new entries before this one, and update it to point to the last entry
	    kLastSwinghEnum = kWide,
	  };
	}
	/****************************************************************************/
	/***        Macro Definitions                                             ***/
	/****************************************************************************/

	/****************************************************************************/
	/***        Type Definitions                                              ***/
	/****************************************************************************/

	/****************************************************************************/
	/***         Exported global functions                                     ***/
	/****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRSEND_H_ */

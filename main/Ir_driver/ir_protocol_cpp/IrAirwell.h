/*
 * IrAirwell.h
 *
 *  Created on: Jan 13, 2021
 *      Author: ductu
 */

#ifndef MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRAIRWELL_H_
#define MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRAIRWELL_H_

#include "IrSend.h"
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
/// Native representation of a Airwell A/C message.
union AirwellProtocol
{
	uint64_t raw; // The state of the IR remote in native IR code form.
	struct
	{
		uint64_t : 19;
		uint64_t Temp : 4;
		uint64_t : 5;
		uint64_t Fan : 2;
		uint64_t Mode : 3;
		uint64_t PowerToggle : 1;
		uint64_t : 0;
	};
};
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
// Constants
const uint64_t kAirwellKnownGoodState = 0x140500002; // Mode Fan, Speed 1, 25C
// Temperature
const uint8_t kAirwellMinTemp = 16; // Celsius
const uint8_t kAirwellMaxTemp = 30; // Celsius
// Fan
const uint8_t kAirwellFanLow = 0;	 // 0b00
const uint8_t kAirwellFanMedium = 1; // 0b01
const uint8_t kAirwellFanHigh = 2;	 // 0b10
const uint8_t kAirwellFanAuto = 3;	 // 0b11
// Modes
const uint8_t kAirwellCool = 1; // 0b001
const uint8_t kAirwellHeat = 2; // 0b010
const uint8_t kAirwellAuto = 3; // 0b011
const uint8_t kAirwellDry = 4;	// 0b100
const uint8_t kAirwellFan = 5;	// 0b101
/****************************************************************************/
/***         Exported global functions                                     ***/
/****************************************************************************/
class Ir_Airwell
{
public:
	Ir_Airwell(uint32_t frequency, uint16_t dutycycle);
	virtual ~Ir_Airwell();
	void sendAirwell(const uint16_t repeat = kAirwellMinRepeats);
	void setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp);
private:
    uint8_t convertFan(const stdAc::fanspeed_t speed);
    uint8_t convertMode(const stdAc::opmode_t mode);
	void send(uint64_t data, uint16_t nbits = kAirwellBits,
			  uint16_t repeat = kAirwellMinRepeats);

	void stateReset();
	void setPowerToggle(const bool on);
	bool getPowerToggle() const;
	void setTemp(const uint8_t temp);
	uint8_t getTemp() const;
	void setFan(const uint8_t speed);
	uint8_t getFan() const;
	void setMode(const uint8_t mode);
	uint8_t getMode() const;
	uint64_t getRaw() const;
	void setRaw(const uint64_t state);
	AirwellProtocol _AirwellProtocol;
	Ir_Send _ir_airwell_send;
};
#endif /* MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRAIRWELL_H_ */

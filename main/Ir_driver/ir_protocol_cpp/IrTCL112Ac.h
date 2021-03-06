/*
 * IrTCL112Ac.h
 *
 *  Created on: Mar 9, 2021
 *      Author: ductu
 */

#ifndef MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRTCL112AC_H_
#define MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRTCL112AC_H_


/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "IrSend.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
// Constants
const uint16_t kTcl112AcHdrMark = 3000;
const uint16_t kTcl112AcHdrSpace = 1650;
const uint16_t kTcl112AcBitMark = 500;
const uint16_t kTcl112AcOneSpace = 1050;
const uint16_t kTcl112AcZeroSpace = 325;
const uint32_t kTcl112AcGap = kDefaultMessageGap;  // Just a guess.
// Total tolerance percentage to use for matching the header mark.
const uint8_t kTcl112AcHdrMarkTolerance = 6;
const uint8_t kTcl112AcTolerance = 5;  // Extra Percentage for the rest.

const uint8_t kTcl112AcHeat = 1;
const uint8_t kTcl112AcDry =  2;
const uint8_t kTcl112AcCool = 3;
const uint8_t kTcl112AcFan =  7;
const uint8_t kTcl112AcAuto = 8;
const uint8_t kTcl112AcModeSize = 4;  // Nr. of Bits

const uint8_t kTcl112AcFanSize = 3;  // Nr. of Bits. Mask = 0b00000111
const uint8_t kTcl112AcFanAuto = 0b000;
const uint8_t kTcl112AcFanLow  = 0b010;
const uint8_t kTcl112AcFanMed  = 0b011;
const uint8_t kTcl112AcFanHigh = 0b101;

const uint8_t kTcl112AcHalfDegreeOffset = 5;
const float   kTcl112AcTempMax    = 31.0;
const float   kTcl112AcTempMin    = 16.0;

const uint8_t kTcl112AcPowerOffset = 2;
const uint8_t kTcl112AcBitEconoOffset = 7;
const uint8_t kTcl112AcBitLightOffset = 6;
const uint8_t kTcl112AcBitHealthOffset = 4;
const uint8_t kTcl112AcBitSwingHOffset = 3;
const uint8_t kTcl112AcSwingVOffset = 3;  // Mask 0b00111000
const uint8_t kTcl112AcSwingVSize = 3;  // Nr. of bits.
const uint8_t kTcl112AcSwingVOn =    0b111;
const uint8_t kTcl112AcSwingVOff =   0b000;
const uint8_t kTcl112AcBitTurboOffset = 6;
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***         Exported global functions                                     ***/
/****************************************************************************/
class Ir_TCL_112Ac {
public:
	Ir_TCL_112Ac(uint32_t frequency, uint16_t dutycycle);
	virtual ~Ir_TCL_112Ac();
	void sendTCL_112Ac(const uint16_t repeat = kTcl112AcDefaultRepeat);
	void setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp);
public:
	void send(const unsigned char data[],
            const uint16_t nbytes = kTcl112AcStateLength,
            const uint16_t repeat = kTcl112AcDefaultRepeat);
	void stateReset(void);
	uint8_t* getRaw(void);
	void setRaw(const uint8_t new_code[],
			 const uint16_t length = kTcl112AcStateLength);
	void on(void);
	void off(void);
	void setPower(const bool on);
	bool getPower(void);
	void setTemp(const float celsius);  // Celsius in 0.5 increments
	float getTemp(void);
	void setMode(const uint8_t mode);
	uint8_t getMode(void);
	static uint8_t calcChecksum(uint8_t state[],
							 const uint16_t length = kTcl112AcStateLength);
	static bool validChecksum(uint8_t state[],
						   const uint16_t length = kTcl112AcStateLength);
	void setFan(const uint8_t speed);
	uint8_t getFan(void);
	void setEcono(const bool on);
	bool getEcono(void);
	void setHealth(const bool on);
	bool getHealth(void);
	void setLight(const bool on);
	bool getLight(void);
	void setSwingHorizontal(const bool on);
	bool getSwingHorizontal(void);
	void setSwingVertical(const bool on);
	bool getSwingVertical(void);
	void setTurbo(const bool on);
	bool getTurbo(void);
	uint8_t convertMode(const stdAc::opmode_t mode);
	uint8_t convertFan(const stdAc::fanspeed_t speed);

	uint8_t remote_state[kTcl112AcStateLength];  ///< The State in IR code form.
	void checksum(const uint16_t length = kTcl112AcStateLength);

private:
 	 Ir_Send _ir_remote;
};
#endif /* MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRTCL112AC_H_ */

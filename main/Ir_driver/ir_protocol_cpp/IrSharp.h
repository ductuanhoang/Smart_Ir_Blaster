/*
 * IrSharp.h
 *
 *  Created on: Jan 18, 2021
 *      Author: ductu
 */

#ifndef MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRSHARP_H_
#define MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRSHARP_H_


/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "IrSend.h"
#include "IRutils.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
// Constants
const uint16_t kSharpAcHdrMark = 3800;
const uint16_t kSharpAcHdrSpace = 1900;
const uint16_t kSharpAcBitMark = 470;
const uint16_t kSharpAcZeroSpace = 500;
const uint16_t kSharpAcOneSpace = 1400;
const uint32_t kSharpAcGap = kDefaultMessageGap;

// Byte[4]
const uint8_t kSharpAcModelBit = 4;  // Mask 0b000x0000
const uint8_t kSharpAcByteTemp = 4;
const uint8_t kSharpAcMinTemp = 15;  // Celsius
const uint8_t kSharpAcMaxTemp = 30;  // Celsius
// Byte[5]
const uint8_t kSharpAcBytePowerSpecial = 5;
const uint8_t kSharpAcPowerSetSpecialOffset = kHighNibble;  // 0bxxxx0000
const uint8_t kSharpAcPowerSpecialSize = kNibbleSize;       // 0bxxxx0000
const uint8_t kSharpAcPowerUnknown = 0;                     // 0b0000
const uint8_t kSharpAcPowerOnFromOff = 1;                   // 0b0001
const uint8_t kSharpAcPowerOff = 2;                         // 0b0010
const uint8_t kSharpAcPowerOn = 3;                          // 0b0011 (Normal)
const uint8_t kSharpAcPowerSetSpecialOn = 6;                // 0b0110
const uint8_t kSharpAcPowerSetSpecialOff = 7;               // 0b0111
const uint8_t kSharpAcPowerTimerSetting = 8;                // 0b1000
// Byte[6]
const uint8_t kSharpAcByteMode = 6;
const uint8_t kSharpAcModeSize = 2;        // Mask 0b000000xx;
const uint8_t kSharpAcAuto =                             0b00;  // A907 only
const uint8_t kSharpAcFan =                              0b00;  // A705 only
const uint8_t kSharpAcDry =                              0b11;
const uint8_t kSharpAcCool =                             0b10;
const uint8_t kSharpAcHeat =                             0b01;  // A907 only
const uint8_t kSharpAcByteClean = kSharpAcByteMode;
const uint8_t kSharpAcBitCleanOffset = 3;  // Mask 0b0000x000
const uint8_t kSharpAcByteFan = kSharpAcByteMode;
const uint8_t kSharpAcFanOffset = 4;       // Mask 0b0xxx0000
const uint8_t kSharpAcFanSize = 3;  // Nr. of Bits
const uint8_t kSharpAcFanAuto =                     0b010;  // 2
const uint8_t kSharpAcFanMin =                      0b100;  // 4 (FAN1)
const uint8_t kSharpAcFanMed =                      0b011;  // 3 (FAN2)
const uint8_t kSharpAcFanA705Low =                  0b011;  // 3
const uint8_t kSharpAcFanHigh =                     0b101;  // 5 (FAN3)
const uint8_t kSharpAcFanA705Med =                  0b101;  // 5
const uint8_t kSharpAcFanMax =                      0b111;  // 7 (FAN4)
// Byte[7]
const uint8_t kSharpAcByteTimer = 7;
const uint8_t kSharpAcTimerIncrement = 30;  // Mins
const uint8_t kSharpAcTimerHoursOffset = kLowNibble;
const uint8_t kSharpAcTimerHoursSize = kNibbleSize;   // Mask 0b0000xxxx
const uint8_t kSharpAcTimerHoursOff =                             0b0000;
const uint8_t kSharpAcTimerHoursMax =                             0b1100;  // 12
const uint8_t kSharpAcBitTimerType = 6;               // Mask 0b0x000000
const uint8_t kSharpAcOffTimerType =                           0b0;
const uint8_t kSharpAcOnTimerType =                            0b1;
const uint8_t kSharpAcBitTimerEnabled = 7;            // Mask 0bx0000000
// Byte[8]
const uint8_t kSharpAcByteSwing = 8;
const uint8_t kSharpAcSwingOffset = 0;
const uint8_t kSharpAcSwingSize = 3;  // Mask 0b00000xxx
const uint8_t kSharpAcSwingToggle =                0b111;
const uint8_t kSharpAcSwingNoToggle =              0b000;
// Byte[10]
const uint8_t kSharpAcByteSpecial = 10;  // Mask 0bxxxxxxxx
const uint8_t kSharpAcSpecialPower =              0x00;
const uint8_t kSharpAcSpecialTurbo =              0x01;
const uint8_t kSharpAcSpecialTempEcono =          0x04;
const uint8_t kSharpAcSpecialFan =                0x05;
const uint8_t kSharpAcSpecialSwing =              0x06;
const uint8_t kSharpAcSpecialTimer =              0xC0;
const uint8_t kSharpAcSpecialTimerHalfHour =      0xDE;
// Byte[11]
const uint8_t kSharpAcByteIon = 11;
const uint8_t kSharpAcBitIonOffset = 2;  // Mask 0b00000x00
// Byte[12] (Checksum)
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
/// Sharp A/C model numbers
enum sharp_ac_remote_model_t {
  A907 = 1,  // 802 too.
  A705 = 2,
};
/****************************************************************************/
/***         Exported global functions                                     ***/
/****************************************************************************/
class Ir_Sharp {
public:
	Ir_Sharp(uint32_t frequency, uint16_t dutycycle);
	virtual ~Ir_Sharp();

	void sendSharpAC(const uint16_t repeat = kSharpAcDefaultRepeat);

	void sendSharpRaw(const uint64_t data, const uint16_t nbits,
	                          const uint16_t repeat);
	void setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp);
private:
	uint8_t convertFan(const stdAc::fanspeed_t speed);
	uint8_t convertMode(const stdAc::opmode_t mode);
	void send(const unsigned char data[],
            const uint16_t nbytes = kSharpAcStateLength,
            const uint16_t repeat = kSharpAcDefaultRepeat);
	void setModel(const sharp_ac_remote_model_t model);
	sharp_ac_remote_model_t getModel(const bool raw = false);
	void on(void);
	void off(void);
	void setPower(const bool on, const bool prev_on = true);
	bool getPower(void);
	bool isPowerSpecial(void);
	void setTemp(const uint8_t temp, const bool save = true);
	uint8_t getTemp(void);
	void setFan(const uint8_t fan, const bool save = true);
	uint8_t getFan(void);
	void setMode(const uint8_t mode, const bool save = true);
	uint8_t getMode(void);
	void setSpecial(const uint8_t mode);
	uint8_t getSpecial(void);
	bool getTurbo(void);
	void setTurbo(const bool on);
	bool getSwingToggle(void);
	void setSwingToggle(const bool on);
	bool getIon(void);
	void setIon(const bool on);
	bool getEconoToggle(void);
	void setEconoToggle(const bool on);
	bool getLightToggle(void);
	void setLightToggle(const bool on);
	uint16_t getTimerTime(void);
	bool getTimerEnabled(void);
	bool getTimerType(void);
	void setTimer(bool enable, bool timer_type, uint16_t mins);
	bool getClean(void);
	void setClean(const bool on);
	uint8_t* getRaw(void);
	void setRaw(const uint8_t new_code[],
			  const uint16_t length = kSharpAcStateLength);
	static bool validChecksum(uint8_t state[],
							const uint16_t length = kSharpAcStateLength);

	Ir_Send _ir_sharp_send;
	uint8_t remote[kSharpAcStateLength];  ///< State of the remote in IR code form
	uint8_t _temp;  ///< Saved copy of the desired temp.
	uint8_t _mode;  ///< Saved copy of the desired mode.
	uint8_t _fan;  ///< Saved copy of the desired fan speed.
	sharp_ac_remote_model_t _model;  ///< Saved copy of the model.
	void stateReset(void);
	void checksum(void);
	static uint8_t calcChecksum(uint8_t state[],
							  const uint16_t length = kSharpAcStateLength);
	void setPowerSpecial(const uint8_t value);
	uint8_t getPowerSpecial(void);
	void clearPowerSpecial(void);
	bool _getEconoToggle(void);
	void _setEconoToggle(const bool on);
};
#endif /* MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRSHARP_H_ */

/*
 * IrPanasonic.h
 *
 *  Created on: Jan 12, 2021
 *      Author: ductu
 */

#ifndef MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRPANASONIC_H_
#define MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRPANASONIC_H_

#include "IrSend.h"
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
// Constants
const uint16_t kPanasonicFreq = 36700;
const uint16_t kPanasonicAcExcess = 0;
// Much higher than usual. See issue #540.
const uint16_t kPanasonicAcTolerance = 40;

const uint8_t kPanasonicAcAuto = 0;  // 0b000
const uint8_t kPanasonicAcDry = 2;   // 0b010
const uint8_t kPanasonicAcCool = 3;  // 0b011
const uint8_t kPanasonicAcHeat = 4;  // 0b010
const uint8_t kPanasonicAcFan = 6;   // 0b110
const uint8_t kPanasonicAcFanMin = 0;
const uint8_t kPanasonicAcFanMed = 2;
const uint8_t kPanasonicAcFanMax = 4;
const uint8_t kPanasonicAcFanAuto = 7;
const uint8_t kPanasonicAcFanDelta = 3;
const uint8_t kPanasonicAcPowerOffset = 0;
const uint8_t kPanasonicAcTempOffset = 1;  // Bits
const uint8_t kPanasonicAcTempSize = 5;  // Bits
const uint8_t kPanasonicAcMinTemp = 16;      // Celsius
const uint8_t kPanasonicAcMaxTemp = 30;      // Celsius
const uint8_t kPanasonicAcFanModeTemp = 27;  // Celsius
const uint8_t kPanasonicAcQuietOffset = 0;
const uint8_t kPanasonicAcPowerfulOffset = 5;   // 0b100000
// CKP & RKR models have Powerful and Quiet bits swapped.
const uint8_t kPanasonicAcQuietCkpOffset = kPanasonicAcPowerfulOffset;
const uint8_t kPanasonicAcPowerfulCkpOffset = kPanasonicAcQuietOffset;
const uint8_t kPanasonicAcSwingVHighest = 0x1;  // 0b0001
const uint8_t kPanasonicAcSwingVHigh = 0x2;     // 0b0010
const uint8_t kPanasonicAcSwingVMiddle = 0x3;   // 0b0011
const uint8_t kPanasonicAcSwingVLow = 0x4;      // 0b0100
const uint8_t kPanasonicAcSwingVLowest = 0x5;   // 0b0101
const uint8_t kPanasonicAcSwingVAuto = 0xF;     // 0b1111

const uint8_t kPanasonicAcSwingHMiddle = 0x6;     // 0b0110
const uint8_t kPanasonicAcSwingHFullLeft = 0x9;   // 0b1001
const uint8_t kPanasonicAcSwingHLeft = 0xA;       // 0b1010
const uint8_t kPanasonicAcSwingHRight = 0xB;      // 0b1011
const uint8_t kPanasonicAcSwingHFullRight = 0xC;  // 0b1100
const uint8_t kPanasonicAcSwingHAuto = 0xD;       // 0b1101
const uint8_t kPanasonicAcChecksumInit = 0xF4;
const uint8_t kPanasonicAcOnTimerOffset = 1;
const uint8_t kPanasonicAcOffTimerOffset = 2;
const uint8_t kPanasonicAcTimeSize = 11;  // Bits
const uint8_t kPanasonicAcTimeOverflowSize = 3;  // Bits
const uint16_t kPanasonicAcTimeMax = 23 * 60 + 59;  // Mins since midnight.
const uint16_t kPanasonicAcTimeSpecial = 0x600;

const uint8_t kPanasonicAcIonFilterByte = 22;  // Byte
const uint8_t kPanasonicAcIonFilterOffset = 0;  // Bit

const uint8_t kPanasonicKnownGoodState[kPanasonicAcStateLength] = {
        0x02, 0x20, 0xE0, 0x04, 0x00, 0x00, 0x00, 0x06, 0x02,
        0x20, 0xE0, 0x04, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00,
        0x00, 0x0E, 0xE0, 0x00, 0x00, 0x81, 0x00, 0x00, 0x00};


/// Panasonic A/C model numbers
enum panasonic_ac_remote_model_t {
  kPanasonicUnknown = 0,
  kPanasonicLke = 1,
  kPanasonicNke = 2,
  kPanasonicDke = 3,  // PKR too.
  kPanasonicJke = 4,
  kPanasonicCkp = 5,
  kPanasonicRkr = 6,
};

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***         Exported global functions                                     ***/
/****************************************************************************/
class Ir_Panasonic {
public:
	Ir_Panasonic(uint32_t frequency, uint16_t dutycycle);
	virtual ~Ir_Panasonic();
    void sendPanasonic64(const uint64_t data,
                    const uint16_t nbits = kPanasonicBits,
                    const uint16_t repeat = kNoRepeat);

    void sendPanasonicAC32(const uint64_t data,
    		const uint16_t nbits = kPanasonicAc32Bits,
			const uint16_t repeat = kPanasonicAcDefaultRepeat);

    void sendPanasonic(const uint16_t address, const uint32_t data,
                  const uint16_t nbits = kPanasonicBits,
                  const uint16_t repeat = kNoRepeat);
	void sendPanasonicAC(const uint16_t repeat = kPanasonicAcDefaultRepeat);

    void setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp);
private:
    uint8_t convertFan(const stdAc::fanspeed_t speed);
    uint8_t convertMode(const stdAc::opmode_t mode);

    void send(const unsigned char data[],
               const uint16_t nbytes = kPanasonicAcStateLength,
               const uint16_t repeat = kPanasonicAcDefaultRepeat);

    void stateReset(void);
    void on(void);
    void off(void);
    void setPower(const bool on);
    bool getPower(void);
    void setTemp(const uint8_t temp, const bool remember = true);
    uint8_t getTemp(void);
    void setFan(const uint8_t fan);
    uint8_t getFan(void);
    void setMode(const uint8_t desired);
    uint8_t getMode(void);
    void setRaw(const uint8_t state[]);
    uint8_t *getRaw(void);
    static bool validChecksum(const uint8_t *state,
                              const uint16_t length = kPanasonicAcStateLength);
    static uint8_t calcChecksum(const uint8_t *state,
                                const uint16_t length = kPanasonicAcStateLength);
    void setQuiet(const bool on);
    bool getQuiet(void);
    void setPowerful(const bool on);
    bool getPowerful(void);
    void setIon(const bool on);
    bool getIon(void);
    void setModel(const panasonic_ac_remote_model_t model);
    panasonic_ac_remote_model_t getModel(void);
    void setSwingVertical(const uint8_t desired_elevation);
    uint8_t getSwingVertical(void);
    void setSwingHorizontal(const uint8_t desired_direction);
    uint8_t getSwingHorizontal(void);
    static uint16_t encodeTime(const uint8_t hours, const uint8_t mins);
    uint16_t getClock(void);
    void setClock(const uint16_t mins_since_midnight);
    uint16_t getOnTimer(void);
    void setOnTimer(const uint16_t mins_since_midnight, const bool enable = true);
    void cancelOnTimer(void);
    bool isOnTimerEnabled(void);
    uint16_t getOffTimer(void);
    void setOffTimer(const uint16_t mins_since_midnight,
                     const bool enable = true);
    void cancelOffTimer(void);
    bool isOffTimerEnabled(void);

    uint8_t remote_state[kPanasonicAcStateLength];  ///< The state in code form.
    uint8_t _swingh;
    uint8_t _temp;
    void fixChecksum(const uint16_t length = kPanasonicAcStateLength);
    static uint16_t _getTime(const uint8_t ptr[]);
    static void _setTime(uint8_t * const ptr, const uint16_t mins_since_midnight,
                         const bool round_down);

    Ir_Send _ir_panasonic;
};


#endif /* MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRPANASONIC_H_ */

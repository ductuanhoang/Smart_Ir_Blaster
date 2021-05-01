/*
 * IrToshiba.h
 *
 *  Created on: Jan 16, 2021
 *      Author: ductu
 */

#ifndef MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRTOSHIBA_H_
#define MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRTOSHIBA_H_


/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "IrSend.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
// Constants
// Byte[0] - 0xF2
// Byte[1] - 0x0D (inverted previous byte's value)
// Byte[2] - The expected payload length (in bytes) past the Byte[4].
const uint8_t kToshibaAcLengthByte = 2;  ///< Byte pos of the "length" attribute
const uint8_t kToshibaAcMinLength = 6;  ///< Min Nr. of bytes in a message.
///< Known lengths are:
///<   1 (56 bit message)
///<   3 (72 bit message)
///<   4 (80 bit message)
// Byte[3] - The bit-inverted value of the "length" byte.
const uint16_t kToshibaAcInvertedLength = 4;  ///< Nr. of leading bytes in
///< inverted pairs.
// Byte[4]
const uint8_t kToshibaAcShortMsgBit = 5;  ///< Mask 0b00x00000
const uint8_t kToshibaAcLongMsgBit = 3;   ///< Mask 0b00001000
// Byte[5]
const uint8_t kToshibaAcSwingOffset = 0;     ///< Bit offset.
const uint8_t kToshibaAcSwingSize = 2;       ///< Mask 0b000000xx
const uint8_t kToshibaAcSwingStep = 0;       ///<            0b00
const uint8_t kToshibaAcSwingOn = 1;         ///<            0b01
const uint8_t kToshibaAcSwingOff = 2;        ///<            0b10

const uint8_t kToshibaAcTempOffset = 4;  ///< Bit offset.
const uint8_t kToshibaAcTempSize = 4;  ///< Mask 0bxxxx0000
const uint8_t kToshibaAcMinTemp = 17;  ///< 17C
const uint8_t kToshibaAcMaxTemp = 30;  ///< 30C
// Byte[6]
const uint8_t kToshibaAcModeOffset = 0;
const uint8_t kToshibaAcModeSize = 3;  // Mask 0b00000xxx
const uint8_t kToshibaAcAuto = 0;                // 0b000
const uint8_t kToshibaAcCool = 1;                // 0b001
const uint8_t kToshibaAcDry =  2;                // 0b010
const uint8_t kToshibaAcHeat = 3;                // 0b011
const uint8_t kToshibaAcFan =  4;                // 0b100
const uint8_t kToshibaAcOff =  7;                // 0b111
const uint8_t kToshibaAcFanOffset = 5;
const uint8_t kToshibaAcFanSize = 3;   // Mask 0bxxx00000
const uint8_t kToshibaAcFanAuto = 0;   //      0b000
const uint8_t kToshibaAcFanMin =  1;   //      0b001
const uint8_t kToshibaAcFanMed =  3;   //      0b011
const uint8_t kToshibaAcFanMax =  5;   //      0b101
// Byte[8] (Checksum for 72 bit messages, Eco/Turbo for long 80 bit messages)
const uint8_t kToshibaAcEcoTurboOffset = 0;
const uint8_t kToshibaAcEcoTurboSize = 2;  // Mask 0b000000xx
const uint8_t kToshibaAcTurboOn = 1;       //            0b01
const uint8_t kToshibaAcEconoOn = 3;       //            0b11
// Byte[last] - Checksum (xor)

// Legacy defines. (Deprecated)
#define TOSHIBA_AC_AUTO kToshibaAcAuto
#define TOSHIBA_AC_COOL kToshibaAcCool
#define TOSHIBA_AC_DRY kToshibaAcDry
#define TOSHIBA_AC_HEAT kToshibaAcHeat
#define TOSHIBA_AC_POWER kToshibaAcPower
#define TOSHIBA_AC_FAN_AUTO kToshibaAcFanAuto
#define TOSHIBA_AC_FAN_MAX kToshibaAcFanMax
#define TOSHIBA_AC_MIN_TEMP kToshibaAcMinTemp
#define TOSHIBA_AC_MAX_TEMP kToshibaAcMaxTemp

/****************************************************************************/
/***         Exported global functions                                     ***/
/****************************************************************************/
class Ir_Toshiba {
public:
	Ir_Toshiba(uint32_t frequency, uint16_t dutycycle);
	virtual ~Ir_Toshiba();
    void sendToshibaAC(const uint16_t repeat = kToshibaACMinRepeat);

    void setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp);
private:
    uint8_t convertFan(const stdAc::fanspeed_t speed);
    uint8_t convertMode(const stdAc::opmode_t mode);
    void stateReset(void);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
    void send(const uint8_t data[],
              const uint16_t nbytes = kToshibaACStateLength,
              const uint16_t repeat = kToshibaACMinRepeat);
    void on(void);
    void off(void);
    void setPower(const bool on);
    bool getPower(void);
    void setTemp(const uint8_t degrees);
    uint8_t getTemp(void);
    void setFan(const uint8_t speed);
    uint8_t getFan(void);
    void setTurbo(const bool on);
    bool getTurbo(void);
    void setEcono(const bool on);
    bool getEcono(void);
    void setMode(const uint8_t mode);
    uint8_t getMode(const bool raw = false);
    void setRaw(const uint8_t newState[]);
    uint8_t* getRaw(void);
    static uint16_t getInternalStateLength(const uint8_t state[],
                                           const uint16_t size);
    uint16_t getStateLength(void);
    static bool validChecksum(const uint8_t state[],
                              const uint16_t length = kToshibaACStateLength);
    uint8_t getSwing(const bool raw = true);
    void setSwing(const uint8_t setting);

    uint8_t remote_state[kToshibaACStateLengthLong];  ///< The state in code form.
    uint8_t backup[kToshibaACStateLengthLong];  ///< A backup copy of the state.
    uint8_t prev_mode;  ///< Store of the previously set mode.
    bool _send_swing;  ///< Flag indicating if we need to send a swing message.
    uint8_t _swing_mode;  ///< The saved swing state/mode/command.
    void checksum(const uint16_t length = kToshibaACStateLength);
    static uint8_t calcChecksum(const uint8_t state[],
                                const uint16_t length = kToshibaACStateLength);
    void setStateLength(const uint16_t size);
    void _backupState(void);
    void _restoreState(void);

    Ir_Send _ir_toshiba_send;
};
#endif /* MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRTOSHIBA_H_ */

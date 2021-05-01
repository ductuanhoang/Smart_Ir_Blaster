/*
 * IrLG.h
 *
 *  Created on: Jan 15, 2021
 *      Author: ductu
 */

#ifndef MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRLG_H_
#define MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRLG_H_
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "IrSend.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
/// Native representation of a LG A/C message.
union LGProtocol{
    uint32_t raw;  ///< The state of the IR remote in IR code form.
    struct {
        uint32_t Sum  :4;
        uint32_t Fan  :3;
        uint32_t      :1;
        uint32_t Temp :4;
        uint32_t Mode :3;
        uint32_t      :3;
        uint32_t Power:2;
        uint32_t Sign :8;
    };
};

const uint8_t kLgAcFanLowest = 0;  // 0b000
const uint8_t kLgAcFanLow = 1;     // 0b001
const uint8_t kLgAcFanMedium = 2;  // 0b010
const uint8_t kLgAcFanHigh = 4;    // 0b100
const uint8_t kLgAcFanAuto = 5;    // 0b101
const uint8_t kLgAcTempAdjust = 15;
const uint8_t kLgAcMinTemp = 16;  // Celsius
const uint8_t kLgAcMaxTemp = 30;  // Celsius
const uint8_t kLgAcCool = 0;  // 0b000
const uint8_t kLgAcDry = 1;   // 0b001
const uint8_t kLgAcFan = 2;   // 0b010
const uint8_t kLgAcAuto = 3;  // 0b011
const uint8_t kLgAcHeat = 4;  // 0b100
const uint8_t kLgAcPowerOff = 3;  // 0b11
const uint8_t kLgAcPowerOn = 0;   // 0b00
const uint8_t kLgAcSignature = 0x88;

const uint32_t kLgAcOffCommand = 0x88C0051;
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
/// LG A/C model numbers
enum lg_ac_remote_model_t {
    GE6711AR2853M = 1,  // (1) LG 28-bit Protocol (default)
    AKB75215403,        // (2) LG2 28-bit Protocol
};
/****************************************************************************/
/***         Exported global functions                                     ***/
/****************************************************************************/
class Ir_LG {
public:
	Ir_LG(uint32_t frequency, uint16_t dutycycle);
	virtual ~Ir_LG();
	void sendLGAC(const uint16_t repeat = kLgDefaultRepeat);
    void sendLG(uint64_t data, uint16_t nbits = kLgBits,
                uint16_t repeat = kNoRepeat);
    void sendLG2(uint64_t data, uint16_t nbits = kLgBits,
                 uint16_t repeat = kNoRepeat);
    void setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp);

private:
    uint8_t convertMode(const stdAc::opmode_t mode);
    uint8_t convertFan(const stdAc::fanspeed_t speed);

    void on(void);
    void off(void);
    void setPower(const bool on);
    bool getPower(void) const;
    void setTemp(const uint8_t degrees);
    uint8_t getTemp(void) const;
    void setFan(const uint8_t speed);
    uint8_t getFan(void) const;
    void setMode(const uint8_t mode);
    uint8_t getMode(void) const;
    uint32_t getRaw(void);
    void setRaw(const uint32_t new_code);

    void stateReset(void);
    static uint8_t calcChecksum(const uint32_t state);
    static bool validChecksum(const uint32_t state);
    bool isValidLgAc(void) const;
    void setModel(const lg_ac_remote_model_t model);
    lg_ac_remote_model_t getModel(void) const;
    LGProtocol _;
    decode_type_t _protocol;  ///< model

    uint8_t _temp;

    void checksum(void);
    void _setTemp(const uint8_t value);
    Ir_Send _ir_lg_send;
};
#endif /* MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRLG_H_ */

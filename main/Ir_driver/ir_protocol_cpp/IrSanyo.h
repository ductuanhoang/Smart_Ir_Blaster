/*
 * IrSanyo.h
 *
 *  Created on: Mar 6, 2021
 *      Author: ductu
 */

#ifndef MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRSANYO_H_
#define MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRSANYO_H_


/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "IrSend.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
// Constants

// Sanyo A/C
// Ref: https://docs.google.com/spreadsheets/d/1dYfLsnYvpjV-SgO8pdinpfuBIpSzm8Q1R5SabrLeskw/edit?usp=sharing
// Byte[0] - 0x6A (Fixed?)
// Byte[1] - Address + Temperature
const uint8_t kSanyoAcTempByte = 1;    ///< Index
const uint8_t kSanyoAcTempOffset = 0;  ///< Mask 0b000xxxxx
const uint8_t kSanyoAcTempSize = 5;    ///< Mask 0b000xxxxx
const uint8_t kSanyoAcTempMin = 16;    ///< Celsius
const uint8_t kSanyoAcTempMax = 30;    ///< Celsius
const uint8_t kSanyoAcTempDelta = 4;   ///< Celsius to Native Temp difference.
// Byte[2] - Ambient Temp + Sensor
const uint8_t kSanyoAcSensorByte = 2;       ///< Index
const uint8_t kSanyoAcSensorBit = 2;  ///< Mask 0b00x00000
// Ambient Temp Mask                            0b000xxxxx
const uint8_t kSanyoAcBeepBit = 6;    ///< Mask 0b0x000000
// Byte[3] - Off Hour
const uint8_t kSanyoAcOffHourByte = 3;    ///< Index
const uint8_t kSanyoAcOffHourOffset = 0;  ///< Mask 0b0000xxxx
const uint8_t kSanyoAcOffHourSize = 4;    ///< Mask 0b0000xxxx
const uint8_t kSanyoAcHourMax = 15;       ///<          0b1111
// Byte[4] - Mode + Fan + Timer Enables
const uint8_t kSanyoAcModeByte = 4;    ///< Index
const uint8_t kSanyoAcModeOffset = 4;  ///< Mask 0b0xxx0000
const uint8_t kSanyoAcModeSize =   3;  ///< Mask 0b0xxx0000
const uint8_t kSanyoAcHeat = 1;        ///<       0b001
const uint8_t kSanyoAcCool = 2;        ///<       0b010
const uint8_t kSanyoAcDry =  3;        ///<       0b011
const uint8_t kSanyoAcAuto = 4;        ///<       0b100
const uint8_t kSanyoAcOffTimerEnableBit = 2;  ///< Mask 0b00000x00
const uint8_t kSanyoAcFanOffset = 0;   ///< Mask 0b000000xx
const uint8_t kSanyoAcFanSize =   2;   ///< Mask 0b000000xx
const uint8_t kSanyoAcFanAuto =   0;   ///<            0b00
const uint8_t kSanyoAcFanHigh =   1;   ///<            0b01
const uint8_t kSanyoAcFanLow =    2;   ///<            0b10
const uint8_t kSanyoAcFanMedium = 3;   ///<            0b11
// Byte[5] - Power + SwingV
const uint8_t kSanyoAcPowerByte = 5;    ///< Index
const uint8_t kSanyoAcPowerOffset = 6;  ///< Mask 0bxx000000
const uint8_t kSanyoAcPowerSize = 2;    ///< Mask 0bxx000000
// const uint8_t kSanyoAcPowerStandby =           0b00;  ///< Standby?
const uint8_t kSanyoAcPowerOff =                  0b01;  ///< Off
const uint8_t kSanyoAcPowerOn =                   0b10;  ///< On
const uint8_t kSanyoAcSwingVOffset = 0;  ///< Mask 0b00000xxx
const uint8_t kSanyoAcSwingVSize = 3;    ///< Mask 0b00000xxx
const uint8_t kSanyoAcSwingVAuto =         0;  ///<     0b000
const uint8_t kSanyoAcSwingVLowest =       2;  ///<     0b010
const uint8_t kSanyoAcSwingVLow =          3;  ///<     0b011
const uint8_t kSanyoAcSwingVLowerMiddle =  4;  ///<     0b100
const uint8_t kSanyoAcSwingVUpperMiddle =  5;  ///<     0b101
const uint8_t kSanyoAcSwingVHigh =         6;  ///<     0b110
const uint8_t kSanyoAcSwingVHighest =      7;  ///<     0b111
// Byte[6] - Sleep
const uint8_t kSanyoAcSleepByte = 6;    ///< Index
const uint8_t kSanyoAcSleepBit = 3;  ///< Mask 0b0000x000
// Byte[8] - Checksum (8-bit Sum of all preceeding nibbles)
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***         Exported global functions                                     ***/
/****************************************************************************/
class IrSanyo {
public:
	IrSanyo(uint32_t frequency, uint16_t dutycycle);
	virtual ~IrSanyo();
    void sendSanYoAc(const uint16_t repeat = kNoRepeat);
    void sendSanyoLC7461(const uint64_t data,
                    const uint16_t nbits = kSanyoLC7461Bits,
                    const uint16_t repeat = kNoRepeat);
    void setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp);
private:

    uint8_t convertFan(const stdAc::fanspeed_t speed);
    uint8_t convertMode(const stdAc::opmode_t mode);
    void send(const uint8_t *data,
                const uint16_t nbytes = kSanyoAcStateLength,
                const uint16_t repeat = kNoRepeat);
    void stateReset(void);
    void on(void);
    void off(void);
    void setPower(const bool on);
    bool getPower(void);
    void setTemp(const uint8_t degrees);
    uint8_t getTemp(void);
    void setSensorTemp(const uint8_t degrees);
    uint8_t getSensorTemp(void);
    void setFan(const uint8_t speed);
    uint8_t getFan(void);
    void setMode(const uint8_t mode);
    uint8_t getMode(void);
    void setSleep(const bool on);
    bool getSleep(void);
    void setSensor(const bool location);
    bool getSensor(void);
    void setBeep(const bool on);
    bool getBeep(void);
    void setSwingV(const uint8_t setting);
    uint8_t getSwingV(void);
    void setRaw(const uint8_t newState[]);
    uint8_t* getRaw(void);
    uint16_t getOffTimer(void);
    void setOffTimer(const uint16_t mins);
    static bool validChecksum(const uint8_t state[],
                              const uint16_t length = kSanyoAcStateLength);

    uint8_t remote_state[kSanyoAcStateLength];  ///< The state in IR code form.
    void checksum(void);
    static uint8_t calcChecksum(const uint8_t state[],
                                const uint16_t length = kSanyoAcStateLength);
    void _setTemp(uint8_t *ptr, const uint8_t degrees);
    uint8_t _getTemp(uint8_t *ptr);

    Ir_Send _ir_sanyo;
};
#endif /* MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRSANYO_H_ */

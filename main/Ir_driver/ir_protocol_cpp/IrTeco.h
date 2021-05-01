/*
 * IrTeco.h
 *
 *  Created on: Mar 9, 2021
 *      Author: ductu
 */

#ifndef MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRTECO_H_
#define MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRTECO_H_


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
// Constants.
const uint8_t kTecoAuto = 0;
const uint8_t kTecoCool = 1;
const uint8_t kTecoDry = 2;
const uint8_t kTecoFan = 3;
const uint8_t kTecoHeat = 4;
const uint8_t kTecoFanAuto = 0;  // 0b00
const uint8_t kTecoFanLow = 1;   // 0b01
const uint8_t kTecoFanMed = 2;   // 0b10
const uint8_t kTecoFanHigh = 3;  // 0b11
const uint8_t kTecoMinTemp = 16;   // 16C
const uint8_t kTecoMaxTemp = 30;   // 30C

const uint8_t kTecoModeOffset = 0;
const uint8_t kTecoPowerOffset = 3;
const uint8_t kTecoFanOffset = 4;
const uint8_t kTecoFanSize = 2;  // Nr. of bits
const uint8_t kTecoSwingOffset = 6;
const uint8_t kTecoSleepOffset = 7;
const uint8_t kTecoTempOffset = 8;
const uint8_t kTecoTempSize = 4;  // Nr. of bits
const uint8_t kTecoTimerHalfHourOffset = 12;
const uint8_t kTecoTimerTensHoursOffset = 13;
const uint8_t kTecoTimerTensHoursSize = 2;  // Nr. of bits
const uint8_t kTecoTimerOnOffset = 15;
const uint8_t kTecoTimerUnitHoursOffset = 16;
const uint8_t kTecoTimerUnitHoursSize = 4;  // Nr. of bits
const uint8_t kTecoHumidOffset = 20;
const uint8_t kTecoLightOffset = 21;
const uint8_t kTecoSaveOffset = 23;
const uint64_t kTecoReset =      0b01001010000000000000010000000000000;
/*
  (header mark and space)
        Teco AC map read and to be sent in LSB with number of bits

  byte 0 = Cst 0x02
  byte 1 = Cst 0x50
    b6-7 = "AIR" 0, 1, 2 (Not Implemented)
  byte 2:
    b0 = Save
    b1 = "Tree with bubbles" / Filter?? (Not Implemented)
    b2 = Light/LED.
    b3 = Humid
    b4-7 = Timer hours (unit, not thenth)
      hours:
        0000 (0) = +0 hour
        0001 (1) = +1 hour
        ...
        1001 (9) = +9 hours
  byte 3: = timer and Temperature
    b0 = Timer (1 = On, 0 = Off)
    b1-2 = Timer - number of 10hours
      10Hours:
        00 = 0 * 10hours of timer
        01 = 1 * 10 hours of timer
        10 = 2 * 10hours of timer
    b3 = Timer - half hour (1=half hour on, 0 = round hour)
    b4-7: Degrees C.
      0000 (0) = 16C
      0001 (1) = 17C
      0010 (2) = 18C
      ...
      1101 (13) = 29C
      1110 (14) = 30C
  byte 4: Basics
    b0 = Sleep Mode (1 = On, 0 = Off)
    b1 = Vent swing (1 = On, 0 = Off)
    b2-3 = Fan
      Fan:
        00 = Auto
        01 = Fan 1
        10 = Fan 2
        11 = Fan 3 or higher
    b4 = Power Status (1 = On, 0 = Off)
    b5-7 = Modes LSB first
      Modes:
        000 = Auto (temp = 25C)
        001 = Cool
        010 = Dry (temp = 25C, but not shown)
        011 = Fan
        100 = Heat
*/
/****************************************************************************/
/***         Exported global functions                                     ***/
/****************************************************************************/
class Ir_Teco {
public:
	Ir_Teco(uint32_t frequency, uint16_t dutycycle);
	virtual ~Ir_Teco();
	void sendTecoAc(const uint16_t repeat = kTecoDefaultRepeat);
	void setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp);
public:
	uint8_t convertMode(const stdAc::opmode_t mode);
	uint8_t convertFan(const stdAc::fanspeed_t speed);

	void send(const uint64_t data, const uint16_t nbits = kTecoBits,
            const uint16_t repeat = kNoRepeat);

    void stateReset(void);


    void begin(void);
    void on(void);
    void off(void);

    void setPower(const bool on);
    bool getPower(void);

    void setTemp(const uint8_t temp);
    uint8_t getTemp(void);

    void setFan(const uint8_t fan);
    uint8_t getFan(void);

    void setMode(const uint8_t mode);
    uint8_t getMode(void);

    void setSwing(const bool on);
    bool getSwing(void);

    void setSleep(const bool on);
    bool getSleep(void);

    void setLight(const bool on);
    bool getLight(void);

    void setHumid(const bool on);
    bool getHumid(void);

    void setSave(const bool on);
    bool getSave(void);

    uint16_t getTimer(void);
    void setTimer(const uint16_t mins);

    uint64_t getRaw(void);
    void setRaw(const uint64_t new_code);

    uint64_t remote_state;  ///< The state of the IR remote in IR code form.
    bool getTimerEnabled(void);

    Ir_Send _ir_remote;
};
#endif /* MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRTECO_H_ */

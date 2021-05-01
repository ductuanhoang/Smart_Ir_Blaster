/*
 * IrSamsung.h
 *
 *  Created on: Jan 12, 2021
 *      Author: ductu
 */

#ifndef MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRSAMSUNG_H_
#define MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRSAMSUNG_H_
#include "IrSend.h"

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/// Native representation of a Samsung A/C message.
union SamsungProtocol{
  uint8_t raw[kSamsungAcExtendedStateLength];  ///< State in code form.
  struct {
    // Byte 0
    uint8_t :8;
    // Byte 1
    uint8_t         :4;
    uint8_t Quiet1  :1;
    uint8_t Power1  :1;
    uint8_t         :2;
    // Byte 2~4
    uint8_t pad0[3];
    // Byte 5
    uint8_t         :5;
    uint8_t Quiet5  :1;
    uint8_t         :2;
    // Byte 6
    uint8_t         :4;
    uint8_t Power6  :2;
    uint8_t         :2;
    // Byte 7
    uint8_t :8;
    // Byte 8
    uint8_t Powerful8 :8;
    // Byte 9
    uint8_t       :4;
    uint8_t Swing :3;
    uint8_t       :1;
    // Byte 10
    uint8_t             :1;
    uint8_t Powerful10  :3;
    uint8_t Display     :1;
    uint8_t             :2;
    uint8_t Clean10     :1;
    // Byte 11
    uint8_t Ion     :1;
    uint8_t Clean11 :1;
    uint8_t         :2;
    uint8_t Temp    :4;
    // Byte 12
    uint8_t       :1;
    uint8_t Fan   :3;
    uint8_t Mode  :3;
    uint8_t       :1;
    // Byte 13
    uint8_t       :1;
    uint8_t Beep  :1;
    uint8_t       :6;
  };
  struct {
    uint8_t :8;
    // Byte 1
    uint8_t       :4;
    uint8_t Sum1  :4;
    uint8_t pad1[6];
    // Byte 8
    uint8_t       :4;
    uint8_t Sum2  :4;
    uint8_t :8;
    // Byte 10
    uint8_t         :1;
    uint8_t Breeze  :3;  // WindFree
    uint8_t         :4;
  };
};

// Constants
const uint8_t kSamsungAcPowerfulMask8 = 0b01010000;
const uint8_t kSamsungAcSwingMove =                0b010;
const uint8_t kSamsungAcSwingStop =                0b111;
const uint8_t kSamsungAcPowerful10On =                     0b011;
const uint8_t kSamsungAcBreezeOn =                         0b101;
const uint8_t kSamsungAcMinTemp = 16;   // C   Mask 0b11110000
const uint8_t kSamsungAcMaxTemp = 30;   // C   Mask 0b11110000
const uint8_t kSamsungAcAutoTemp = 25;  // C   Mask 0b11110000
const uint8_t kSamsungAcAuto = 0;
const uint8_t kSamsungAcCool = 1;
const uint8_t kSamsungAcDry = 2;
const uint8_t kSamsungAcFan = 3;
const uint8_t kSamsungAcHeat = 4;
const uint8_t kSamsungAcFanAuto = 0;
const uint8_t kSamsungAcFanLow = 2;
const uint8_t kSamsungAcFanMed = 4;
const uint8_t kSamsungAcFanHigh = 5;
const uint8_t kSamsungAcFanAuto2 = 6;
const uint8_t kSamsungAcFanTurbo = 7;
const uint16_t kSamsungAcSectionLength = 7;
const uint64_t kSamsungAcPowerSection = 0x1D20F00000000;
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***         Exported global functions                                     ***/
/****************************************************************************/
class Ir_Samsung
{
public:
	Ir_Samsung(uint32_t frequency, uint16_t dutycycle);
	virtual ~Ir_Samsung();

	void sendSamsung(const uint64_t data, const uint16_t nbits = kSamsungBits,
                     const uint16_t repeat = kNoRepeat);
	void sendSamsung36(const uint64_t data, const uint16_t nbits = kSamsung36Bits,
            const uint16_t repeat = kNoRepeat);
	void sendSamsungAC(const uint16_t repeat, const bool calcchecksum = true);

	void setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp);
private:
	uint8_t convertFan(const stdAc::fanspeed_t speed);
	uint8_t convertMode(const stdAc::opmode_t mode);
	void send(const unsigned char data[],
            const uint16_t nbytes = kSamsungAcStateLength,
            const uint16_t repeat = kSamsungAcDefaultRepeat);
    void sendOn(const uint16_t repeat = kSamsungAcDefaultRepeat);
    void sendOff(const uint16_t repeat = kSamsungAcDefaultRepeat);

    void on(void);
    void off(void);

    void setPower(const bool on);
    bool getPower(void);
    void setTemp(const uint8_t temp);
    uint8_t getTemp(void);
    void setFan(const uint8_t speed);
    uint8_t getFan(void);
    void setMode(const uint8_t mode);
    uint8_t getMode(void);
    void setSwing(const bool on);
    bool getSwing(void);
    void setBeep(const bool on);
    bool getBeep(void);
    void setClean(const bool on);
    bool getClean(void);
    void setQuiet(const bool on);
    bool getQuiet(void);
    void setPowerful(const bool on);
    bool getPowerful(void);
    void setBreeze(const bool on);
    bool getBreeze(void);
    void setDisplay(const bool on);
    bool getDisplay(void);
    void setIon(const bool on);
    bool getIon(void);

    void stateReset(const bool forcepower = true, const bool initialPower = true);

	SamsungProtocol _;
	bool _forcepower;  ///< Hack to know when we need to send a special power mesg
	bool _lastsentpowerstate;
	void checksum(const uint16_t length = kSamsungAcStateLength);
	uint8_t calcChecksum(const uint8_t state[], const uint16_t length);
	Ir_Send _ir_samsungAc_send;
};
#endif /* MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRSAMSUNG_H_ */

/*
 * IrAmcor.h
 *
 *  Created on: Jan 14, 2021
 *      Author: ductu
 */

#ifndef MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRAMCOR_H_
#define MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRAMCOR_H_


/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "IrSend.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
/// Native representation of a Amcor A/C message.
union AmcorProtocol{
    uint8_t raw[kAmcorStateLength];  // The state of the IR remote.
    struct {
        // Byte 0
        uint8_t       :8;  // Typically 0x01
        // Byte 1
        uint8_t Mode  :3;
        uint8_t       :1;
        uint8_t Fan   :3;
        uint8_t       :1;
        // Byte 2
        uint8_t       :1;
        uint8_t Temp  :6;
        uint8_t       :1;
        // Byte 3
        uint8_t       :8;
        // Byte 4
        uint8_t       :8;
        // Byte 5
        uint8_t       :4;
        uint8_t Power :4;
        // Byte 6
        uint8_t Max   :2;
        uint8_t       :4;
        uint8_t Vent  :2;
        // Byte 7
        uint8_t Sum   :8;
    };
};

// Constants

// Fan Control
const uint8_t kAmcorFanMin =   0b001;
const uint8_t kAmcorFanMed =   0b010;
const uint8_t kAmcorFanMax =   0b011;
const uint8_t kAmcorFanAuto =  0b100;
// Modes
const uint8_t kAmcorCool =     0b001;
const uint8_t kAmcorHeat =     0b010;
const uint8_t kAmcorFan =      0b011;  // Aka "Vent"
const uint8_t kAmcorDry =      0b100;
const uint8_t kAmcorAuto =     0b101;

// Temperature
const uint8_t kAmcorMinTemp = 12;  // Celsius
const uint8_t kAmcorMaxTemp = 32;  // Celsius

// Power
const uint8_t kAmcorPowerOn =   0b0011;  // 0x3
const uint8_t kAmcorPowerOff =  0b1100;  // 0xC

// Max Mode (aka "Lo" in Cool and "Hi" in Heat)
const uint8_t kAmcorMax = 0b11;

// "Vent" Mode
const uint8_t kAmcorVentOn =  0b11;


/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***         Exported global functions                                     ***/
/****************************************************************************/
class Ir_Amcor {
public:
	Ir_Amcor(uint32_t frequency, uint16_t dutycycle);
	virtual ~Ir_Amcor();
	void sendAmcor(const uint16_t repeat = kAmcorDefaultRepeat);
	void setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp);
private:
    uint8_t convertFan(const stdAc::fanspeed_t speed);
    uint8_t convertMode(const stdAc::opmode_t mode);

    void send(const unsigned char data[],
              const uint16_t nbytes = kAmcorStateLength,
              const uint16_t repeat = kAmcorDefaultRepeat);

    void stateReset();

    static uint8_t calcChecksum(const uint8_t state[],
                                const uint16_t length = kAmcorStateLength);
    static bool validChecksum(const uint8_t state[],
                              const uint16_t length = kAmcorStateLength);
    void setPower(const bool on);
    bool getPower(void) const;
    void on(void);
    void off(void);
    void setTemp(const uint8_t degrees);
    uint8_t getTemp(void) const;
    void setMax(const bool on);
    bool getMax(void) const;
    void setFan(const uint8_t speed);
    uint8_t getFan(void) const;
    void setMode(const uint8_t mode);
    uint8_t getMode(void) const;
    uint8_t* getRaw(void);
    void setRaw(const uint8_t state[]);

    AmcorProtocol _;
    void checksum(void);
    Ir_Send _ir_amcor_send;
};
#endif /* MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRAMCOR_H_ */

/*
 * IrMitsubishiHeavy.h
 *
 *  Created on: Mar 10, 2021
 *      Author: ductu
 */

#ifndef MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRMITSUBISHIHEAVY_H_
#define MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRMITSUBISHIHEAVY_H_


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

/****************************************************************************/
/***         Exported global functions                                     ***/
/****************************************************************************/
/// Native representation of a Mitsubishi Heavy 152-bit A/C message.
union Mitsubishi152Protocol{
    uint8_t raw[kMitsubishiHeavy152StateLength];  ///< State in code form
    struct {
        // Byte 0~4
        uint8_t Sig[5];
        // Byte 5
        uint8_t Mode  :3;
        uint8_t Power :1;
        uint8_t       :1;
        uint8_t Clean :1;
        uint8_t Filter:1;
        uint8_t       :1;
        // Byte 6
        uint8_t       :8;
        // Byte 7
        uint8_t Temp  :4;
        uint8_t       :4;
        // Byte 8
        uint8_t       :8;
        // Byte 9
        uint8_t Fan   :4;
        uint8_t       :4;
        // Byte 10
        uint8_t       :8;
        // Byte 11
        uint8_t         :1;
        uint8_t Three   :1;
        uint8_t         :2;
        uint8_t D       :1;  // binding with "Three"
        uint8_t SwingV  :3;
        // Byte 12
        uint8_t         :8;
        // Byte 13
        uint8_t SwingH  :4;
        uint8_t         :4;
        // Byte 14
        uint8_t         :8;
        // Byte 15
        uint8_t         :6;
        uint8_t Night   :1;
        uint8_t Silent  :1;
    };
};

// Constants.
const uint8_t kMitsubishiHeavySigLength = 5;

// ZMS (152 bit)
const uint8_t kMitsubishiHeavyZmsSig[kMitsubishiHeavySigLength] = {
        0xAD, 0x51, 0x3C, 0xE5, 0x1A};

const uint8_t kMitsubishiHeavyAuto = 0;         // 0b000
const uint8_t kMitsubishiHeavyCool = 1;         // 0b001
const uint8_t kMitsubishiHeavyDry =  2;         // 0b010
const uint8_t kMitsubishiHeavyFan =  3;         // 0b011
const uint8_t kMitsubishiHeavyHeat = 4;         // 0b100

const uint8_t kMitsubishiHeavyMinTemp = 17;   // 17C
const uint8_t kMitsubishiHeavyMaxTemp = 31;   // 31C

const uint8_t kMitsubishiHeavy152FanAuto =  0x0;  // 0b0000
const uint8_t kMitsubishiHeavy152FanLow =   0x1;  // 0b0001
const uint8_t kMitsubishiHeavy152FanMed =   0x2;  // 0b0010
const uint8_t kMitsubishiHeavy152FanHigh =  0x3;  // 0b0011
const uint8_t kMitsubishiHeavy152FanMax =   0x4;  // 0b0100
const uint8_t kMitsubishiHeavy152FanEcono = 0x6;  // 0b0110
const uint8_t kMitsubishiHeavy152FanTurbo = 0x8;  // 0b1000

const uint8_t kMitsubishiHeavy152SwingVAuto =    0;  // 0b000
const uint8_t kMitsubishiHeavy152SwingVHighest = 1;  // 0b001
const uint8_t kMitsubishiHeavy152SwingVHigh =    2;  // 0b010
const uint8_t kMitsubishiHeavy152SwingVMiddle =  3;  // 0b011
const uint8_t kMitsubishiHeavy152SwingVLow =     4;  // 0b100
const uint8_t kMitsubishiHeavy152SwingVLowest =  5;  // 0b101
const uint8_t kMitsubishiHeavy152SwingVOff =     6;  // 0b110

const uint8_t kMitsubishiHeavy152SwingHAuto =      0;  // 0b0000
const uint8_t kMitsubishiHeavy152SwingHLeftMax =   1;  // 0b0001
const uint8_t kMitsubishiHeavy152SwingHLeft =      2;  // 0b0010
const uint8_t kMitsubishiHeavy152SwingHMiddle =    3;  // 0b0011
const uint8_t kMitsubishiHeavy152SwingHRight =     4;  // 0b0100
const uint8_t kMitsubishiHeavy152SwingHRightMax =  5;  // 0b0101
const uint8_t kMitsubishiHeavy152SwingHRightLeft = 6;  // 0b0110
const uint8_t kMitsubishiHeavy152SwingHLeftRight = 7;  // 0b0111
const uint8_t kMitsubishiHeavy152SwingHOff =       8;  // 0b1000

/// Native representation of a Mitsubishi Heavy 88-bit A/C message.
union Mitsubishi88Protocol{
    uint8_t raw[kMitsubishiHeavy88StateLength];  ///< State in code form
    struct {
        // Byte 0~4
        uint8_t Sig[5];
        // Byte 5
        uint8_t         :1;
        uint8_t SwingV5 :1;
        uint8_t SwingH1 :2;
        uint8_t         :1;
        uint8_t Clean   :1;
        uint8_t SwingH2 :2;
        // Byte 6
        uint8_t         :8;
        // Byte 7
        uint8_t         :3;
        uint8_t SwingV7 :2;
        uint8_t Fan     :3;
        // Byte 8
        uint8_t       :8;
        // Byte 9
        uint8_t Mode  :3;
        uint8_t Power :1;
        uint8_t Temp  :4;
    };
};

// ZJS (88 bit)
const uint8_t kMitsubishiHeavyZjsSig[kMitsubishiHeavySigLength] = {
        0xAD, 0x51, 0x3C, 0xD9, 0x26};

const uint8_t kMitsubishiHeavy88SwingHSize = 2;  // Bits (per offset)
const uint8_t kMitsubishiHeavy88SwingHOff =       0b0000;
const uint8_t kMitsubishiHeavy88SwingHAuto =      0b1000;
const uint8_t kMitsubishiHeavy88SwingHLeftMax =   0b0001;
const uint8_t kMitsubishiHeavy88SwingHLeft =      0b0101;
const uint8_t kMitsubishiHeavy88SwingHMiddle =    0b1001;
const uint8_t kMitsubishiHeavy88SwingHRight =     0b1101;
const uint8_t kMitsubishiHeavy88SwingHRightMax =  0b0010;
const uint8_t kMitsubishiHeavy88SwingHRightLeft = 0b1010;
const uint8_t kMitsubishiHeavy88SwingHLeftRight = 0b0110;
const uint8_t kMitsubishiHeavy88SwingH3D =        0b1110;

const uint8_t kMitsubishiHeavy88FanAuto =  0;  // 0b000
const uint8_t kMitsubishiHeavy88FanLow =   2;  // 0b010
const uint8_t kMitsubishiHeavy88FanMed =   3;  // 0b011
const uint8_t kMitsubishiHeavy88FanHigh =  4;  // 0b100
const uint8_t kMitsubishiHeavy88FanTurbo = 6;  // 0b110
const uint8_t kMitsubishiHeavy88FanEcono = 7;  // 0b111
const uint8_t kMitsubishiHeavy88SwingVByte5Size = 1;

// Mask 0b111
const uint8_t kMitsubishiHeavy88SwingVOff =       0b000;  // 0
const uint8_t kMitsubishiHeavy88SwingVAuto =      0b100;  // 4
const uint8_t kMitsubishiHeavy88SwingVHighest =   0b110;  // 6
const uint8_t kMitsubishiHeavy88SwingVHigh =      0b001;  // 1
const uint8_t kMitsubishiHeavy88SwingVMiddle =    0b011;  // 3
const uint8_t kMitsubishiHeavy88SwingVLow =       0b101;  // 5
const uint8_t kMitsubishiHeavy88SwingVLowest =    0b111;  // 7


// Classes

/// Class for handling detailed Mitsubishi Heavy 152-bit A/C messages.
class IRMitsubishiHeavy152Ac {
public:
    IRMitsubishiHeavy152Ac();
    virtual ~IRMitsubishiHeavy152Ac();
    void sendMitsubishiHeavy152Ac(const uint16_t repeat = kMitsubishiHeavy152MinRepeat);
    void setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp);

    void stateReset(void);

    void send (const unsigned char data[],const uint16_t nbytes = kMitsubishiHeavy152StateLength,
                const uint16_t repeat = kMitsubishiHeavy152MinRepeat);

    void begin(void);
    void on(void);
    void off(void);

    void setPower(const bool on);
    bool getPower(void) const;

    void setTemp(const uint8_t temp);
    uint8_t getTemp(void) const;

    void setFan(const uint8_t fan);
    uint8_t getFan(void) const;

    void setMode(const uint8_t mode);
    uint8_t getMode(void) const;

    void setSwingVertical(const uint8_t pos);
    uint8_t getSwingVertical(void) const;
    void setSwingHorizontal(const uint8_t pos);
    uint8_t getSwingHorizontal(void) const;

    void setNight(const bool on);
    bool getNight(void) const;

    void set3D(const bool on);
    bool get3D(void) const;

    void setSilent(const bool on);
    bool getSilent(void) const;

    void setFilter(const bool on);
    bool getFilter(void) const;

    void setClean(const bool on);
    bool getClean(void) const;

    void setTurbo(const bool on);
    bool getTurbo(void) const;

    void setEcono(const bool on);
    bool getEcono(void) const;

    uint8_t* getRaw(void);
    void setRaw(const uint8_t* data);

    static bool checkZmsSig(const uint8_t *state);
    static bool validChecksum(
            const uint8_t *state,
            const uint16_t length = kMitsubishiHeavy152StateLength);
    static uint8_t convertMode(const stdAc::opmode_t mode);
    static uint8_t convertFan(const stdAc::fanspeed_t speed);
    static uint8_t convertSwingV(const stdAc::swingv_t position);
    static uint8_t convertSwingH(const stdAc::swingh_t position);
    void checksum(void);
private:
    Mitsubishi152Protocol _;
};

/// Class for handling detailed Mitsubishi Heavy 88-bit A/C messages.
class IRMitsubishiHeavy88Ac {
public:
    IRMitsubishiHeavy88Ac(uint32_t frequency, uint16_t dutycycle);
    virtual ~IRMitsubishiHeavy88Ac();
    void sendMitsubishiHeavy88(const uint16_t repeat = kMitsubishiHeavy88MinRepeat);
    void send(const unsigned char data[],
                const uint16_t nbytes = kMitsubishiHeavy88StateLength,
                const uint16_t repeat = kMitsubishiHeavy88MinRepeat);
    void setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp);

    void stateReset(void);
    void on(void);
    void off(void);

    void setPower(const bool on);
    bool getPower(void) const;

    void setTemp(const uint8_t temp);
    uint8_t getTemp(void) const;

    void setFan(const uint8_t fan);
    uint8_t getFan(void) const;

    void setMode(const uint8_t mode);
    uint8_t getMode(void) const;

    void setSwingVertical(const uint8_t pos);
    uint8_t getSwingVertical(void) const;
    void setSwingHorizontal(const uint8_t pos);
    uint8_t getSwingHorizontal(void) const;

    void setTurbo(const bool on);
    bool getTurbo(void) const;

    void setEcono(const bool on);
    bool getEcono(void) const;

    void set3D(const bool on);
    bool get3D(void) const;

    void setClean(const bool on);
    bool getClean(void) const;

    uint8_t* getRaw(void);
    void setRaw(const uint8_t* data);

    static bool checkZjsSig(const uint8_t *state);
    static bool validChecksum(
            const uint8_t *state,
            const uint16_t length = kMitsubishiHeavy88StateLength);
    static uint8_t convertMode(const stdAc::opmode_t mode);
    static uint8_t convertFan(const stdAc::fanspeed_t speed);
    static uint8_t convertSwingV(const stdAc::swingv_t position);
    static uint8_t convertSwingH(const stdAc::swingh_t position);
    void checksum(void);
private:
    Mitsubishi88Protocol _;


    Ir_Send _ir_remote;
};
#endif /* MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRMITSUBISHIHEAVY_H_ */

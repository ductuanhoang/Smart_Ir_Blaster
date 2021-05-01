/*
 * IrMitsubishi112.h
 *
 *  Created on: Mar 9, 2021
 *      Author: ductu
 */

#ifndef MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRMITSUBISHI112_H_
#define MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRMITSUBISHI112_H_


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
/// Native representation of a Mitsubishi 144-bit A/C message.
union Mitsubishi144Protocol{
    uint8_t raw[kMitsubishiACStateLength];  ///< The state in code form.
    struct {
        // Byte 0~4
        uint8_t pad0[5];
        // Byte 5
        uint8_t       :5;
        uint8_t Power :1;
        uint8_t       :2;
        // Byte 6
        uint8_t       :3;
        uint8_t Mode  :3;
        uint8_t       :2;
        // Byte 7
        uint8_t Temp  :8;
        // Byte 8
        uint8_t         :4;
        uint8_t WideVane:4;  // SwingH
        // Byte 9
        uint8_t Fan     :3;
        uint8_t Vane    :3;  // SwingV
        uint8_t VaneBit :1;
        uint8_t FanAuto :1;
        // Byte 10
        uint8_t Clock   :8;
        // Byte 11
        uint8_t StopClock :8;
        // Byte 12
        uint8_t StartClock:8;
        // Byte 13
        uint8_t Timer :3;
        uint8_t       :5;
        // Byte 14~16
        uint8_t pad1[3];
        // Byte 17
        uint8_t Sum   :8;
    };
};

// Constants
const uint8_t kMitsubishiAcAuto = 0b100;
const uint8_t kMitsubishiAcCool = 0b011;
const uint8_t kMitsubishiAcDry =  0b010;
const uint8_t kMitsubishiAcHeat = 0b001;
const uint8_t kMitsubishiAcFanAuto = 0;
const uint8_t kMitsubishiAcFanMax = 5;
const uint8_t kMitsubishiAcFanRealMax = 4;
const uint8_t kMitsubishiAcFanSilent = 6;
const uint8_t kMitsubishiAcFanQuiet = kMitsubishiAcFanSilent;
const uint8_t kMitsubishiAcMinTemp = 16;  // 16C
const uint8_t kMitsubishiAcMaxTemp = 31;  // 31C
const uint8_t kMitsubishiAcVaneAuto = 0;
const uint8_t kMitsubishiAcVaneAutoMove = 7;
const uint8_t kMitsubishiAcNoTimer = 0;
const uint8_t kMitsubishiAcStartTimer = 5;
const uint8_t kMitsubishiAcStopTimer = 3;
const uint8_t kMitsubishiAcStartStopTimer = 7;
const uint8_t kMitsubishiAcWideVaneAuto = 8;

/// Native representation of a Mitsubishi 136-bit A/C message.
union Mitsubishi136Protocol{
    uint8_t raw[kMitsubishi136StateLength];  ///< The state in code form.
    struct {
        // Byte 0~4
        uint8_t pad[5];
        // Byte 5
        uint8_t       :6;
        uint8_t Power :1;
        uint8_t       :1;
        // Byte 6
        uint8_t Mode  :3;
        uint8_t       :1;
        uint8_t Temp  :4;
        // Byte 7
        uint8_t         :1;
        uint8_t Fan     :2;
        uint8_t         :1;
        uint8_t SwingV  :4;
    };
};

const uint8_t kMitsubishi136PowerByte = 5;
const uint8_t kMitsubishi136MinTemp = 17;  // 17C
const uint8_t kMitsubishi136MaxTemp = 30;  // 30C
const uint8_t kMitsubishi136Fan =             0b000;
const uint8_t kMitsubishi136Cool =            0b001;
const uint8_t kMitsubishi136Heat =            0b010;
const uint8_t kMitsubishi136Auto =            0b011;
const uint8_t kMitsubishi136Dry =             0b101;
const uint8_t kMitsubishi136SwingVLowest =   0b0000;
const uint8_t kMitsubishi136SwingVLow =      0b0001;
const uint8_t kMitsubishi136SwingVHigh =     0b0010;
const uint8_t kMitsubishi136SwingVHighest =  0b0011;
const uint8_t kMitsubishi136SwingVAuto =     0b1100;
const uint8_t kMitsubishi136FanMin =          0b00;
const uint8_t kMitsubishi136FanLow =          0b01;
const uint8_t kMitsubishi136FanMed =          0b10;
const uint8_t kMitsubishi136FanMax =          0b11;
const uint8_t kMitsubishi136FanQuiet = kMitsubishi136FanMin;

/// Native representation of a Mitsubishi 112-bit A/C message.
union Mitsubishi112Protocol{
    uint8_t raw[kMitsubishi112StateLength];  ///< The state in code form.
    struct {
        // Byte 0~4
        uint8_t pad0[5];
        // Byte 5
        uint8_t       :2;
        uint8_t Power :1;
        uint8_t       :5;
        // Byte 6
        uint8_t Mode  :3;
        uint8_t       :5;
        // Byte 7
        uint8_t Temp  :4;
        uint8_t       :4;
        // Byte 8
        uint8_t Fan     :3;
        uint8_t SwingV  :3;
        uint8_t         :2;
        // Byte 9~11
        uint8_t pad1[3];
        // Byte 12
        uint8_t         :2;
        uint8_t SwingH  :4;
        uint8_t         :2;
        // Byte 13
        uint8_t Sum :8;
    };
};

const uint8_t kMitsubishi112Cool =                        0b011;
const uint8_t kMitsubishi112Heat =                        0b001;
const uint8_t kMitsubishi112Auto =                        0b111;
const uint8_t kMitsubishi112Dry =                         0b010;

const uint8_t kMitsubishi112MinTemp = 16;  // 16C
const uint8_t kMitsubishi112MaxTemp = 31;  // 31C

const uint8_t kMitsubishi112FanMin =                     0b010;
const uint8_t kMitsubishi112FanLow =                     0b011;
const uint8_t kMitsubishi112FanMed =                     0b101;
const uint8_t kMitsubishi112FanMax =                     0b000;
const uint8_t kMitsubishi112FanQuiet = kMitsubishi112FanMin;
const uint8_t kMitsubishi112SwingVLowest =               0b101;
const uint8_t kMitsubishi112SwingVLow =                  0b100;
const uint8_t kMitsubishi112SwingVMiddle =               0b011;
const uint8_t kMitsubishi112SwingVHigh =                 0b010;
const uint8_t kMitsubishi112SwingVHighest =              0b001;
const uint8_t kMitsubishi112SwingVAuto =                 0b111;

const uint8_t kMitsubishi112SwingHLeftMax =              0b0001;
const uint8_t kMitsubishi112SwingHLeft =                 0b0010;
const uint8_t kMitsubishi112SwingHMiddle =               0b0011;
const uint8_t kMitsubishi112SwingHRight =                0b0100;
const uint8_t kMitsubishi112SwingHRightMax =             0b0101;
const uint8_t kMitsubishi112SwingHWide =                 0b1000;
const uint8_t kMitsubishi112SwingHAuto =                 0b1100;

// Legacy defines (Deprecated)
#define MITSUBISHI_AC_VANE_AUTO_MOVE kMitsubishiAcVaneAutoMove
#define MITSUBISHI_AC_VANE_AUTO kMitsubishiAcVaneAuto
#define MITSUBISHI_AC_MIN_TEMP kMitsubishiAcMinTemp
#define MITSUBISHI_AC_MAX_TEMP kMitsubishiAcMaxTemp
#define MITSUBISHI_AC_HEAT kMitsubishiAcHeat
#define MITSUBISHI_AC_FAN_SILENT kMitsubishiAcFanSilent
#define MITSUBISHI_AC_FAN_REAL_MAX kMitsubishiAcFanRealMax
#define MITSUBISHI_AC_FAN_MAX kMitsubishiAcFanMax
#define MITSUBISHI_AC_FAN_AUTO kMitsubishiAcFanAuto
#define MITSUBISHI_AC_DRY kMitsubishiAcDry
#define MITSUBISHI_AC_COOL kMitsubishiAcCool
#define MITSUBISHI_AC_AUTO kMitsubishiAcAuto


/// Class for handling detailed Mitsubishi 144-bit A/C messages.
/// @note Inspired and derived from the work done at: https://github.com/r45635/HVAC-IR-Control
/// @warning Consider this very alpha code. Seems to work, but not validated.
class IRMitsubishiAC {
public:
    IRMitsubishiAC(uint32_t frequency, uint16_t dutycycle);
    virtual ~IRMitsubishiAC();
    void sendMitsubishiAC(const uint16_t repeat = kMitsubishiACMinRepeat);
    void setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp);
private:
    void send(const unsigned char data[],
              const uint16_t nbytes = kMitsubishiACStateLength,
              const uint16_t repeat = kMitsubishiACMinRepeat);

    void stateReset(void);
    static bool validChecksum(const uint8_t* data);
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
    void setVane(const uint8_t position);
    void setWideVane(const uint8_t position);
    uint8_t getVane(void) const;
    uint8_t getWideVane(void) const;
    uint8_t* getRaw(void);
    void setRaw(const uint8_t* data);
    uint8_t getClock(void) const;
    void setClock(const uint8_t clock);
    uint8_t getStartClock(void) const;
    void setStartClock(const uint8_t clock);
    uint8_t getStopClock(void) const;
    void setStopClock(const uint8_t clock);
    uint8_t getTimer(void) const;
    void setTimer(const uint8_t timer);
    static uint8_t convertMode(const stdAc::opmode_t mode);
    static uint8_t convertFan(const stdAc::fanspeed_t speed);
    static uint8_t convertSwingV(const stdAc::swingv_t position);
    static uint8_t convertSwingH(const stdAc::swingh_t position);

    Mitsubishi144Protocol _;
    void checksum(void);
    static uint8_t calculateChecksum(const uint8_t* data);

    Ir_Send _ir_remote;
};

/// Class for handling detailed Mitsubishi 136-bit A/C messages.
class IRMitsubishi136 {
public:
    IRMitsubishi136(uint32_t frequency, uint16_t dutycycle);
    virtual ~IRMitsubishi136();
    void sendMitsubishi136(const uint16_t repeat = kMitsubishi136MinRepeat);
    void setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp);
private:
    void stateReset(void);
    void send(const unsigned char data[],
              const uint16_t nbytes = kMitsubishi136StateLength,
              const uint16_t repeat = kMitsubishi136MinRepeat);

    static bool validChecksum(const uint8_t* data,
                              const uint16_t len = kMitsubishi136StateLength);
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
    void setSwingV(const uint8_t position);
    uint8_t getSwingV(void) const;
    void setQuiet(const bool on);
    bool getQuiet(void) const;
    uint8_t* getRaw(void);

    void setRaw(const uint8_t* data);
    static uint8_t convertMode(const stdAc::opmode_t mode);
    static uint8_t convertFan(const stdAc::fanspeed_t speed);
    static uint8_t convertSwingV(const stdAc::swingv_t position);

    Mitsubishi136Protocol _;
    void checksum(void);
    Ir_Send _ir_remote;
};

/// Class for handling detailed Mitsubishi 122-bit A/C messages.
class IRMitsubishi112 {
public:
    IRMitsubishi112(uint32_t frequency, uint16_t dutycycle);
    virtual ~IRMitsubishi112();
    void stateReset(void);

    void sendMitsubishi112(const uint16_t repeat = kMitsubishi112MinRepeat);
    void setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp);
private:
    void send(const unsigned char data[],
              const uint16_t nbytes = kMitsubishi112StateLength,
              const uint16_t repeat = kMitsubishi112MinRepeat);
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
    void setSwingV(const uint8_t position);
    uint8_t getSwingV(void) const;
    void setSwingH(const uint8_t position);
    uint8_t getSwingH(void) const;
    void setQuiet(const bool on);
    bool getQuiet(void) const;
    uint8_t* getRaw(void);
    void setRaw(const uint8_t* data);
    static uint8_t convertMode(const stdAc::opmode_t mode);
    static uint8_t convertFan(const stdAc::fanspeed_t speed);
    static uint8_t convertSwingV(const stdAc::swingv_t position);
    static uint8_t convertSwingH(const stdAc::swingh_t position);

    Mitsubishi112Protocol _;
    void checksum(void);

    Ir_Send _ir_remote;
};
#endif /* MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRMITSUBISHI112_H_ */

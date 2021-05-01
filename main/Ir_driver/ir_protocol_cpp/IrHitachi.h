/*
 * IrHitachi.h
 *
 *  Created on: Mar 6, 2021
 *      Author: ductu
 */

#ifndef MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRHITACHI_H_
#define MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRHITACHI_H_
#include "IrSend.h"

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
/// HITACHI_AC1 A/C model numbers
enum hitachi_ac1_remote_model_t {
  R_LT0541_HTA_A = 1,  // (1) R-LT0541-HTA Remote in "A" setting. (Default)
  R_LT0541_HTA_B,      // (2) R-LT0541-HTA Remote in "B" setting.
};
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
/// Native representation of a Hitachi 224-bit A/C message.
union HitachiProtocol{
    uint8_t raw[kHitachiAcStateLength];  ///< The state in native code.
    struct {
        // Byte 0~9
        uint8_t pad0[10];
        // Byte 10
        uint8_t Mode  :8;
        // Byte 11
        uint8_t Temp  :8;
        // Byte 12
        uint8_t       :8;
        // Byte 13
        uint8_t Fan   :8;
        // Byte 14
        uint8_t         :7;
        uint8_t SwingV  :1;
        // Byte 15
        uint8_t         :7;
        uint8_t SwingH  :1;
        // Byte 16
        uint8_t       :8;
        // Byte 17
        uint8_t Power :1;
        uint8_t       :7;
        // Byte 18~26
        uint8_t pad1[9];
        // Byte 27
        uint8_t Sum   :8;
    };
};

// Constants
const uint16_t kHitachiAcFreq = 38000;  // Hz.
const uint8_t kHitachiAcAuto = 2;
const uint8_t kHitachiAcHeat = 3;
const uint8_t kHitachiAcCool = 4;
const uint8_t kHitachiAcDry = 5;
const uint8_t kHitachiAcFan = 0xC;
const uint8_t kHitachiAcFanAuto = 1;
const uint8_t kHitachiAcFanLow = 2;
const uint8_t kHitachiAcFanMed = 3;
const uint8_t kHitachiAcFanHigh = 5;
const uint8_t kHitachiAcMinTemp = 16;   // 16C
const uint8_t kHitachiAcMaxTemp = 32;   // 32C
const uint8_t kHitachiAcAutoTemp = 23;  // 23C

/// Native representation of a Hitachi 53-byte/424-bit A/C message.
union Hitachi424Protocol{
    uint8_t raw[kHitachiAc424StateLength];  ///< The state in native code
    struct {
        // Byte 0~10
        uint8_t pad0[11];
        // Byte 11
        uint8_t Button  :8;
        // Byte 12
        uint8_t       :8;
        // Byte 13
        uint8_t       :2;
        uint8_t Temp  :6;
        // Byte 14~24
        uint8_t pad1[11];
        // Byte 25
        uint8_t Mode  :4;
        uint8_t Fan   :4;
        // Byte 26
        uint8_t       :8;
        // Byte 27
        uint8_t Power :8;
        // Byte 28~34
        uint8_t pad2[7];
        // Byte 35
        uint8_t SwingH  :3;
        uint8_t         :5;
        // Byte 36
        uint8_t         :8;
        // Byte 37
        uint8_t         :5;
        uint8_t SwingV  :1;
        uint8_t         :2;
    };
};

// HitachiAc424 & HitachiAc344
const uint8_t kHitachiAc424ButtonPowerMode = 0x13;
const uint8_t kHitachiAc424ButtonFan = 0x42;
const uint8_t kHitachiAc424ButtonTempDown = 0x43;
const uint8_t kHitachiAc424ButtonTempUp = 0x44;
const uint8_t kHitachiAc424ButtonSwingV = 0x81;
const uint8_t kHitachiAc424ButtonSwingH = 0x8C;
const uint8_t kHitachiAc344ButtonPowerMode = kHitachiAc424ButtonPowerMode;
const uint8_t kHitachiAc344ButtonFan = kHitachiAc424ButtonFan;
const uint8_t kHitachiAc344ButtonTempDown = kHitachiAc424ButtonTempDown;
const uint8_t kHitachiAc344ButtonTempUp = kHitachiAc424ButtonTempUp;
const uint8_t kHitachiAc344ButtonSwingV = kHitachiAc424ButtonSwingV;
const uint8_t kHitachiAc344ButtonSwingH = kHitachiAc424ButtonSwingH;

const uint8_t kHitachiAc424MinTemp = 16;   // 16C
const uint8_t kHitachiAc424MaxTemp = 32;   // 32C
const uint8_t kHitachiAc344MinTemp = kHitachiAc424MinTemp;
const uint8_t kHitachiAc344MaxTemp = kHitachiAc424MaxTemp;
const uint8_t kHitachiAc424FanTemp = 27;   // 27C

const uint8_t kHitachiAc424Fan = 1;
const uint8_t kHitachiAc424Cool = 3;
const uint8_t kHitachiAc424Dry = 5;
const uint8_t kHitachiAc424Heat = 6;
const uint8_t kHitachiAc344Fan = kHitachiAc424Fan;
const uint8_t kHitachiAc344Cool = kHitachiAc424Cool;
const uint8_t kHitachiAc344Dry = kHitachiAc424Dry;
const uint8_t kHitachiAc344Heat = kHitachiAc424Heat;

const uint8_t kHitachiAc424FanMin = 1;
const uint8_t kHitachiAc424FanLow = 2;
const uint8_t kHitachiAc424FanMedium = 3;
const uint8_t kHitachiAc424FanHigh = 4;
const uint8_t kHitachiAc424FanAuto = 5;
const uint8_t kHitachiAc424FanMax = 6;
const uint8_t kHitachiAc424FanMaxDry = 2;
const uint8_t kHitachiAc344FanMin = kHitachiAc424FanMin;
const uint8_t kHitachiAc344FanLow = kHitachiAc424FanLow;
const uint8_t kHitachiAc344FanMedium = kHitachiAc424FanMedium;
const uint8_t kHitachiAc344FanHigh = kHitachiAc424FanHigh;
const uint8_t kHitachiAc344FanAuto = kHitachiAc424FanAuto;
const uint8_t kHitachiAc344FanMax = kHitachiAc424FanMax;

const uint8_t kHitachiAc424PowerOn = 0xF1;
const uint8_t kHitachiAc424PowerOff = 0xE1;

const uint8_t kHitachiAc344SwingHAuto = 0;              // 0b000
const uint8_t kHitachiAc344SwingHRightMax = 1;          // 0b001
const uint8_t kHitachiAc344SwingHRight = 2;             // 0b010
const uint8_t kHitachiAc344SwingHMiddle = 3;            // 0b011
const uint8_t kHitachiAc344SwingHLeft = 4;              // 0b100
const uint8_t kHitachiAc344SwingHLeftMax = 5;           // 0b101


/// Native representation of a Hitachi 104-bit A/C message.
union Hitachi1Protocol{
    uint8_t raw[kHitachiAc1StateLength];  ///< The state in native code.
    struct {
        // Byte 0~2
        uint8_t pad[3];
        // Byte 3
        uint8_t        :6;
        uint8_t Model  :2;
        // Byte 4
        uint8_t        :8;
        // Byte 5
        uint8_t Fan    :4;
        uint8_t Mode   :4;
        // Byte 6
        uint8_t        :2;
        uint8_t Temp   :5;  // stored in LSB order.
        uint8_t        :1;
        // Byte 7
        uint8_t OffTimerLow :8;  // nr. of minutes
        // Byte 8
        uint8_t OffTimerHigh :8;  // & in LSB order.
        // Byte 9
        uint8_t OnTimerLow :8;  // nr. of minutes
        // Byte 10
        uint8_t OnTimerHigh :8;  // & in LSB order.
        // Byte 11
        uint8_t SwingToggle  :1;
        uint8_t Sleep        :3;
        uint8_t PowerToggle  :1;
        uint8_t Power        :1;
        uint8_t SwingV       :1;
        uint8_t SwingH       :1;
        // Byte 12
        uint8_t Sum :8;
    };
};
// HitachiAc1
// Model
const uint8_t kHitachiAc1Model_A =                 0b10;
const uint8_t kHitachiAc1Model_B =                 0b01;

// Mode & Fan
const uint8_t kHitachiAc1Dry =                  0b0010;  // 2
const uint8_t kHitachiAc1Fan =                  0b0100;  // 4
const uint8_t kHitachiAc1Cool =                 0b0110;  // 6
const uint8_t kHitachiAc1Heat =                 0b1001;  // 9
const uint8_t kHitachiAc1Auto =                 0b1110;  // 14
const uint8_t kHitachiAc1FanAuto = 1;          // 0b0001
const uint8_t kHitachiAc1FanHigh = 2;          // 0b0010
const uint8_t kHitachiAc1FanMed = 4;           // 0b0100
const uint8_t kHitachiAc1FanLow = 8;           // 0b1000

// Temp
const uint8_t kHitachiAc1TempSize = 5;  // Mask 0b01111100
const uint8_t kHitachiAc1TempDelta = 7;
const uint8_t kHitachiAc1TempAuto = 25;  // Celsius
// Timer
const uint8_t kHitachiAc1TimerSize = 16;  // Mask 0b1111111111111111
// Sleep
const uint8_t kHitachiAc1SleepOff =                          0b000;
const uint8_t kHitachiAc1Sleep1 =                            0b001;
const uint8_t kHitachiAc1Sleep2 =                            0b010;
const uint8_t kHitachiAc1Sleep3 =                            0b011;
const uint8_t kHitachiAc1Sleep4 =                            0b100;
// Checksum
const uint8_t kHitachiAc1ChecksumStartByte = 5;


/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***         Exported global functions                                     ***/
/****************************************************************************/
// Classes
/// Class for handling detailed Hitachi 224-bit A/C messages.
/// @see https://github.com/ToniA/arduino-heatpumpir/blob/master/HitachiHeatpumpIR.cpp
class IRHitachiAc {
public:
    IRHitachiAc(uint32_t frequency, uint16_t dutycycle);
    virtual ~IRHitachiAc();
    void sendHitachiAC(const uint16_t repeat = kHitachiAcDefaultRepeat);
    void send(const unsigned char data[],
              const uint16_t nbytes = kHitachiAcStateLength,
              const uint16_t repeat = kHitachiAcDefaultRepeat);
private:
    void stateReset(void);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
//    int8_t calibrate(void) { return _irsend.calibrate(); }
    void on(void);
    void off(void);
    void setPower(const bool on);
    bool getPower(void) const;
    void setTemp(const uint8_t temp);
    uint8_t getTemp(void) const;
    void setFan(const uint8_t speed);
    uint8_t getFan(void) const;
    void setMode(const uint8_t mode);
    uint8_t getMode(void) const;
    void setSwingVertical(const bool on);
    bool getSwingVertical(void) const;
    void setSwingHorizontal(const bool on);
    bool getSwingHorizontal(void) const;
    uint8_t* getRaw(void);
    void setRaw(const uint8_t new_code[],
                const uint16_t length = kHitachiAcStateLength);
    static bool validChecksum(const uint8_t state[],
                              const uint16_t length = kHitachiAcStateLength);
    static uint8_t calcChecksum(const uint8_t state[],
                                const uint16_t length = kHitachiAcStateLength);

    Ir_Send _ir_hitachiAC;
    HitachiProtocol _;
    void checksum(const uint16_t length = kHitachiAcStateLength);
    uint8_t _previoustemp;
};

class IRHitachiAc2{
public:
	IRHitachiAc2();
    virtual ~IRHitachiAc2();
    void sendHitachiAc2(const unsigned char data[],
                        const uint16_t nbytes = kHitachiAc2StateLength,
                        const uint16_t repeat = kHitachiAcDefaultRepeat);
private:
};
/// Class for handling detailed Hitachi 104-bit A/C messages.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1056
class IRHitachiAc1 {
public:
    IRHitachiAc1(uint32_t frequency, uint16_t dutycycle);
    virtual ~IRHitachiAc1();
    void sendHitachiAc1(const uint16_t repeat = kHitachiAcDefaultRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
    void setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp);
private:
    uint8_t convertMode(const stdAc::opmode_t mode);
    uint8_t convertFan(const stdAc::fanspeed_t speed);

    void send(const unsigned char data[],
              const uint16_t nbytes = kHitachiAc1StateLength,
              const uint16_t repeat = kHitachiAcDefaultRepeat);
    void on(void);
    void off(void);
    void setModel(const hitachi_ac1_remote_model_t model);
    hitachi_ac1_remote_model_t getModel(void) const;
    void setPower(const bool on);
    bool getPower(void) const;
    void setPowerToggle(const bool on);
    bool getPowerToggle(void) const;
    void setTemp(const uint8_t temp);
    uint8_t getTemp(void) const;
    void setFan(const uint8_t speed, const bool force = false);
    uint8_t getFan(void) const;
    void setMode(const uint8_t mode);
    uint8_t getMode(void) const;
    void setSwingToggle(const bool toggle);
    bool getSwingToggle(void) const;
    void setSwingV(const bool on);
    bool getSwingV(void) const;
    void setSwingH(const bool on);
    bool getSwingH(void) const;
    void setSleep(const uint8_t mode);
    uint8_t getSleep(void) const;
    void setOnTimer(const uint16_t mins);
    uint16_t getOnTimer(void) const;
    void setOffTimer(const uint16_t mins);
    uint16_t getOffTimer(void) const;
    uint8_t* getRaw(void);
    void setRaw(const uint8_t new_code[],
                const uint16_t length = kHitachiAc1StateLength);
    static bool validChecksum(const uint8_t state[],
                              const uint16_t length = kHitachiAc1StateLength);
    static uint8_t calcChecksum(const uint8_t state[],
                                const uint16_t length = kHitachiAc1StateLength);
    Ir_Send _ir_hitachiAC1;  ///< Instance of the IR send class
    Hitachi1Protocol _;
    void stateReset(void);
    void checksum(const uint16_t length = kHitachiAc1StateLength);
};
/// Class for handling detailed Hitachi 53-byte/424-bit A/C messages.
class IRHitachiAc424 {
    friend class IRHitachiAc344;
public:
    explicit IRHitachiAc424(uint32_t frequency, uint16_t dutycycle);
    virtual ~IRHitachiAc424();
    void sendHitachi_Ac424(const uint16_t repeat = kHitachiAcDefaultRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all
    virtual void stateReset(void);
    virtual void setRaw(const uint8_t new_code[],
                        const uint16_t length = kHitachiAc424StateLength);

    Ir_Send _ir_hitachiAC424;  ///< Instance of the IR send class
private:
    void send(const unsigned char data[],
              const uint16_t nbytes = kHitachiAc424StateLength,
              const uint16_t repeat = kHitachiAcDefaultRepeat);

    void on(void);
    void off(void);
    void setPower(const bool on);
    bool getPower(void) const;
    void setTemp(const uint8_t temp, bool setPrevious = true);
    uint8_t getTemp(void) const;
    void setFan(const uint8_t speed);
    uint8_t getFan(void) const;
    uint8_t getButton(void) const;
    void setButton(const uint8_t button);
    void setSwingVToggle(const bool on);
    bool getSwingVToggle(void) const;
    void setMode(const uint8_t mode);
    uint8_t getMode(void) const;
    uint8_t* getRaw(void);



    Hitachi424Protocol _;
    void setInvertedStates(void);
    uint8_t _previoustemp;
};
/// Class for handling detailed Hitachi 15to27-byte/120to216-bit A/C messages.
class IRHitachiAc3 {
public:
    IRHitachiAc3(uint32_t frequency, uint16_t dutycycle);
    ~IRHitachiAc3();

    void sendHitachiAc3(const uint16_t repeat = kHitachiAcDefaultRepeat);
private:
    void stateReset(void);
    void send(const unsigned char data[],
            const uint16_t nbytes,  // No default as there as so many
                                    // different sizes
            const uint16_t repeat = kHitachiAcDefaultRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.

    uint8_t getMode(void);
    uint8_t* getRaw(void);
    void setRaw(const uint8_t new_code[],
            const uint16_t length = kHitachiAc3StateLength);
    static bool hasInvertedStates(const uint8_t state[], const uint16_t length);

    Ir_Send _ir_hitachiAC3;
    uint8_t remote_state[kHitachiAc3StateLength];  ///< The state in native code.
    void setInvertedStates(const uint16_t length = kHitachiAc3StateLength);
};

/// Class for handling detailed Hitachi 344-bit A/C messages.
class IRHitachiAc344: public IRHitachiAc424 {
public:
	explicit IRHitachiAc344(uint32_t frequency, uint16_t dutycycle);
    virtual ~IRHitachiAc344();
    void sendHitachiAc344(const uint16_t repeat = kHitachiAcDefaultRepeat);
    void stateReset(void) override;
    void setRaw(const uint8_t new_code[],
                const uint16_t length = kHitachiAc344StateLength) override;
private:
    void send(const unsigned char data[],
            const uint16_t nbytes = kHitachiAc344StateLength,
            const uint16_t repeat = kHitachiAcDefaultRepeat);

    void setSwingV(const bool on);
    bool getSwingV(void) const;
    void setSwingH(const uint8_t position);
    uint8_t getSwingH(void) const;
    Ir_Send _ir_HitachiAc344;
};
#endif /* MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRHITACHI_H_ */

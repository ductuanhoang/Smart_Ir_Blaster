/*
 * IrDaikin.h
 *
 *  Created on: Jan 15, 2021
 *      Author: ductu
 */

#ifndef MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRDAIKIN_H_
#define MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRDAIKIN_H_

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "IrSend.h"
#include "IRutils.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/


/// Native representation of a Daikin A/C message.
union DaikinESPProtocol{
    uint8_t raw[kDaikinStateLength];  ///< The state of the IR remote.
    struct {
        // Byte 0~5
        uint64_t          :48;
        // Byte 6
        uint64_t          :4;
        uint64_t Comfort  :1;
        uint64_t          :3;
        // Byte 7
        uint64_t Sum1     :8;  // checksum of the first part

        // Byte 8~12
        uint64_t              :40;
        // Byte 13~14
        uint64_t CurrentTime  :11;  // Current time, mins past midnight
        uint64_t CurrentDay   :3;  // Day of the week (SUN=1, MON=2, ..., SAT=7)
        uint64_t              :2;
        // Byte 15
        uint64_t Sum2         :8;  // checksum of the second part

        // Byte 16~20
        uint64_t          :40;
        // Byte 21
        uint64_t Power    :1;
        uint64_t OnTimer  :1;
        uint64_t OffTimer :1;
        uint64_t          :1;  // always 1
        uint64_t Mode     :3;
        uint64_t          :1;
        // Byte 22
        uint64_t          :1;
        uint64_t Temp     :7;  // Temp should be between 10 - 32
        // Byte 23
        uint64_t          :8;

        // Byte 24
        uint64_t SwingV   :4;  // 0000 =  off, 1111 = on
        uint64_t Fan      :4;
        // Byte 25
        uint64_t SwingH   :4;  // 0000 =  off, 1111 = on
        uint64_t          :4;
        // Byte 26~28
        uint64_t OnTime   :12;  // timer mins past midnight
        uint64_t OffTime  :12;  // timer mins past midnight
        // Byte 29
        uint64_t Powerful :1;
        uint64_t          :4;
        uint64_t Quiet    :1;
        uint64_t          :2;
        // Byte 30~31
        uint64_t          :0;

        // Byte 32
        uint8_t             :1;
        uint8_t Sensor      :1;
        uint8_t Econo       :1;
        uint8_t             :4;
        uint8_t WeeklyTimer :1;
        // Byte 33
        uint8_t       :1;
        uint8_t Mold  :1;
        uint8_t       :6;
        // Byte 34
        uint8_t Sum3  :8;  // checksum of the third part
    };
};

// Constants
const uint8_t kDaikinAuto = 0b000;  // temp 25
const uint8_t kDaikinDry =  0b010;  // temp 0xc0 = 96 degrees c
const uint8_t kDaikinCool = 0b011;
const uint8_t kDaikinHeat = 0b100;  // temp 23
const uint8_t kDaikinFan =  0b110;  // temp not shown, but 25
const uint8_t kDaikinMinTemp = 10;  // Celsius
const uint8_t kDaikinMaxTemp = 32;  // Celsius
const uint8_t kDaikinFanMin = 1;
const uint8_t kDaikinFanMed = 3;
const uint8_t kDaikinFanMax = 5;
const uint8_t kDaikinFanAuto = 0b1010;  // 10 / 0xA
const uint8_t kDaikinFanQuiet = 0b1011;  // 11 / 0xB
const uint8_t kDaikinSwingOn =  0b1111;
const uint8_t kDaikinSwingOff = 0b0000;
const uint16_t kDaikinHeaderLength = 5;
const uint8_t kDaikinSections = 3;
const uint8_t kDaikinSection1Length = 8;
const uint8_t kDaikinSection2Length = 8;
const uint8_t kDaikinSection3Length =
        kDaikinStateLength - kDaikinSection1Length - kDaikinSection2Length;
const uint8_t kDaikinByteChecksum1 = 7;
const uint8_t kDaikinByteChecksum2 = 15;
// const uint8_t kDaikinBitEye = 0b10000000;
const uint16_t kDaikinUnusedTime = 0x600;
const uint8_t kDaikinBeepQuiet = 1;
const uint8_t kDaikinBeepLoud = 2;
const uint8_t kDaikinBeepOff = 3;
const uint8_t kDaikinLightBright = 1;
const uint8_t kDaikinLightDim = 2;
const uint8_t kDaikinLightOff = 3;
const uint8_t kDaikinCurBit = kDaikinStateLength;
const uint8_t kDaikinCurIndex = kDaikinStateLength + 1;
const uint8_t kDaikinTolerance = 35;
const uint16_t kDaikinMarkExcess = kMarkExcess;
const uint16_t kDaikinHdrMark = 3650;   // kDaikinBitMark * 8
const uint16_t kDaikinHdrSpace = 1623;  // kDaikinBitMark * 4
const uint16_t kDaikinBitMark = 428;
const uint16_t kDaikinZeroSpace = 428;
const uint16_t kDaikinOneSpace = 1280;
const uint16_t kDaikinGap = 29000;
// Note bits in each octet swapped so can be sent as a single value
const uint64_t kDaikinFirstHeader64 =
        0b1101011100000000000000001100010100000000001001111101101000010001;

/// Native representation of a Daikin2 A/C message.
union Daikin2Protocol{
    struct{
        uint8_t pad[3];
        uint8_t raw[kDaikin2StateLength];  ///< The state of the IR remote.
    };
    struct {
        // Byte -3~4
        uint64_t :64;

        // Byte 5~6
        uint64_t CurrentTime  :12;
        uint64_t              :3;
        uint64_t Power2       :1;
        // Byte 7
        uint64_t              :4;
        uint64_t Light        :2;
        uint64_t Beep         :2;
        // Byte 8
        uint64_t FreshAir     :1;
        uint64_t              :2;
        uint64_t Mold         :1;
        uint64_t              :1;
        uint64_t Clean        :1;
        uint64_t              :1;
        uint64_t FreshAirHigh :1;
        // Byte 9~12
        uint64_t              :32;

        // Byte 13
        uint64_t          :7;
        uint64_t EyeAuto  :1;
        // Byte 14~16
        uint64_t          :24;
        // Byte 17
        uint64_t SwingH   :8;
        // Byte 18
        uint64_t SwingV   :4;
        uint64_t          :4;
        // Byte 19
        uint64_t Sum1     :8;
        // Byte 20
        uint64_t          :8;

        // Byte 21~24
        uint64_t          :32;
        // Byte 25
        uint64_t Power    :1;
        uint64_t OnTimer  :1;
        uint64_t OffTimer :1;
        uint64_t          :1;
        uint64_t Mode     :3;
        uint64_t          :1;
        // Byte 26
        uint64_t          :1;
        uint64_t Temp     :7;
        // Byte 27
        uint64_t          :8;
        // Byte 28
        uint64_t          :4;
        uint64_t Fan      :4;

        // Byte 29
        uint64_t            :8;
        // Byte 30~32
        /// @see https://github.com/crankyoldgit/IRremoteESP8266/pull/1264
        uint64_t OnTime     :12;
        uint64_t OffTime    :12;
        // Byte 33
        uint64_t Powerful   :1;
        uint64_t            :4;
        uint64_t Quiet      :1;
        uint64_t            :2;
        // Byte 34~35
        uint64_t            :16;
        // Byte 36
        uint64_t            :1;
        uint64_t Eye        :1;
        uint64_t Econo      :1;
        uint64_t            :1;
        uint64_t Purify     :1;
        uint64_t SleepTimer :1;
        uint64_t            :2;

        // Byte 37
        uint8_t       :8;
        // Byte 38
        uint8_t Sum2  :8;
    };
};

const uint16_t kDaikin2Freq = 36700;  // Modulation Frequency in Hz.
const uint16_t kDaikin2LeaderMark = 10024;
const uint16_t kDaikin2LeaderSpace = 25180;
const uint16_t kDaikin2Gap = kDaikin2LeaderMark + kDaikin2LeaderSpace;
const uint16_t kDaikin2HdrMark = 3500;
const uint16_t kDaikin2HdrSpace = 1728;
const uint16_t kDaikin2BitMark = 460;
const uint16_t kDaikin2OneSpace = 1270;
const uint16_t kDaikin2ZeroSpace = 420;
const uint16_t kDaikin2Sections = 2;
const uint16_t kDaikin2Section1Length = 20;
const uint16_t kDaikin2Section2Length = 19;
const uint8_t kDaikin2Tolerance = 5;  // Extra percentage tolerance
const uint8_t kDaikin2SwingVHigh = 0x1;
const uint8_t kDaikin2SwingVLow = 0x6;
const uint8_t kDaikin2SwingVSwing = 0xF;
const uint8_t kDaikin2SwingVAuto = 0xE;
const uint8_t kDaikin2SwingVBreeze = 0xC;
const uint8_t kDaikin2SwingVCirculate = 0xD;

const uint8_t kDaikin2SwingHWide =     0xA3;
const uint8_t kDaikin2SwingHLeftMax =  0xA8;
const uint8_t kDaikin2SwingHLeft =     0xA9;
const uint8_t kDaikin2SwingHMiddle =   0xAA;
const uint8_t kDaikin2SwingHRight =    0xAB;
const uint8_t kDaikin2SwingHRightMax = 0xAC;
const uint8_t kDaikin2SwingHAuto =     0xBE;
const uint8_t kDaikin2SwingHSwing =    0xBF;

const uint8_t kDaikin2MinCoolTemp = 18;  // Min temp (in C) when in Cool mode.

/// Native representation of a Daikin216 A/C message.
union Daikin216Protocol{
    uint8_t raw[kDaikin216StateLength];  ///< The state of the IR remote.
    struct {
        // Byte 0~6
        uint8_t pad0[7];
        // Byte 7
        uint8_t Sum1  :8;
        // Byte 8~12
        uint8_t pad1[5];
        // Byte 13
        uint8_t Power :1;
        uint8_t       :3;
        uint8_t Mode  :3;
        uint8_t       :1;
        // Byte 14
        uint8_t         :1;
        uint8_t Temp    :6;
        uint8_t         :1;
        // Byte 15
        uint8_t         :8;
        // Byte 16
        uint8_t SwingV  :4;
        uint8_t Fan     :4;
        // Byte 17
        uint8_t SwingH  :4;
        uint8_t         :4;
        // Byte 18~20
        uint8_t pad2[3];
        // Byte 21
        uint8_t Powerful  :1;
        uint8_t           :0;
        // Byte 22~25
        uint8_t pad3[4];
        // Byte 26
        uint8_t Sum2      :8;
    };
};

const uint16_t kDaikin216Freq = 38000;  // Modulation Frequency in Hz.
const uint16_t kDaikin216HdrMark = 3440;
const uint16_t kDaikin216HdrSpace = 1750;
const uint16_t kDaikin216BitMark = 420;
const uint16_t kDaikin216OneSpace = 1300;
const uint16_t kDaikin216ZeroSpace = 450;
const uint16_t kDaikin216Gap = 29650;
const uint16_t kDaikin216Sections = 2;
const uint16_t kDaikin216Section1Length = 8;
const uint16_t kDaikin216Section2Length = kDaikin216StateLength -
                                          kDaikin216Section1Length;

const uint8_t kDaikin216SwingOn = 0b1111;
const uint8_t kDaikin216SwingOff = 0b0000;

/// Native representation of a Daikin160 A/C message.
union Daikin160Protocol{
    uint8_t raw[kDaikin160StateLength];  ///< The state of the IR remote.
    struct {
        // Byte 0~5
        uint8_t pad0[6];
        // Byte 6
        uint8_t Sum1 :8;
        // Byte 7~11
        uint8_t pad1[5];
        // Byte 12
        uint8_t Power   :1;
        uint8_t         :3;
        uint8_t Mode    :3;
        uint8_t         :1;
        // Byte 13
        uint8_t         :4;
        uint8_t SwingV  :4;
        // Byte 14~15
        uint8_t pad2[2];
        // Byte 16
        uint8_t       :1;
        uint8_t Temp  :6;
        uint8_t       :1;
        // Byte 17
        uint8_t Fan   :4;
        uint8_t       :4;
        // Byte 18
        uint8_t       :8;
        // Byte 19
        uint8_t Sum2  :8;
    };
};

const uint16_t kDaikin160Freq = 38000;  // Modulation Frequency in Hz.
const uint16_t kDaikin160HdrMark = 5000;
const uint16_t kDaikin160HdrSpace = 2145;
const uint16_t kDaikin160BitMark = 342;
const uint16_t kDaikin160OneSpace = 1786;
const uint16_t kDaikin160ZeroSpace = 700;
const uint16_t kDaikin160Gap = 29650;
const uint16_t kDaikin160Sections = 2;
const uint16_t kDaikin160Section1Length = 7;
const uint16_t kDaikin160Section2Length = kDaikin160StateLength -
                                          kDaikin160Section1Length;
const uint8_t kDaikin160SwingVLowest =  0x1;
const uint8_t kDaikin160SwingVLow =     0x2;
const uint8_t kDaikin160SwingVMiddle =  0x3;
const uint8_t kDaikin160SwingVHigh =    0x4;
const uint8_t kDaikin160SwingVHighest = 0x5;
const uint8_t kDaikin160SwingVAuto =    0xF;

/// Native representation of a Daikin176 A/C message.
union Daikin176Protocol{
    uint8_t raw[kDaikin176StateLength];  ///< The state of the IR remote.
    struct {
        // Byte 0~5
        uint8_t pad0[6];
        // Byte 6
        uint8_t Sum1 :8;
        // Byte 7~11
        uint8_t pad1[5];
        // Byte 12
        uint8_t         :4;
        uint8_t AltMode :3;
        uint8_t         :1;
        // Byte 13
        uint8_t ModeButton  :8;
        // Byte 14
        uint8_t Power :1;
        uint8_t       :3;
        uint8_t Mode  :3;
        uint8_t       :1;
        // Byte 15~16
        uint8_t pad2[2];
        // Byte 17
        uint8_t       :1;
        uint8_t Temp  :6;
        uint8_t       :1;
        // Byte 18
        uint8_t SwingH  :4;
        uint8_t Fan     :4;
        // Byte 19~20
        uint8_t pad3[2];
        // Byte 21
        uint8_t Sum2  :8;
    };
};

const uint16_t kDaikin176Freq = 38000;  // Modulation Frequency in Hz.
const uint16_t kDaikin176HdrMark = 5070;
const uint16_t kDaikin176HdrSpace = 2140;
const uint16_t kDaikin176BitMark = 370;
const uint16_t kDaikin176OneSpace = 1780;
const uint16_t kDaikin176ZeroSpace = 710;
const uint16_t kDaikin176Gap = 29410;
const uint16_t kDaikin176Sections = 2;
const uint16_t kDaikin176Section1Length = 7;
const uint16_t kDaikin176Section2Length = kDaikin176StateLength -
                                          kDaikin176Section1Length;
const uint8_t kDaikin176Fan =  0b000;  // 0
const uint8_t kDaikin176Heat = 0b001;  // 1
const uint8_t kDaikin176Cool = 0b010;  // 2
const uint8_t kDaikin176Auto = 0b011;  // 3
const uint8_t kDaikin176Dry =  0b111;  // 7
const uint8_t kDaikin176ModeButton = 0b00000100;
const uint8_t kDaikin176DryFanTemp = 17;  // Dry/Fan mode is always 17 Celsius.
const uint8_t kDaikin176FanMax = 3;
const uint8_t kDaikin176SwingHAuto =  0x5;
const uint8_t kDaikin176SwingHOff = 0x6;

/// Native representation of a Daikin128 A/C message.
union Daikin128Protocol{
    uint8_t raw[kDaikin128StateLength];  ///< The state of the IR remote.
    struct {
        // Byte 0
        uint8_t       :8;
        // Byte 1
        uint8_t Mode  :4;
        uint8_t Fan   :4;
        // Byte 2
        uint8_t ClockMins   :8;
        // Byte 3
        uint8_t ClockHours  :8;
        // Byte 4
        uint8_t OnHours     :6;
        uint8_t OnHalfHour  :1;
        uint8_t OnTimer     :1;
        // Byte 5
        uint8_t OffHours    :6;
        uint8_t OffHalfHour :1;
        uint8_t OffTimer    :1;
        // Byte 6
        uint8_t Temp    :8;
        // Byte 7
        uint8_t SwingV  :1;
        uint8_t Sleep   :1;
        uint8_t         :1;  // always 1
        uint8_t Power   :1;
        uint8_t Sum1    :4;
        // Byte 8
        uint8_t         :8;
        // Byte 9
        uint8_t Ceiling :1;
        uint8_t         :1;
        uint8_t Econo   :1;
        uint8_t Wall    :1;
        uint8_t         :4;
        // Byte 10~14
        uint8_t pad[5];
        // Byte 15
        uint8_t Sum2    :8;
    };
};

const uint16_t kDaikin128Freq = 38000;  // Modulation Frequency in Hz.
const uint16_t kDaikin128LeaderMark = 9800;
const uint16_t kDaikin128LeaderSpace = 9800;
const uint16_t kDaikin128HdrMark = 4600;
const uint16_t kDaikin128HdrSpace = 2500;
const uint16_t kDaikin128BitMark = 350;
const uint16_t kDaikin128OneSpace = 954;
const uint16_t kDaikin128ZeroSpace = 382;
const uint16_t kDaikin128Gap = 20300;
const uint16_t kDaikin128FooterMark = kDaikin128HdrMark;
const uint16_t kDaikin128Sections = 2;
const uint16_t kDaikin128SectionLength = 8;
const uint8_t kDaikin128Dry =             0b00000001;
const uint8_t kDaikin128Cool =            0b00000010;
const uint8_t kDaikin128Fan =             0b00000100;
const uint8_t kDaikin128Heat =            0b00001000;
const uint8_t kDaikin128Auto =            0b00001010;
const uint8_t kDaikin128FanAuto =         0b0001;
const uint8_t kDaikin128FanHigh =         0b0010;
const uint8_t kDaikin128FanMed =          0b0100;
const uint8_t kDaikin128FanLow =          0b1000;
const uint8_t kDaikin128FanPowerful =     0b0011;
const uint8_t kDaikin128FanQuiet =        0b1001;
const uint8_t kDaikin128MinTemp = 16;  // C
const uint8_t kDaikin128MaxTemp = 30;  // C
const uint8_t kDaikin128BitWall =         0b00001000;
const uint8_t kDaikin128BitCeiling =      0b00000001;

/// Native representation of a Daikin152 A/C message.
union Daikin152Protocol{
    uint8_t raw[kDaikin152StateLength];  ///< The state of the IR remote.
    struct {
        // Byte 0~4
        uint8_t pad0[5];
        // Byte 5
        uint8_t Power :1;
        uint8_t       :3;
        uint8_t Mode  :3;
        uint8_t       :1;
        // Byte 6
        uint8_t       :1;
        uint8_t Temp  :7;
        // Byte 7
        uint8_t :8;
        // Byte 8
        uint8_t SwingV  :4;
        uint8_t Fan     :4;
        // Byte 9~12
        uint8_t pad1[4];
        // Byte 13
        uint8_t Powerful  :1;
        uint8_t           :4;
        uint8_t Quiet     :1;
        uint8_t           :2;
        // Byte 14~15
        uint8_t pad2[2];
        // Byte 16
        uint8_t         :1;
        uint8_t Comfort :1;
        uint8_t Econo   :1;
        uint8_t Sensor  :1;
        uint8_t         :4;
        // Byte 17
        uint8_t     :8;
        // Byte 18
        uint8_t Sum :8;
    };
};

const uint16_t kDaikin152Freq = 38000;  // Modulation Frequency in Hz.
const uint8_t  kDaikin152LeaderBits = 5;
const uint16_t kDaikin152HdrMark = 3492;
const uint16_t kDaikin152HdrSpace = 1718;
const uint16_t kDaikin152BitMark = 433;
const uint16_t kDaikin152OneSpace = 1529;
const uint16_t kDaikin152ZeroSpace = kDaikin152BitMark;
const uint16_t kDaikin152Gap = 25182;

const uint8_t kDaikin152DryTemp = kDaikin2MinCoolTemp;  // Celsius
const uint8_t kDaikin152FanTemp = 0x60;  // 96 Celsius

/// Native representation of a Daikin64 A/C message.
union Daikin64Protocol{
    uint64_t raw;  ///< The state of the IR remote.
    struct {
        uint8_t             :8;
        uint8_t Mode        :4;
        uint8_t Fan         :4;
        uint8_t ClockMins   :8;
        uint8_t ClockHours  :8;
        uint8_t OnHours     :6;
        uint8_t OnHalfHour  :1;
        uint8_t OnTimer     :1;
        uint8_t OffHours    :6;
        uint8_t OffHalfHour :1;
        uint8_t OffTimer    :1;
        uint8_t Temp        :8;
        uint8_t SwingV      :1;
        uint8_t Sleep       :1;
        uint8_t             :1;
        uint8_t Power       :1;
        uint8_t Sum         :4;
    };
};

const uint16_t kDaikin64HdrMark = kDaikin128HdrMark;
const uint16_t kDaikin64BitMark = kDaikin128BitMark;
const uint16_t kDaikin64HdrSpace = kDaikin128HdrSpace;
const uint16_t kDaikin64OneSpace = kDaikin128OneSpace;
const uint16_t kDaikin64ZeroSpace = kDaikin128ZeroSpace;
const uint16_t kDaikin64LdrMark = kDaikin128LeaderMark;
const uint16_t kDaikin64Gap = kDaikin128Gap;
const uint16_t kDaikin64LdrSpace = kDaikin128LeaderSpace;
const uint16_t kDaikin64Freq = kDaikin128Freq;  // Hz.
const uint8_t kDaikin64Overhead = 9;
const int8_t  kDaikin64ToleranceDelta = 5;  // +5%

const uint64_t kDaikin64KnownGoodState = 0x7C16161607204216;
const uint8_t kDaikin64Dry =  0b001;
const uint8_t kDaikin64Cool = 0b010;
const uint8_t kDaikin64Fan =  0b100;
const uint8_t kDaikin64FanAuto =  0b0001;
const uint8_t kDaikin64FanLow =   0b1000;
const uint8_t kDaikin64FanMed =   0b0100;
const uint8_t kDaikin64FanHigh =  0b0010;
const uint8_t kDaikin64FanQuiet = 0b1001;
const uint8_t kDaikin64FanTurbo = 0b0011;
const uint8_t kDaikin64MinTemp = 16;  // Celsius
const uint8_t kDaikin64MaxTemp = 30;  // Celsius
const uint8_t kDaikin64ChecksumOffset = 60;
const uint8_t kDaikin64ChecksumSize = 4;  // Mask 0b1111 << 59

// Legacy defines.
#define DAIKIN_COOL kDaikinCool
#define DAIKIN_HEAT kDaikinHeat
#define DAIKIN_FAN kDaikinFan
#define DAIKIN_AUTO kDaikinAuto
#define DAIKIN_DRY kDaikinDry
#define DAIKIN_MIN_TEMP kDaikinMinTemp
#define DAIKIN_MAX_TEMP kDaikinMaxTemp
#define DAIKIN_FAN_MIN kDaikinFanMin
#define DAIKIN_FAN_MAX kDaikinFanMax
#define DAIKIN_FAN_AUTO kDaikinFanAuto
#define DAIKIN_FAN_QUIET kDaikinFanQuiet

/****************************************************************************/
/***         Exported global functions                                     ***/
/****************************************************************************/

/*
 * Ir daikin esp class
 */
class Ir_DaikinESP {
public:
    Ir_DaikinESP(uint32_t frequency, uint16_t dutycycle);
    virtual ~Ir_DaikinESP();
    void sendDaikinESP(const uint16_t repeat = kDaikinDefaultRepeat);
    void setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp);
private:
    uint8_t convertMode(const stdAc::opmode_t mode);
    uint8_t convertFan(const stdAc::fanspeed_t speed);

    void send_esp(const unsigned char data[],
            const uint16_t nbytes = kDaikinStateLength,
            const uint16_t repeat = kDaikinDefaultRepeat);
    void on(void);
    void off(void);
    void setPower(const bool on);
    bool getPower(void) const;
    void setTemp(const uint8_t temp);
    uint8_t getTemp(void);
    void setFan(const uint8_t fan);
    uint8_t getFan(void);
    void setMode(const uint8_t mode);
    uint8_t getMode(void);
    void setSwingVertical(const bool on);
    bool getSwingVertical(void);
    void setSwingHorizontal(const bool on);
    bool getSwingHorizontal(void);
    bool getQuiet(void) ;
    void setQuiet(const bool on);
    bool getPowerful(void);
    void setPowerful(const bool on);
    void setSensor(const bool on);
    bool getSensor(void);
    void setEcono(const bool on);
    bool getEcono(void);
    void setMold(const bool on);
    bool getMold(void);
    void setComfort(const bool on);
    bool getComfort(void);
    void enableOnTimer(const uint16_t starttime);
    void disableOnTimer(void);
    uint16_t getOnTime(void);
    bool getOnTimerEnabled(void);
    void enableOffTimer(const uint16_t endtime);
    void disableOffTimer(void);
    uint16_t getOffTime(void);
    bool getOffTimerEnabled(void);
    void setCurrentTime(const uint16_t mins_since_midnight);
    uint16_t getCurrentTime(void);
    void setCurrentDay(const uint8_t day_of_week);
    uint8_t getCurrentDay(void);
    void setWeeklyTimerEnable(const bool on);
    bool getWeeklyTimerEnable(void);
    uint8_t* getRaw(void);
    void setRaw(const uint8_t new_code[],
                const uint16_t length = kDaikinStateLength);
    static bool validChecksum(uint8_t state[],
                              const uint16_t length = kDaikinStateLength);

    void stateReset(void);
    void checksum(void);

    DaikinESPProtocol _;
    Ir_Send _ir_daikinESP_send;
};
/*
 * Ir daikin 2 class
 */
class Ir_Daikin2 {
public:
    Ir_Daikin2(uint32_t frequency, uint16_t dutycycle);
    virtual ~Ir_Daikin2();
    void sendDaikin2(const uint16_t repeat = kDaikin2DefaultRepeat);
    void setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp);
private:

    uint8_t convertMode(const stdAc::opmode_t mode);
    uint8_t convertFan(const stdAc::fanspeed_t speed);
    void send(const unsigned char data[],
              const uint16_t nbytes = kDaikin2StateLength,
              const uint16_t repeat = kDaikin2DefaultRepeat);

    void on(void);
    void off(void);
    void setPower(const bool on);
    bool getPower(void) const;
    void setTemp(const uint8_t desired);
    uint8_t getTemp(void) const;
    void setFan(const uint8_t fan);
    uint8_t getFan(void) const;
    uint8_t getMode(void) const;
    void setMode(const uint8_t desired_mode);
    void setSwingVertical(const uint8_t position);
    uint8_t getSwingVertical(void) const;
    void setSwingHorizontal(const uint8_t position);
    uint8_t getSwingHorizontal(void) const;
    bool getQuiet(void) const;
    void setQuiet(const bool on);
    bool getPowerful(void) const;
    void setPowerful(const bool on);
    void setEcono(const bool on);
    bool getEcono(void) const;
    void setEye(const bool on);
    bool getEye(void) const;
    void setEyeAuto(const bool on);
    bool getEyeAuto(void) const;
    void setPurify(const bool on);
    bool getPurify(void) const;
    void setMold(const bool on);
    bool getMold(void) const;
    void enableOnTimer(const uint16_t starttime);
    void disableOnTimer(void);
    uint16_t getOnTime(void) const;
    bool getOnTimerEnabled(void) const;
    void enableSleepTimer(const uint16_t sleeptime);
    void disableSleepTimer(void);
    uint16_t getSleepTime(void) const;
    bool getSleepTimerEnabled(void) const;
    void enableOffTimer(const uint16_t endtime);
    void disableOffTimer(void);
    uint16_t getOffTime(void) const;
    bool getOffTimerEnabled(void) const;
    void setCurrentTime(const uint16_t numMins);
    uint16_t getCurrentTime(void) const;
    void setBeep(const uint8_t beep);
    uint8_t getBeep(void) const;
    void setLight(const uint8_t light);
    uint8_t getLight(void) const;
    void setClean(const bool on);
    bool getClean(void) const;
    void setFreshAir(const bool on);
    bool getFreshAir(void) const;
    void setFreshAirHigh(const bool on);
    bool getFreshAirHigh(void) const;
    uint8_t* getRaw(void);
    void setRaw(const uint8_t new_code[]);
    static bool validChecksum(uint8_t state[],
                              const uint16_t length = kDaikin2StateLength);

    // # of bytes per command
    Daikin2Protocol _;
    void stateReset(void);
    void checksum(void);
    void clearOnTimerFlag(void);
    void clearSleepTimerFlag(void);
    Ir_Send _ir_daikin2;
};

/*
 * Ir daikin216 class
 */
class Ir_Daikin216 {
public:
    Ir_Daikin216(uint32_t frequency, uint16_t dutycycle);
    virtual ~Ir_Daikin216();
    void sendDaikin216(const uint16_t repeat = kDaikin216DefaultRepeat);
    void setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp);
private:
    uint8_t convertMode(const stdAc::opmode_t mode) ;
    uint8_t convertFan(const stdAc::fanspeed_t speed);

    void send(const unsigned char data[],
              const uint16_t nbytes = kDaikin216StateLength,
              const uint16_t repeat = kDaikin216DefaultRepeat);

    uint8_t* getRaw(void);
    void setRaw(const uint8_t new_code[]);
    static bool validChecksum(uint8_t state[],
                              const uint16_t length = kDaikin216StateLength);
    void on(void);
    void off(void);
    void setPower(const bool on);
    bool getPower(void) const;
    void setTemp(const uint8_t temp);
    uint8_t getTemp(void) const;
    void setMode(const uint8_t mode);
    uint8_t getMode(void) const;
    void setFan(const uint8_t fan);
    uint8_t getFan(void) const;
    void setSwingVertical(const bool on);
    bool getSwingVertical(void) const;
    void setSwingHorizontal(const bool on);
    bool getSwingHorizontal(void) const;
    void setQuiet(const bool on);
    bool getQuiet(void) const;
    void setPowerful(const bool on);
    bool getPowerful(void) const;

    // # of bytes per command
    Daikin216Protocol _;
    void stateReset(void);
    void checksum(void);

    Ir_Send _ir_daikin216;
};
/*
 * Ir daikin 160 class
 */
class Ir_Daikin160 {
public:
    Ir_Daikin160(uint32_t frequency, uint16_t dutycycle);
    virtual ~Ir_Daikin160();
    void sendDaikin160(const uint16_t repeat = kDaikin160DefaultRepeat);
    void setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp);
private:
    uint8_t convertMode(const stdAc::opmode_t mode);
    uint8_t convertFan(const stdAc::fanspeed_t speed);
    void send(const unsigned char data[],
              const uint16_t nbytes = kDaikin160StateLength,
              const uint16_t repeat = kDaikin160DefaultRepeat);
    uint8_t* getRaw(void);
    void setRaw(const uint8_t new_code[]);
    static bool validChecksum(uint8_t state[],
                              const uint16_t length = kDaikin160StateLength);
    void on(void);
    void off(void);
    void setPower(const bool on);
    bool getPower(void) const;
    void setTemp(const uint8_t temp);
    uint8_t getTemp(void) const;
    void setMode(const uint8_t mode);
    uint8_t getMode(void) const;

    void setFan(const uint8_t fan);
    uint8_t getFan(void) const;

    void setSwingVertical(const uint8_t position);
    uint8_t getSwingVertical(void) const;

    // # of bytes per command
    Daikin160Protocol _;
    void stateReset(void);
    void checksum(void);
    Ir_Send _ir_daikin160_send;
};

/*
 * Ir daikin 176 class
 */
class Ir_Daikin176 {
public:
    Ir_Daikin176(uint32_t frequency, uint16_t dutycycle);
    virtual ~Ir_Daikin176();
    void sendDaikin176(const uint16_t repeat = kDaikin176DefaultRepeat);
    void setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp);
private:
    uint8_t convertMode(const stdAc::opmode_t mode);
    uint8_t convertFan(const stdAc::fanspeed_t speed);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
    void send(const unsigned char data[],
            const uint16_t nbytes = kDaikin176StateLength,
            const uint16_t repeat = kDaikin176DefaultRepeat);

    uint8_t* getRaw(void);
    void setRaw(const uint8_t new_code[]);
    static bool validChecksum(uint8_t state[],
                              const uint16_t length = kDaikin176StateLength);
    void on(void);
    void off(void);
    void setPower(const bool on);
    bool getPower(void) const;
    void setTemp(const uint8_t temp);
    uint8_t getTemp(void) const;
    void setMode(const uint8_t mode);
    uint8_t getMode(void) const;
    void setFan(const uint8_t fan);
    uint8_t getFan(void) const;
    void setSwingHorizontal(const uint8_t position);
    uint8_t getSwingHorizontal(void) const;

    // # of bytes per command
    Daikin176Protocol _;
    uint8_t _saved_temp;  ///< The previously user requested temp value.
    void stateReset(void);
    void checksum(void);

    Ir_Send _ir_daikin176_send;
};
/*
 * Ir daikin 128
 */
class Ir_Daikin128 {
public:
    Ir_Daikin128(uint32_t frequency, uint16_t dutycycle);
    virtual ~Ir_Daikin128();

    void sendDaikin128(const uint16_t repeat = kDaikin128DefaultRepeat);
    /// Run the calibration to calculate uSec timing offsets for this platform.
    /// @return The uSec timing offset needed per modulation of the IR Led.
    /// @note This will produce a 65ms IR signal pulse at 38kHz.
    ///   Only ever needs to be run once per object instantiation, if at all.
    void setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp);
private:
    uint8_t convertMode(const stdAc::opmode_t mode);
    uint8_t convertFan(const stdAc::fanspeed_t speed);
    void send(const unsigned char data[],
              const uint16_t nbytes = kDaikin128StateLength,
              const uint16_t repeat = kDaikin128DefaultRepeat);
    void setPowerToggle(const bool toggle);
    bool getPowerToggle(void) const;
    void setTemp(const uint8_t temp);
    uint8_t getTemp(void) const;
    void setFan(const uint8_t speed);
    uint8_t getFan(void) const;
    uint8_t getMode(void) const;
    void setMode(const uint8_t mode);
    void setSwingVertical(const bool on);
    bool getSwingVertical(void) const;
    bool getSleep(void) const;
    void setSleep(const bool on);
    bool getQuiet(void) const;
    void setQuiet(const bool on);
    bool getPowerful(void) const;
    void setPowerful(const bool on);
    void setEcono(const bool on);
    bool getEcono(void) const;
    void setOnTimer(const uint16_t on);
    uint16_t getOnTimer(void) const;
    bool getOnTimerEnabled(void) const;
    void setOnTimerEnabled(const bool on);
    void setOffTimer(const uint16_t mins_since_midnight);
    uint16_t getOffTimer(void) const;
    bool getOffTimerEnabled(void) const;
    void setOffTimerEnabled(const bool on);
    void setClock(const uint16_t mins_since_midnight);
    uint16_t getClock(void) const;
    void setLightToggle(const uint8_t unit);
    uint8_t getLightToggle(void) const;
    uint8_t* getRaw(void);
    void setRaw(const uint8_t new_code[]);
    static bool validChecksum(uint8_t state[]);

    // # of bytes per command
    Daikin128Protocol _;
    void stateReset(void);
    static uint8_t calcFirstChecksum(const uint8_t state[]);
    static uint8_t calcSecondChecksum(const uint8_t state[]);
    void checksum(void);

    Ir_Send _ir_daikin128_send;
};
/*
 * Ir daikin152 class
 */
class Ir_Daikin152 {
public:
    Ir_Daikin152(uint32_t frequency, uint16_t dutycycle);
    virtual ~Ir_Daikin152();
    void sendDaikin152(const uint16_t repeat = kDaikin152DefaultRepeat);
    void setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp);
private:
    uint8_t convertMode(const stdAc::opmode_t mode) ;
    uint8_t convertFan(const stdAc::fanspeed_t speed);
    void send(const unsigned char data[],
                  const uint16_t nbytes = kDaikin152StateLength,
                  const uint16_t repeat = kDaikin152DefaultRepeat);

    uint8_t* getRaw(void);
    void setRaw(const uint8_t new_code[]);
    static bool validChecksum(uint8_t state[],
                              const uint16_t length = kDaikin152StateLength);
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
    void setSwingV(const bool on);
    bool getSwingV(void) const;
    bool getQuiet(void) const;
    void setQuiet(const bool on);
    bool getPowerful(void) const;
    void setPowerful(const bool on);
    void setSensor(const bool on);
    bool getSensor(void) const;
    void setEcono(const bool on);
    bool getEcono(void) const;
    void setComfort(const bool on);
    bool getComfort(void) const;
    // # of bytes per command
    Daikin152Protocol _;
    void stateReset(void);
    void checksum(void);

    Ir_Send _ir_daikin152_send;
};
/*
 * Ir daikin64 class
 */
class Ir_Daikin64 {
public:
    Ir_Daikin64(uint32_t frequency, uint16_t dutycycle);
    virtual ~Ir_Daikin64();
    void sendDaikin64(const uint16_t repeat = kDaikin64DefaultRepeat);
    void setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp);
private:
    uint8_t convertFan(const stdAc::fanspeed_t speed);
    uint8_t convertMode(const stdAc::opmode_t mode);

    void send_64(const uint64_t data, const uint16_t nbits = kDaikin64Bits,
                 const uint16_t repeat = kDaikin64DefaultRepeat);
    uint64_t getRaw(void);
    void setRaw(const uint64_t new_state);
    static uint8_t calcChecksum(const uint64_t state);
    static bool validChecksum(const uint64_t state);
    void setPowerToggle(const bool on);
    bool getPowerToggle(void) const;
    void setTemp(const uint8_t temp);
    uint8_t getTemp(void) const;
    void setFan(const uint8_t speed);
    uint8_t getFan(void) const;
    void setMode(const uint8_t mode);
    uint8_t getMode(void) const;
    void setSwingVertical(const bool on);
    bool getSwingVertical(void) const;
    void setSleep(const bool on);
    bool getSleep(void) const;
    bool getQuiet(void) const;
    void setQuiet(const bool on);
    bool getTurbo(void) const;
    void setTurbo(const bool on);
    void setClock(const uint16_t mins_since_midnight);
    uint16_t getClock(void) const;
    void setOnTimeEnabled(const bool on);
    bool getOnTimeEnabled(void) const;
    void setOnTime(const uint16_t mins_since_midnight);
    uint16_t getOnTime(void) const;
    void setOffTimeEnabled(const bool on);
    bool getOffTimeEnabled(void) const;
    void setOffTime(const uint16_t mins_since_midnight);
    uint16_t getOffTime(void) const;

    Daikin64Protocol _;
    void stateReset(void);
    void checksum(void);
    Ir_Send _ir_daikin64_send;
};
#endif /* MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRDAIKIN_H_ */

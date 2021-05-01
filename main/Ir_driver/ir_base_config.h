/*
 * ir_base_config.h
 *
 *  Created on: Dec 17, 2020
 *      Author: Yolo
 */

#ifndef MAIN_IR_DRIVER_IR_BASE_CONFIG_H_
#define MAIN_IR_DRIVER_IR_BASE_CONFIG_H_
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "../Common.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
/// Enumerator for defining and numbering of supported IR protocol.
/// @note Always add to the end of the list and should never remove entries
///  or change order. Projects may save the type number for later usage
///  so numbering should always stay the same.
enum decode_type_t {
    UNKNOWN = -1,
    UNUSED = 0,
    RC5,
    RC6,
    NEC,
    SONY,
    PANASONIC,  // (5)
    JVC,
    SAMSUNG,
    WHYNTER,
    AIWA_RC_T501,
    LG,  // (10)
    SANYO,
    MITSUBISHI,
    DISH,
    SHARP,
    COOLIX,  // (15)
    DAIKIN,
    DENON,
    KELVINATOR,
    SHERWOOD,
    MITSUBISHI_AC,  // (20)
    RCMM,
    SANYO_LC7461,
    RC5X,
    GREE,
    PRONTO,  // Technically not a protocol, but an encoding. (25)
    NEC_LIKE,
    ARGO,
    TROTEC,
    NIKAI,
    RAW,  // Technically not a protocol, but an encoding. (30)
    GLOBALCACHE,  // Technically not a protocol, but an encoding.
    TOSHIBA_AC,
    FUJITSU_AC,
    MIDEA,
    MAGIQUEST,  // (35)
    LASERTAG,
    CARRIER_AC,
    HAIER_AC,
    MITSUBISHI2,
    HITACHI_AC,  // (40)
    HITACHI_AC1,
    HITACHI_AC2,
    GICABLE,
    HAIER_AC_YRW02,
    WHIRLPOOL_AC,  // (45)
    SAMSUNG_AC,
    LUTRON,
    ELECTRA_AC,
    PANASONIC_AC,
    PIONEER,  // (50)
    LG2,
    MWM,
    DAIKIN2,
    VESTEL_AC,
    TECO,  // (55)
    SAMSUNG36,
    TCL112AC,
    LEGOPF,
    MITSUBISHI_HEAVY_88,
    MITSUBISHI_HEAVY_152,  // 60
    DAIKIN216,
    SHARP_AC,
    GOODWEATHER,
    INAX,
    DAIKIN160,  // 65
    NEOCLIMA,
    DAIKIN176,
    DAIKIN128,
    AMCOR,
    DAIKIN152,  // 70
    MITSUBISHI136,
    MITSUBISHI112,
    HITACHI_AC424,
    SONY_38K,
    EPSON,  // 75
    SYMPHONY,
    HITACHI_AC3,
    DAIKIN64,
    AIRWELL,
    DELONGHI_AC,  // 80
    DOSHISHA,
    MULTIBRACKETS,
    CARRIER_AC40,
    CARRIER_AC64,
    HITACHI_AC344,  // 85
    CORONA_AC,
    MIDEA24,
    ZEPEAL,
    SANYO_AC,
    VOLTAS,  // 90
    METZ,
    TRANSCOLD,
    TECHNIBEL_AC,
    MIRAGE,
    ELITESCREENS,  // 95
    PANASONIC_AC32,
    // Add new entries before this one, and update it to point to the last entry.
    kLastDecodeType = PANASONIC_AC32,
};
// Message lengths & required repeat values
#define kNoRepeat 0
#define kSingleRepeat 1

#define kAirwellBits 34
#define kAirwellMinRepeats 2
#define kAiwaRcT501Bits 15
#define kAiwaRcT501MinRepeats kSingleRepeat
#define kAlokaBits 32
#define kAmcorStateLength 8
#define kAmcorBits kAmcorStateLength * 8
#define kAmcorDefaultRepeat kSingleRepeat
#define kArgoStateLength 12
#define kArgoBits kArgoStateLength * 8
#define kArgoDefaultRepeat kNoRepeat
#define kCoolixBits 24
#define kCoolixDefaultRepeat kSingleRepeat
#define kCarrierAcBits 32
#define kCarrierAcMinRepeat kNoRepeat
#define kCarrierAc40Bits 40
#define kCarrierAc40MinRepeat 2
#define kCarrierAc64Bits 64
#define kCarrierAc64MinRepeat kNoRepeat
#define kCoronaAcStateLengthShort 7
#define kCoronaAcStateLength kCoronaAcStateLengthShort * 3
#define kCoronaAcBitsShort kCoronaAcStateLengthShort * 8
#define kCoronaAcBits kCoronaAcStateLength * 8
#define kDaikinStateLength 35
#define kDaikinBits kDaikinStateLength * 8
#define kDaikinStateLengthShort kDaikinStateLength - 8
#define kDaikinBitsShort kDaikinStateLengthShort * 8
#define kDaikinDefaultRepeat kNoRepeat
#define kDaikin2StateLength 39
#define kDaikin2Bits kDaikin2StateLength * 8
#define kDaikin2DefaultRepeat kNoRepeat
#define kDaikin64Bits 64
#define kDaikin64DefaultRepeat kNoRepeat
#define kDaikin160StateLength 20
#define kDaikin160Bits kDaikin160StateLength * 8
#define kDaikin160DefaultRepeat kNoRepeat
#define kDaikin128StateLength 16
#define kDaikin128Bits kDaikin128StateLength * 8
#define kDaikin128DefaultRepeat kNoRepeat
#define kDaikin152StateLength 19
#define kDaikin152Bits kDaikin152StateLength * 8
#define kDaikin152DefaultRepeat kNoRepeat
#define kDaikin176StateLength 22
#define kDaikin176Bits kDaikin176StateLength * 8
#define kDaikin176DefaultRepeat kNoRepeat
#define kDaikin216StateLength 27
#define kDaikin216Bits kDaikin216StateLength * 8
#define kDaikin216DefaultRepeat kNoRepeat
#define kDelonghiAcBits 64
#define kDelonghiAcDefaultRepeat kNoRepeat
#define kTechnibelAcBits 56
#define kTechnibelAcDefaultRepeat kNoRepeat
#define kDenonBits 15
#define kDenon48Bits 48
#define kDenonLegacyBits 14
#define kDishBits 16
#define kDishMinRepeat 3
#define kDoshishaBits 40
#define kEpsonBits 32
#define kEpsonMinRepeat 2
#define kElectraAcStateLength 13
#define kElectraAcBits kElectraAcStateLength * 8
#define kElectraAcMinRepeat kNoRepeat
#define kFujitsuAcMinRepeat kNoRepeat
#define kFujitsuAcStateLength 16
#define kFujitsuAcStateLengthShort 7
#define kFujitsuAcBits kFujitsuAcStateLength * 8
#define kFujitsuAcMinBits (kFujitsuAcStateLengthShort - 1) * 8
#define kGicableBits 16
#define kGicableMinRepeat kSingleRepeat
#define kGoodweatherBits 48
#define kGoodweatherMinRepeat kNoRepeat
#define kGreeStateLength 8
#define kGreeBits kGreeStateLength * 8
#define kGreeDefaultRepeat kNoRepeat
#define kHaierACStateLength 9
#define kHaierACBits kHaierACStateLength * 8
#define kHaierAcDefaultRepeat kNoRepeat
#define kHaierACYRW02StateLength 14
#define kHaierACYRW02Bits kHaierACYRW02StateLength * 8
#define kHaierAcYrw02DefaultRepeat kNoRepeat
#define kHitachiAcStateLength 28
#define kHitachiAcBits kHitachiAcStateLength * 8
#define kHitachiAcDefaultRepeat kNoRepeat
#define kHitachiAc1StateLength 13
#define kHitachiAc1Bits kHitachiAc1StateLength * 8
#define kHitachiAc2StateLength 53
#define kHitachiAc2Bits kHitachiAc2StateLength * 8
#define kHitachiAc3StateLength 27
#define kHitachiAc3Bits kHitachiAc3StateLength * 8
#define kHitachiAc3MinStateLength 15
#define kHitachiAc3MinBits kHitachiAc3MinStateLength * 8
#define kHitachiAc344StateLength 43
#define kHitachiAc344Bits kHitachiAc344StateLength * 8
#define kHitachiAc424StateLength 53
#define kHitachiAc424Bits kHitachiAc424StateLength * 8
#define kInaxBits 24
#define kInaxMinRepeat kSingleRepeat
#define kJvcBits 16
#define kKelvinatorStateLength 16
#define kKelvinatorBits kKelvinatorStateLength * 8
#define kKelvinatorDefaultRepeat kNoRepeat
#define kLasertagBits 13
#define kLasertagMinRepeat kNoRepeat
#define kLegoPfBits 16
#define kLegoPfMinRepeat kNoRepeat
#define kLgBits 28
#define kLg32Bits 32
#define kLgDefaultRepeat kNoRepeat
#define kLutronBits 35
#define kMagiquestBits 56
#define kMetzBits 19
#define kMetzMinRepeat kNoRepeat
#define kMideaBits 48
#define kMideaMinRepeat kNoRepeat
#define kMidea24Bits 24
#define kMidea24MinRepeat kSingleRepeat
#define kMitsubishiBits 16
// TODO(anyone): Verify that the Mitsubishi repeat is really needed.
//               Based on marcosamarinho's code.
#define kMitsubishiMinRepeat kSingleRepeat
#define kMitsubishiACStateLength 18
#define kMitsubishiACBits kMitsubishiACStateLength * 8
#define kMitsubishiACMinRepeat kSingleRepeat
#define kMitsubishi136StateLength 17
#define kMitsubishi136Bits kMitsubishi136StateLength * 8
#define kMitsubishi136MinRepeat kNoRepeat
#define kMitsubishi112StateLength 14
#define kMitsubishi112Bits kMitsubishi112StateLength * 8
#define kMitsubishi112MinRepeat kNoRepeat
#define kMitsubishiHeavy88StateLength 11
#define kMitsubishiHeavy88Bits kMitsubishiHeavy88StateLength * 8
#define kMitsubishiHeavy88MinRepeat kNoRepeat
#define kMitsubishiHeavy152StateLength 19
#define kMitsubishiHeavy152Bits kMitsubishiHeavy152StateLength * 8
#define kMitsubishiHeavy152MinRepeat kNoRepeat
#define kMultibracketsBits 8
#define kMultibracketsDefaultRepeat kSingleRepeat
#define kNikaiBits 24
#define kNECBits 32
#define kNeoclimaStateLength 12
#define kNeoclimaBits kNeoclimaStateLength * 8
#define kNeoclimaMinRepeat kNoRepeat
#define kPanasonicBits 48
#define kPanasonicManufacturer 0x4004
#define kPanasonicAcStateLength 27
#define kPanasonicAcStateShortLength 16
#define kPanasonicAcBits kPanasonicAcStateLength * 8
#define kPanasonicAcShortBits kPanasonicAcStateShortLength * 8
#define kPanasonicAc32Bits 32
#define kPanasonicAcDefaultRepeat kNoRepeat
#define kPioneerBits 64
#define kProntoMinLength 6
#define kRC5RawBits 14
#define kRC5Bits kRC5RawBits - 2
#define kRC5XBits kRC5RawBits - 1
#define kRC6Mode0Bits 20 // Excludes the 'start' bit.
#define kRC6_36Bits 36   // Excludes the 'start' bit.
#define kRCMMBits 24
#define kSamsungBits 32
#define kSamsung36Bits 36
#define kSamsungAcStateLength 14
#define kSamsungAcBits kSamsungAcStateLength * 8
#define kSamsungAcExtendedStateLength 21
#define kSamsungAcExtendedBits kSamsungAcExtendedStateLength * 8
#define kSamsungAcDefaultRepeat kNoRepeat
#define kSanyoAcStateLength 9
#define kSanyoAcBits kSanyoAcStateLength * 8
#define kSanyoSA8650BBits 12
#define kSanyoLC7461AddressBits 13
#define kSanyoLC7461CommandBits 8
#define kSanyoLC7461Bits (kSanyoLC7461AddressBits + kSanyoLC7461CommandBits) * 2
#define kSharpAddressBits 5
#define kSharpCommandBits 8
#define kSharpBits kSharpAddressBits + kSharpCommandBits + 2 // 15
#define kSharpAcStateLength 13
#define kSharpAcBits kSharpAcStateLength * 8 // 104
#define kSharpAcDefaultRepeat kNoRepeat
#define kSherwoodBits kNECBits
#define kSherwoodMinRepeat kSingleRepeat
#define kSony12Bits 12
#define kSony15Bits 15
#define kSony20Bits 20
#define kSonyMinBits 12
#define kSonyMinRepeat 2
#define kSymphonyBits 12
#define kSymphonyDefaultRepeat 3
#define kTcl112AcStateLength 14
#define kTcl112AcBits kTcl112AcStateLength * 8
#define kTcl112AcDefaultRepeat kNoRepeat
#define kTecoBits 35
#define kTecoDefaultRepeat kNoRepeat
#define kToshibaACStateLength 9
#define kToshibaACBits kToshibaACStateLength * 8
#define kToshibaACMinRepeat kSingleRepeat
#define kToshibaACStateLengthShort kToshibaACStateLength - 2
#define kToshibaACBitsShort kToshibaACStateLengthShort * 8
#define kToshibaACStateLengthLong kToshibaACStateLength + 1
#define kToshibaACBitsLong kToshibaACStateLengthLong * 8
#define kTranscoldBits 24
#define kTranscoldDefaultRepeat kNoRepeat
#define kTrotecStateLength 9
#define kTrotecBits kTrotecStateLength * 8
#define kTrotecDefaultRepeat kNoRepeat
#define kWhirlpoolAcStateLength 21
#define kWhirlpoolAcBits kWhirlpoolAcStateLength * 8
#define kWhirlpoolAcDefaultRepeat kNoRepeat
#define kWhynterBits 32
#define kVestelAcBits 56
#define kZepealBits 16
#define kZepealMinRepeat 4
#define kVoltasBits 80
#define kVoltasStateLength 10

// Legacy defines. (Deprecated)
#define AIWA_RC_T501_BITS kAiwaRcT501Bits
#define ARGO_COMMAND_LENGTH kArgoStateLength
#define COOLIX_BITS kCoolixBits
#define CARRIER_AC_BITS kCarrierAcBits
#define DAIKIN_COMMAND_LENGTH kDaikinStateLength
#define DENON_BITS kDenonBits
#define DENON_48_BITS kDenon48Bits
#define DENON_LEGACY_BITS kDenonLegacyBits
#define DISH_BITS kDishBits
#define FUJITSU_AC_MIN_REPEAT kFujitsuAcMinRepeat
#define FUJITSU_AC_STATE_LENGTH kFujitsuAcStateLength
#define FUJITSU_AC_STATE_LENGTH_SHORT kFujitsuAcStateLengthShort
#define FUJITSU_AC_BITS kFujitsuAcBits
#define FUJITSU_AC_MIN_BITS kFujitsuAcMinBits
#define GICABLE_BITS kGicableBits
#define GREE_STATE_LENGTH kGreeStateLength
#define HAIER_AC_STATE_LENGTH kHaierACStateLength
#define HAIER_AC_YRW02_STATE_LENGTH kHaierACYRW02StateLength
#define HITACHI_AC_STATE_LENGTH kHitachiAcStateLength
#define HITACHI_AC_BITS kHitachiAcBits
#define HITACHI_AC1_STATE_LENGTH kHitachiAc1StateLength
#define HITACHI_AC1_BITS kHitachiAc1Bits
#define HITACHI_AC2_STATE_LENGTH kHitachiAc2StateLength
#define HITACHI_AC2_BITS kHitachiAc2Bits
#define JVC_BITS kJvcBits
#define KELVINATOR_STATE_LENGTH kKelvinatorStateLength
#define LASERTAG_BITS kLasertagBits
#define LG_BITS kLgBits
#define LG32_BITS kLg32Bits
#define MAGIQUEST_BITS kMagiquestBits
#define MIDEA_BITS kMideaBits
#define MITSUBISHI_BITS kMitsubishiBits
#define MITSUBISHI_AC_STATE_LENGTH kMitsubishiACStateLength
#define NEC_BITS kNECBits
#define NIKAI_BITS kNikaiBits
#define PANASONIC_BITS kPanasonicBits
#define RC5_BITS kRC5Bits
#define RC5X_BITS kRC5XBits
#define RC6_MODE0_BITS kRC6Mode0Bits
#define RC6_36_BITS kRC6_36Bits
#define RCMM_BITS kRCMMBits
#define SANYO_LC7461_BITS kSanyoLC7461Bits
#define SAMSUNG_BITS kSamsungBits
#define SANYO_SA8650B_BITS kSanyoSA8650BBits
#define SHARP_BITS kSharpBits
#define SHERWOOD_BITS kSherwoodBits
#define SONY_12_BITS kSony12Bits
#define SONY_15_BITS kSony15Bits
#define SONY_20_BITS kSony20Bits
#define TOSHIBA_AC_STATE_LENGTH kToshibaACStateLength
#define TROTEC_COMMAND_LENGTH kTrotecStateLength
#define WHYNTER_BITS kWhynterBits


//  Usecs to wait between messages we don't know the proper gap time.
#define kDefaultMessageGap 100000
#define kMarkExcess  50
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***         Exported global functions                                     ***/
/****************************************************************************/

#endif /* MAIN_IR_DRIVER_IR_BASE_CONFIG_H_ */

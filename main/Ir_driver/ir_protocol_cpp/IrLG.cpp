/*
 * IrLG.cpp
 *
 *  Created on: Jan 15, 2021
 *      Author: ductu
 */

/***********************************************************************************************************************
 * Pragma directive
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes <System Includes>
 ***********************************************************************************************************************/
#include "IrLG.h"
#include "IRutils.h"
/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/
// Constants
// Common timings
const uint16_t kLgBitMark = 550;              ///< uSeconds.
const uint16_t kLgOneSpace = 1600;            ///< uSeconds.
const uint16_t kLgZeroSpace = 550;            ///< uSeconds.
const uint16_t kLgRptSpace = 2250;            ///< uSeconds.
const uint16_t kLgMinGap = 39750;             ///< uSeconds.
const uint32_t kLgMinMessageLength = 108050;  ///< uSeconds.
// LG (28 Bit)
const uint16_t kLgHdrMark = 8500;             ///< uSeconds.
const uint16_t kLgHdrSpace = 4250;            ///< uSeconds.
// LG (32 Bit)
const uint16_t kLg32HdrMark = 4500;           ///< uSeconds.
const uint16_t kLg32HdrSpace = 4450;          ///< uSeconds.
const uint16_t kLg32RptHdrMark = 8950;        ///< uSeconds.
// LG2 (28 Bit)
const uint16_t kLg2HdrMark = 3200;            ///< uSeconds.
const uint16_t kLg2HdrSpace = 9900;           ///< uSeconds.
const uint16_t kLg2BitMark = 480;             ///< uSeconds.

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables and functions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global variables and functions (to be accessed by other files)
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Imported global variables and functions (from other files)
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: 
 * Description  :
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
Ir_LG::Ir_LG(uint32_t frequency, uint16_t dutycycle): _ir_lg_send(frequency, dutycycle) {
	// TODO Auto-generated constructor stub
	stateReset();
}

Ir_LG::~Ir_LG() {
	// TODO Auto-generated destructor stub
}

void Ir_LG::sendLGAC(const uint16_t repeat) {
    if (getPower())
    {
        switch (_protocol) {
            case LG:
                sendLG(getRaw(), kLgBits, repeat);
                break;
            case LG2:
            	sendLG2(getRaw(), kLgBits, repeat);
                break;
            default:
                APP_LOGE("not support this protocol = %d",_protocol);
                break;
        }
//        _ir_lg_send.send(_protocol, getRaw(), kLgBits, repeat);
    }
    else
    {
        // Always send the special Off command if the power is set to off.
        // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1008#issuecomment-570763580
//        _ir_lg_send.send(_protocol, kLgAcOffCommand, kLgBits, repeat);
        switch (_protocol) {
            case LG:
                sendLG(kLgAcOffCommand, kLgBits, repeat);
                break;
            case LG2:
            	sendLG2(kLgAcOffCommand, kLgBits, repeat);
                break;
            default:
                APP_LOGE("not support this protocol = %d",_protocol);
                break;
        }
    }
}

void Ir_LG::setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp) {
    setPower(power);
    setMode(convertMode(stdAc::opmode_t(mode)));
    setFan(convertFan(stdAc::fanspeed_t(speed)));
    setTemp(temp);
}

/***********************************************************************************************************************
 * Static Functions
 ***********************************************************************************************************************/
/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t Ir_LG::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kLgAcCool;
    case stdAc::opmode_t::kHeat: return kLgAcHeat;
    case stdAc::opmode_t::kFan:  return kLgAcFan;
    case stdAc::opmode_t::kDry:  return kLgAcDry;
    default:                     return kLgAcAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t Ir_LG::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:    return kLgAcFanLowest;
    case stdAc::fanspeed_t::kLow:    return kLgAcFanLow;
    case stdAc::fanspeed_t::kMedium: return kLgAcFanMedium;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:    return kLgAcFanHigh;
    default:                         return kLgAcFanAuto;
  }
}

void Ir_LG::sendLG(uint64_t data, uint16_t nbits, uint16_t repeat) {
    uint16_t repeatHeaderMark = 0;
    uint8_t duty = 50;

    if (nbits >= kLg32Bits) {
        // LG 32bit protocol is near identical to Samsung except for repeats.
//        sendSAMSUNG(data, nbits, 0);  // Send it as a single Samsung message.
        repeatHeaderMark = kLg32RptHdrMark;
        duty = 33;
        repeat++;
    } else {
        // LG (28-bit) protocol.
        repeatHeaderMark = kLgHdrMark;
        _ir_lg_send.sendGeneric(kLgHdrMark, kLgHdrSpace, kLgBitMark, kLgOneSpace, kLgBitMark,
                    kLgZeroSpace, kLgBitMark, kLgMinGap, kLgMinMessageLength, data,
                    nbits, 38, true, 0,  // Repeats are handled later.
                    duty);
    }

    // Repeat
    // Protocol has a mandatory repeat-specific code sent after every command.
    if (repeat)
        _ir_lg_send.sendGeneric(repeatHeaderMark, kLgRptSpace, 0, 0, 0, 0,  // No data is sent.
                    kLgBitMark, kLgMinGap, kLgMinMessageLength, 0, 0,  // No data.
                    38, true, repeat - 1, duty);
}

void Ir_LG::sendLG2(uint64_t data, uint16_t nbits, uint16_t repeat) {
    if (nbits >= kLg32Bits) {
        // Let the original routine handle it.
        sendLG(data, nbits, repeat);  // Send it as a single Samsung message.
        return;
    }

    // LGv2 (28-bit) protocol.
    _ir_lg_send.sendGeneric(kLg2HdrMark, kLg2HdrSpace, kLg2BitMark, kLgOneSpace, kLg2BitMark,
                kLgZeroSpace, kLg2BitMark, kLgMinGap, kLgMinMessageLength, data,
                nbits, 38, true, 0,  // Repeats are handled later.
                33);  // Use a duty cycle of 33% (Testing)

    // TODO(crackn): Verify the details of what repeat messages look like.
    // Repeat
    // Protocol has a mandatory repeat-specific code sent after every command.
    if (repeat)
        _ir_lg_send.sendGeneric(kLg2HdrMark, kLgRptSpace, 0, 0, 0, 0,  // No data is sent.
                    kLgBitMark, kLgMinGap, kLgMinMessageLength, 0, 0,  // No data.
                    38, true, repeat - 1, 50);
}

void Ir_LG::on() {
    setPower(true);
}

void Ir_LG::off() {
    setPower(false);
}

void Ir_LG::setPower(const bool on) {
    _.Power = (on ? kLgAcPowerOn : kLgAcPowerOff);
    if (on)
        setTemp(_temp);  // Reset the temp if we are on.
    else
        _setTemp(0);  // Off clears the temp.
}

bool Ir_LG::getPower() const {
    return _.Power == kLgAcPowerOn;
}

void Ir_LG::setTemp(const uint8_t degrees) {
    uint8_t temp = std::max(kLgAcMinTemp, degrees);
    temp = std::min(kLgAcMaxTemp, temp);
    _temp = temp;
    _setTemp(temp - kLgAcTempAdjust);
}

uint8_t Ir_LG::getTemp() const {
    if (getPower())
        return _.Temp + kLgAcTempAdjust;
    else
        return _temp;
}

void Ir_LG::setFan(const uint8_t speed) {
    switch (speed) {
        case kLgAcFanAuto:
        case kLgAcFanLowest:
        case kLgAcFanLow:
        case kLgAcFanMedium:
        case kLgAcFanHigh:
            _.Fan = speed;
            break;
        default:
            _.Fan = kLgAcFanAuto;
    }
}

uint8_t Ir_LG::getFan() const {
    return _.Fan;
}

void Ir_LG::setMode(const uint8_t mode) {
    switch (mode) {
        case kLgAcAuto:
        case kLgAcDry:
        case kLgAcHeat:
        case kLgAcCool:
        case kLgAcFan:
            _.Mode = mode;
            break;
        default:
            _.Mode = kLgAcAuto;
    }
}

uint8_t Ir_LG::getMode() const {
    return _.Mode;
}

uint32_t Ir_LG::getRaw() {
    checksum();
    return _.raw;
}

void Ir_LG::setRaw(const uint32_t new_code) {
    _.raw = new_code;
    _temp = 15;  // Ensure there is a "sane" previous temp.
    _temp = getTemp();
}

void Ir_LG::checksum() {
    _.Sum = calcChecksum(_.raw);
}

void Ir_LG::_setTemp(const uint8_t value) {
    _.Temp = value;
}

lg_ac_remote_model_t Ir_LG::getModel() const {
    switch (_protocol) {
        case LG2:
            return lg_ac_remote_model_t::AKB75215403;
        case LG:
            // FALL THRU
        default:
            return lg_ac_remote_model_t::GE6711AR2853M;
    }
}
void Ir_LG::stateReset() {
    setRaw(kLgAcOffCommand);
    setModel(lg_ac_remote_model_t::GE6711AR2853M);
}

uint8_t Ir_LG::calcChecksum(const uint32_t state) {
    return irutils::sumNibbles(state >> 4, 4);
}

bool Ir_LG::validChecksum(const uint32_t state) {
    LGProtocol LGp;
    LGp.raw = state;
    return calcChecksum(state) == LGp.Sum;
}

bool Ir_LG::isValidLgAc() const {
    return validChecksum(_.raw) && (_.Sign == kLgAcSignature);
}

void Ir_LG::setModel(const lg_ac_remote_model_t model) {
    switch (model) {
        case lg_ac_remote_model_t::AKB75215403:
            _protocol = decode_type_t::LG2;
            break;
        case lg_ac_remote_model_t::GE6711AR2853M:
            // FALL THRU
        default:
            _protocol = decode_type_t::LG;
    }
}
/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/


/*
 * IrToshiba.cpp
 *
 *  Created on: Jan 16, 2021
 *      Author: ductu
 */
/***********************************************************************************************************************
 * Pragma directive
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes <System Includes>
 ***********************************************************************************************************************/
#include "IrToshiba.h"
#include "IRutils.h"
/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/
// Toshiba A/C
const uint16_t kToshibaAcHdrMark = 4400;
const uint16_t kToshibaAcHdrSpace = 4300;
const uint16_t kToshibaAcBitMark = 580;
const uint16_t kToshibaAcOneSpace = 1600;
const uint16_t kToshibaAcZeroSpace = 490;
const uint16_t kToshibaAcMinGap = 7400;
/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/
using irutils::checkInvertedBytePairs;
using irutils::invertBytePairs;
using irutils::setBit;
using irutils::setBits;
using std::min;
using std::max;
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
Ir_Toshiba::Ir_Toshiba(uint32_t frequency, uint16_t dutycycle): _ir_toshiba_send(frequency, dutycycle){
	// TODO Auto-generated constructor stub
    stateReset();
}

Ir_Toshiba::~Ir_Toshiba() {
	// TODO Auto-generated destructor stub
}

void Ir_Toshiba::sendToshibaAC(const uint16_t repeat) {
    _backupState();
    send(getRaw(), getStateLength(), repeat);
    if (_send_swing && (getStateLength() != kToshibaACStateLengthShort)) {
        setStateLength(kToshibaACStateLengthShort);
        // Swing settings expect the min temp to be set.
        // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1205#issuecomment-653922374
        setTemp(kToshibaAcMinTemp);
        setSwing(_swing_mode);
        send(getRaw(), getStateLength(), repeat);
        _restoreState();
    }
    _send_swing = false;
}

void Ir_Toshiba::setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp) {
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
uint8_t Ir_Toshiba::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kToshibaAcCool;
    case stdAc::opmode_t::kHeat: return kToshibaAcHeat;
    case stdAc::opmode_t::kDry:  return kToshibaAcDry;
    case stdAc::opmode_t::kFan:  return kToshibaAcFan;
    case stdAc::opmode_t::kOff:  return kToshibaAcOff;
    default:                     return kToshibaAcAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t Ir_Toshiba::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:    return kToshibaAcFanMax - 4;
    case stdAc::fanspeed_t::kLow:    return kToshibaAcFanMax - 3;
    case stdAc::fanspeed_t::kMedium: return kToshibaAcFanMax - 2;
    case stdAc::fanspeed_t::kHigh:   return kToshibaAcFanMax - 1;
    case stdAc::fanspeed_t::kMax:    return kToshibaAcFanMax;
    default:                         return kToshibaAcFanAuto;
  }
}

void Ir_Toshiba::send(const uint8_t *data, const uint16_t nbytes, const uint16_t repeat) {
    _ir_toshiba_send.sendGeneric(kToshibaAcHdrMark, kToshibaAcHdrSpace, kToshibaAcBitMark,
                kToshibaAcOneSpace, kToshibaAcBitMark, kToshibaAcZeroSpace,
                kToshibaAcBitMark, kToshibaAcMinGap, data, nbytes, 38, true,
                repeat, 50);
}

void Ir_Toshiba::on() {
    setPower(true);
}

void Ir_Toshiba::off() {
    setPower(false);
}

void Ir_Toshiba::setPower(const bool on) {
    if (on) {  // On
        // If not already on, pick the last non-off mode used
        if (!getPower()) setMode(prev_mode);
    } else {  // Off
        setMode(kToshibaAcOff);
    }
}

bool Ir_Toshiba::getPower() {
    return getMode(true) != kToshibaAcOff;
}

void Ir_Toshiba::setTemp(const uint8_t degrees) {
    uint8_t temp = std::max((uint8_t)kToshibaAcMinTemp, degrees);
    temp = std::min((uint8_t)kToshibaAcMaxTemp, temp);
    setBits(&remote_state[5], kToshibaAcTempOffset, kToshibaAcTempSize,
            temp - kToshibaAcMinTemp);
}

uint8_t Ir_Toshiba::getTemp() {
    return GETBITS8(remote_state[5], kToshibaAcTempOffset, kToshibaAcTempSize) +
           kToshibaAcMinTemp;
}

void Ir_Toshiba::setFan(const uint8_t speed) {
    uint8_t fan = speed;
    // Bounds check
    if (fan > kToshibaAcFanMax)
        fan = kToshibaAcFanMax;  // Set the fan to maximum if out of range.
    if (fan > kToshibaAcFanAuto) fan++;
    setBits(&remote_state[6], kToshibaAcFanOffset, kToshibaAcFanSize, fan);
}

uint8_t Ir_Toshiba::getFan() {
    uint8_t fan = GETBITS8(remote_state[6], kToshibaAcFanOffset,
                           kToshibaAcFanSize);
    if (fan == kToshibaAcFanAuto) return kToshibaAcFanAuto;
    return --fan;
}

void Ir_Toshiba::setTurbo(const bool on) {
    if (on) {
        remote_state[8] = kToshibaAcTurboOn;
        setStateLength(kToshibaACStateLengthLong);
    } else {
        if (!getEcono()) setStateLength(kToshibaACStateLength);
    }
}

bool Ir_Toshiba::getTurbo() {
    if (getStateLength() == kToshibaACStateLengthLong)
        return remote_state[8] == kToshibaAcTurboOn;
    return false;
}

void Ir_Toshiba::setEcono(const bool on) {
    if (on) {
        remote_state[8] = kToshibaAcEconoOn;
        setStateLength(kToshibaACStateLengthLong);
    } else {
        if (!getTurbo()) setStateLength(kToshibaACStateLength);
    }
}

bool Ir_Toshiba::getEcono() {
    if (getStateLength() == kToshibaACStateLengthLong)
        return remote_state[8] == kToshibaAcEconoOn;
    return false;
}

void Ir_Toshiba::setMode(const uint8_t mode) {
    if (mode != prev_mode)
        // Changing mode or power turns Econo & Turbo to off on a real remote.
        // Setting the internal message length to "normal" will do that.
        setStateLength(kToshibaACStateLength);
    switch (mode) {
        case kToshibaAcAuto:
        case kToshibaAcCool:
        case kToshibaAcDry:
        case kToshibaAcHeat:
        case kToshibaAcFan:
            prev_mode = mode;
            // FALL-THRU
        case kToshibaAcOff:
            setBits(&remote_state[6], kToshibaAcModeOffset, kToshibaAcModeSize,
                    mode);
            break;
        default: setMode(kToshibaAcAuto);
    }
}

uint8_t Ir_Toshiba::getMode(const bool raw) {
    const uint8_t mode = GETBITS8(remote_state[6], kToshibaAcModeOffset,
                                  kToshibaAcModeSize);
    if (raw) return mode;
    switch (mode) {
        case kToshibaAcOff: return prev_mode;
        default:            return mode;
    }
}

void Ir_Toshiba::setRaw(const uint8_t *newState) {
    memcpy(remote_state, newState, kToshibaACStateLength);
    prev_mode = getMode();
    _send_swing = true;
}

uint8_t * Ir_Toshiba::getRaw() {
    checksum(getStateLength());
    return remote_state;
}

uint16_t Ir_Toshiba::getInternalStateLength(const uint8_t *state, const uint16_t size) {
    if (size < kToshibaAcLengthByte) return 0;
    return std::min((state[kToshibaAcLengthByte] + kToshibaAcMinLength), (uint16_t)kToshibaACStateLengthLong);
}

uint16_t Ir_Toshiba::getStateLength() {
    return getInternalStateLength(remote_state, kToshibaACStateLengthLong);
}

bool Ir_Toshiba::validChecksum(const uint8_t *state, const uint16_t length) {
    return length >= kToshibaAcMinLength &&
           state[length - 1] == calcChecksum(state, length) &&
           checkInvertedBytePairs(state, kToshibaAcInvertedLength) &&
           getInternalStateLength(state, length) == length;
}

uint8_t Ir_Toshiba::getSwing(const bool raw) {
    return raw ? GETBITS8(remote_state[5], kToshibaAcSwingOffset,
                          kToshibaAcSwingSize) : _swing_mode;
}

void Ir_Toshiba::setSwing(const uint8_t setting) {
    switch (setting) {
        case kToshibaAcSwingStep:
        case kToshibaAcSwingOn:
        case kToshibaAcSwingOff:
            _send_swing = true;
            _swing_mode = setting;
            if (getStateLength() == kToshibaACStateLengthShort)
                setBits(&remote_state[5], kToshibaAcSwingOffset, kToshibaAcSwingSize,
                        setting);
    }
}

void Ir_Toshiba::checksum(const uint16_t length) {
    // Stored the checksum value in the last byte.
    if (length >= kToshibaAcMinLength) {
        // Set/clear the short msg bit.
        setBit(&remote_state[4], kToshibaAcShortMsgBit,
               getStateLength() == kToshibaACStateLengthShort);
        // Set/clear the long msg bit.
        setBit(&remote_state[4], kToshibaAcLongMsgBit,
               getStateLength() == kToshibaACStateLengthLong);
        invertBytePairs(remote_state, kToshibaAcInvertedLength);
        // Always do the Xor checksum LAST!
        remote_state[length - 1] = calcChecksum(remote_state, length);
    }
}

uint8_t Ir_Toshiba::calcChecksum(const uint8_t *state, const uint16_t length) {
    return length ? xorBytes(state, length - 1) : 0;
}

void Ir_Toshiba::setStateLength(const uint16_t size) {
    if (size < kToshibaAcMinLength) return;
    remote_state[kToshibaAcLengthByte] = size - kToshibaAcMinLength;
}

void Ir_Toshiba::_backupState() {
    memcpy(backup, remote_state, kToshibaACStateLengthLong);
}

void Ir_Toshiba::_restoreState() {
    memcpy(remote_state, backup, kToshibaACStateLengthLong);
}

/// Reset the state of the remote to a known good state/sequence.
/// @see https://github.com/r45635/HVAC-IR-Control/blob/master/HVAC_ESP8266/HVAC_ESP8266T.ino#L103
void Ir_Toshiba::stateReset(void) {
  static const uint8_t kReset[kToshibaACStateLength] = {
      0xF2, 0x0D, 0x03, 0xFC, 0x01};
  memcpy(remote_state, kReset, kToshibaACStateLength);
  setTemp(22);  // Remote defaults to 22C after factory reset. So do the same.
  setSwing(kToshibaAcSwingOff);
  prev_mode = getMode();
}
/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/

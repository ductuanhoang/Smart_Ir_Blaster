/*
 * IrMitsubishiHeavy.cpp
 *
 *  Created on: Mar 10, 2021
 *      Author: ductu
 */

#include "IrMitsubishiHeavy.h"
#include "IRutils.h"
/***********************************************************************************************************************
 * Pragma directive
 ***********************************************************************************************************************/
// Constants
const uint16_t kMitsubishiHeavyHdrMark = 3140;
const uint16_t kMitsubishiHeavyHdrSpace = 1630;
const uint16_t kMitsubishiHeavyBitMark = 370;
const uint16_t kMitsubishiHeavyOneSpace = 420;
const uint16_t kMitsubishiHeavyZeroSpace = 1220;
const uint32_t kMitsubishiHeavyGap = kDefaultMessageGap;  // Just a guess.
/***********************************************************************************************************************
 * Includes <System Includes>
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables and functions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global variables and functions (to be accessed by other files)
 ***********************************************************************************************************************/
using std::min;
using std::max;
using irutils::checkInvertedBytePairs;
using irutils::invertBytePairs;
/***********************************************************************************************************************
 * Imported global variables and functions (from other files)
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: 
 * Description  :
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
IRMitsubishiHeavy152Ac::IRMitsubishiHeavy152Ac() {
	stateReset();
}

IRMitsubishiHeavy152Ac::~IRMitsubishiHeavy152Ac() {


}


IRMitsubishiHeavy88Ac::IRMitsubishiHeavy88Ac(uint32_t frequency, uint16_t dutycycle):_ir_remote(frequency, dutycycle) {
	stateReset();
}


IRMitsubishiHeavy88Ac::~IRMitsubishiHeavy88Ac() {


}

void IRMitsubishiHeavy152Ac::setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp) {
    setPower(power);
    setMode(convertMode(stdAc::opmode_t(mode)));
    setFan(convertFan(stdAc::fanspeed_t(speed)));
    setTemp(temp);
}

void IRMitsubishiHeavy88Ac::setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp) {
    setPower(power);
    setMode(convertMode(stdAc::opmode_t(mode)));
    setFan(convertFan(stdAc::fanspeed_t(speed)));
    setTemp(temp);
}
/***********************************************************************************************************************
 * Static Functions
 ***********************************************************************************************************************/

/// Send a MitsubishiHeavy 88-bit A/C message.
/// Status: BETA / Appears to be working. Needs testing against a real device.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRMitsubishiHeavy88Ac::send(const unsigned char data[],
                                   const uint16_t nbytes,
                                   const uint16_t repeat) {
  if (nbytes < kMitsubishiHeavy88StateLength)
    return;  // Not enough bytes to send a proper message.
  _ir_remote.sendGeneric(kMitsubishiHeavyHdrMark, kMitsubishiHeavyHdrSpace,
              kMitsubishiHeavyBitMark, kMitsubishiHeavyOneSpace,
              kMitsubishiHeavyBitMark, kMitsubishiHeavyZeroSpace,
              kMitsubishiHeavyBitMark, kMitsubishiHeavyGap,
              data, nbytes, 38000, false, repeat, 50);
}

/// Send a MitsubishiHeavy 152-bit A/C message.
/// Status: BETA / Appears to be working. Needs testing against a real device.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRMitsubishiHeavy152Ac::send(const unsigned char data[],
                                    const uint16_t nbytes,
                                    const uint16_t repeat) {
  if (nbytes < kMitsubishiHeavy152StateLength)
    return;  // Not enough bytes to send a proper message.
  IRMitsubishiHeavy88Ac _ir_remote(38000, 50);
  _ir_remote.send(data, nbytes, repeat);
}



/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRMitsubishiHeavy152Ac::sendMitsubishiHeavy152Ac(const uint16_t repeat) {
  send(getRaw(), kMitsubishiHeavy152StateLength,
                                 repeat);
}


/// Reset the state of the remote to a known good state/sequence.
void IRMitsubishiHeavy152Ac::stateReset(void) {
    memcpy(_.raw, kMitsubishiHeavyZmsSig, kMitsubishiHeavySigLength);
    for (uint8_t i = kMitsubishiHeavySigLength;
         i < kMitsubishiHeavy152StateLength - 3; i += 2) _.raw[i] = 0;
    _.raw[17] = 0x80;
}

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t *IRMitsubishiHeavy152Ac::getRaw(void) {
    checksum();
    return _.raw;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] data A valid code for this protocol.
void IRMitsubishiHeavy152Ac::setRaw(const uint8_t *data) {
    memcpy(_.raw, data, kMitsubishiHeavy152StateLength);
}

/// Set the requested power state of the A/C to on.
void IRMitsubishiHeavy152Ac::on(void) { setPower(true); }

/// Set the requested power state of the A/C to off.
void IRMitsubishiHeavy152Ac::off(void) { setPower(false); }

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMitsubishiHeavy152Ac::setPower(const bool on) {
    _.Power = on;
}

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRMitsubishiHeavy152Ac::getPower(void) const {
    return _.Power;
}

/// Set the temperature.
/// @param[in] temp The temperature in degrees celsius.
void IRMitsubishiHeavy152Ac::setTemp(const uint8_t temp) {
    uint8_t newtemp = temp;
    newtemp = std::min(newtemp, kMitsubishiHeavyMaxTemp);
    newtemp = std::max(newtemp, kMitsubishiHeavyMinTemp);
    _.Temp = newtemp - kMitsubishiHeavyMinTemp;
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRMitsubishiHeavy152Ac::getTemp(void) const {
    return _.Temp + kMitsubishiHeavyMinTemp;
}

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
void IRMitsubishiHeavy152Ac::setFan(const uint8_t speed) {
    uint8_t newspeed = speed;
    switch (speed) {
        case kMitsubishiHeavy152FanLow:
        case kMitsubishiHeavy152FanMed:
        case kMitsubishiHeavy152FanHigh:
        case kMitsubishiHeavy152FanMax:
        case kMitsubishiHeavy152FanEcono:
        case kMitsubishiHeavy152FanTurbo: break;
        default: newspeed = kMitsubishiHeavy152FanAuto;
    }
    _.Fan = newspeed;
}

/// Get the current fan speed setting.
/// @return The current fan speed/mode.
uint8_t IRMitsubishiHeavy152Ac::getFan(void) const {
    return _.Fan;
}

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRMitsubishiHeavy152Ac::setMode(const uint8_t mode) {
    uint8_t newmode = mode;
    switch (mode) {
        case kMitsubishiHeavyCool:
        case kMitsubishiHeavyDry:
        case kMitsubishiHeavyFan:
        case kMitsubishiHeavyHeat:
            break;
        default:
            newmode = kMitsubishiHeavyAuto;
    }
    _.Mode = newmode;
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRMitsubishiHeavy152Ac::getMode(void) const {
    return _.Mode;
}

/// Set the Vertical Swing mode of the A/C.
/// @param[in] pos The position/mode to set the swing to.
void IRMitsubishiHeavy152Ac::setSwingVertical(const uint8_t pos) {
    _.SwingV = std::min(pos, kMitsubishiHeavy152SwingVOff);
}

/// Get the Vertical Swing mode of the A/C.
/// @return The native position/mode setting.
uint8_t IRMitsubishiHeavy152Ac::getSwingVertical(void) const {
    return _.SwingV;
}

/// Set the Horizontal Swing mode of the A/C.
/// @param[in] pos The position/mode to set the swing to.
void IRMitsubishiHeavy152Ac::setSwingHorizontal(const uint8_t pos) {
    _.SwingH = std::min(pos, kMitsubishiHeavy152SwingHOff);
}

/// Get the Horizontal Swing mode of the A/C.
/// @return The native position/mode setting.
uint8_t IRMitsubishiHeavy152Ac::getSwingHorizontal(void) const {
    return _.SwingH;
}

/// Set the Night (Sleep) mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMitsubishiHeavy152Ac::setNight(const bool on) {
    _.Night = on;
}

/// Get the Night (Sleep) mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRMitsubishiHeavy152Ac::getNight(void) const {
    return _.Night;
}

/// Set the 3D mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMitsubishiHeavy152Ac::set3D(const bool on) {
    if (on)
    { _.Three = 1; _.D = 1; }
    else
    { _.Three = 0; _.D = 0; }
}

/// Get the 3D mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRMitsubishiHeavy152Ac::get3D(void) const {
    return _.Three && _.D;
}

/// Set the Silent (Quiet) mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMitsubishiHeavy152Ac::setSilent(const bool on) {
    _.Silent = on;
}

/// Get the Silent (Quiet) mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRMitsubishiHeavy152Ac::getSilent(void) const {
    return _.Silent;
}

/// Set the Filter mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMitsubishiHeavy152Ac::setFilter(const bool on) {
    _.Filter = on;
}

/// Get the Filter mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRMitsubishiHeavy152Ac::getFilter(void) const {
    return _.Filter;
}

/// Set the Clean mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMitsubishiHeavy152Ac::setClean(const bool on) {
    _.Filter = on;
    _.Clean = on;
}

/// Get the Clean mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRMitsubishiHeavy152Ac::getClean(void) const {
    return _.Clean && _.Filter;
}

/// Set the Turbo mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMitsubishiHeavy152Ac::setTurbo(const bool on) {
    if (on)
        setFan(kMitsubishiHeavy152FanTurbo);
    else if (getTurbo()) setFan(kMitsubishiHeavy152FanAuto);
}

/// Get the Turbo mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRMitsubishiHeavy152Ac::getTurbo(void) const {
    return _.Fan == kMitsubishiHeavy152FanTurbo;
}

/// Set the Economical mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMitsubishiHeavy152Ac::setEcono(const bool on) {
    if (on)
        setFan(kMitsubishiHeavy152FanEcono);
    else if (getEcono()) setFan(kMitsubishiHeavy152FanAuto);
}

/// Get the Economical mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRMitsubishiHeavy152Ac::getEcono(void) const {
    return _.Fan == kMitsubishiHeavy152FanEcono;
}

/// Verify the given state has a ZM-S signature.
/// @param[in] state A ptr to a state to be checked.
/// @return true, the check passed. Otherwise, false.
bool IRMitsubishiHeavy152Ac::checkZmsSig(const uint8_t *state) {
    for (uint8_t i = 0; i < kMitsubishiHeavySigLength; i++)
        if (state[i] != kMitsubishiHeavyZmsSig[i]) return false;
    return true;
}

/// Calculate the checksum for the current internal state of the remote.
/// Note: Technically it has no checksum, but does have inverted byte pairs.
void IRMitsubishiHeavy152Ac::checksum(void) {
    const uint8_t kOffset = kMitsubishiHeavySigLength - 2;
    invertBytePairs(_.raw + kOffset, kMitsubishiHeavy152StateLength - kOffset);
}

/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @param[in] length The length/size of the state array.
/// @return true, if the state has a valid checksum. Otherwise, false.
/// Note: Technically it has no checksum, but does have inverted byte pairs.
bool IRMitsubishiHeavy152Ac::validChecksum(const uint8_t *state, const uint16_t length) {
    // Assume anything too short is fine.
    if (length < kMitsubishiHeavySigLength) return true;
    const uint8_t kOffset = kMitsubishiHeavySigLength - 2;
    return checkInvertedBytePairs(state + kOffset, length - kOffset);
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRMitsubishiHeavy152Ac::convertMode(const stdAc::opmode_t mode) {
    switch (mode) {
        case stdAc::opmode_t::kCool: return kMitsubishiHeavyCool;
        case stdAc::opmode_t::kHeat: return kMitsubishiHeavyHeat;
        case stdAc::opmode_t::kDry:  return kMitsubishiHeavyDry;
        case stdAc::opmode_t::kFan:  return kMitsubishiHeavyFan;
        default:                     return kMitsubishiHeavyAuto;
    }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRMitsubishiHeavy152Ac::convertFan(const stdAc::fanspeed_t speed) {
    switch (speed) {
        // Assumes Econo is slower than Low.
        case stdAc::fanspeed_t::kMin:    return kMitsubishiHeavy152FanEcono;
        case stdAc::fanspeed_t::kLow:    return kMitsubishiHeavy152FanLow;
        case stdAc::fanspeed_t::kMedium: return kMitsubishiHeavy152FanMed;
        case stdAc::fanspeed_t::kHigh:   return kMitsubishiHeavy152FanHigh;
        case stdAc::fanspeed_t::kMax:    return kMitsubishiHeavy152FanMax;
        default:                         return kMitsubishiHeavy152FanAuto;
    }
}

/// Convert a stdAc::swingv_t enum into it's native setting.
/// @param[in] position The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRMitsubishiHeavy152Ac::convertSwingV(const stdAc::swingv_t position) {
    switch (position) {
        case stdAc::swingv_t::kAuto:    return kMitsubishiHeavy152SwingVAuto;
        case stdAc::swingv_t::kHighest: return kMitsubishiHeavy152SwingVHighest;
        case stdAc::swingv_t::kHigh:    return kMitsubishiHeavy152SwingVHigh;
        case stdAc::swingv_t::kMiddle:  return kMitsubishiHeavy152SwingVMiddle;
        case stdAc::swingv_t::kLow:     return kMitsubishiHeavy152SwingVLow;
        case stdAc::swingv_t::kLowest:  return kMitsubishiHeavy152SwingVLowest;
        default:                        return kMitsubishiHeavy152SwingVOff;
    }
}

/// Convert a stdAc::swingh_t enum into it's native setting.
/// @param[in] position The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRMitsubishiHeavy152Ac::convertSwingH(const stdAc::swingh_t position) {
    switch (position) {
        case stdAc::swingh_t::kAuto:     return kMitsubishiHeavy152SwingHAuto;
        case stdAc::swingh_t::kLeftMax:  return kMitsubishiHeavy152SwingHLeftMax;
        case stdAc::swingh_t::kLeft:     return kMitsubishiHeavy152SwingHLeft;
        case stdAc::swingh_t::kMiddle:   return kMitsubishiHeavy152SwingHMiddle;
        case stdAc::swingh_t::kRight:    return kMitsubishiHeavy152SwingHRight;
        case stdAc::swingh_t::kRightMax: return kMitsubishiHeavy152SwingHRightMax;
        default:                         return kMitsubishiHeavy152SwingHOff;
    }
}




/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRMitsubishiHeavy88Ac::sendMitsubishiHeavy88(const uint16_t repeat) {
  send(getRaw(), kMitsubishiHeavy88StateLength,
                                repeat);
}


/// Reset the state of the remote to a known good state/sequence.
void IRMitsubishiHeavy88Ac::stateReset(void) {
    memcpy(_.raw, kMitsubishiHeavyZjsSig, kMitsubishiHeavySigLength);
    for (uint8_t i = kMitsubishiHeavySigLength; i < kMitsubishiHeavy88StateLength;
         i++) _.raw[i] = 0;
}

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t *IRMitsubishiHeavy88Ac::getRaw(void) {
    checksum();
    return _.raw;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] data A valid code for this protocol.
void IRMitsubishiHeavy88Ac::setRaw(const uint8_t *data) {
    memcpy(_.raw, data, kMitsubishiHeavy88StateLength);
}

/// Set the requested power state of the A/C to on.
void IRMitsubishiHeavy88Ac::on(void) { setPower(true); }

/// Set the requested power state of the A/C to off.
void IRMitsubishiHeavy88Ac::off(void) { setPower(false); }

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMitsubishiHeavy88Ac::setPower(const bool on) {
    _.Power = on;
}

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRMitsubishiHeavy88Ac::getPower(void) const {
    return _.Power;
}

/// Set the temperature.
/// @param[in] temp The temperature in degrees celsius.
void IRMitsubishiHeavy88Ac::setTemp(const uint8_t temp) {
    uint8_t newtemp = temp;
    newtemp = std::min(newtemp, kMitsubishiHeavyMaxTemp);
    newtemp = std::max(newtemp, kMitsubishiHeavyMinTemp);
    _.Temp = newtemp - kMitsubishiHeavyMinTemp;
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRMitsubishiHeavy88Ac::getTemp(void) const {
    return _.Temp + kMitsubishiHeavyMinTemp;
}

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
void IRMitsubishiHeavy88Ac::setFan(const uint8_t speed) {
    uint8_t newspeed = speed;
    switch (speed) {
        case kMitsubishiHeavy88FanLow:
        case kMitsubishiHeavy88FanMed:
        case kMitsubishiHeavy88FanHigh:
        case kMitsubishiHeavy88FanTurbo:
        case kMitsubishiHeavy88FanEcono: break;
        default: newspeed = kMitsubishiHeavy88FanAuto;
    }
    _.Fan = newspeed;
}

/// Get the current fan speed setting.
/// @return The current fan speed/mode.
uint8_t IRMitsubishiHeavy88Ac::getFan(void) const {
    return _.Fan;
}

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRMitsubishiHeavy88Ac::setMode(const uint8_t mode) {
    uint8_t newmode = mode;
    switch (mode) {
        case kMitsubishiHeavyCool:
        case kMitsubishiHeavyDry:
        case kMitsubishiHeavyFan:
        case kMitsubishiHeavyHeat:
            break;
        default:
            newmode = kMitsubishiHeavyAuto;
    }
    _.Mode = newmode;
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRMitsubishiHeavy88Ac::getMode(void) const {
    return _.Mode;
}

/// Set the Vertical Swing mode of the A/C.
/// @param[in] pos The position/mode to set the swing to.
void IRMitsubishiHeavy88Ac::setSwingVertical(const uint8_t pos) {
    uint8_t newpos;
    switch (pos) {
        case kMitsubishiHeavy88SwingVAuto:
        case kMitsubishiHeavy88SwingVHighest:
        case kMitsubishiHeavy88SwingVHigh:
        case kMitsubishiHeavy88SwingVMiddle:
        case kMitsubishiHeavy88SwingVLow:
        case kMitsubishiHeavy88SwingVLowest: newpos = pos; break;
        default: newpos = kMitsubishiHeavy88SwingVOff;
    }
    _.SwingV5 = newpos;
    _.SwingV7 = (newpos >> kMitsubishiHeavy88SwingVByte5Size);
}

/// Get the Vertical Swing mode of the A/C.
/// @return The native position/mode setting.
uint8_t IRMitsubishiHeavy88Ac::getSwingVertical(void) const {
    return _.SwingV5 | (_.SwingV7 << kMitsubishiHeavy88SwingVByte5Size);
}

/// Set the Horizontal Swing mode of the A/C.
/// @param[in] pos The position/mode to set the swing to.
void IRMitsubishiHeavy88Ac::setSwingHorizontal(const uint8_t pos) {
    uint8_t newpos;
    switch (pos) {
        case kMitsubishiHeavy88SwingHAuto:
        case kMitsubishiHeavy88SwingHLeftMax:
        case kMitsubishiHeavy88SwingHLeft:
        case kMitsubishiHeavy88SwingHMiddle:
        case kMitsubishiHeavy88SwingHRight:
        case kMitsubishiHeavy88SwingHRightMax:
        case kMitsubishiHeavy88SwingHLeftRight:
        case kMitsubishiHeavy88SwingHRightLeft:
        case kMitsubishiHeavy88SwingH3D: newpos = pos; break;
        default:                         newpos = kMitsubishiHeavy88SwingHOff;
    }
    _.SwingH1 = newpos;
    _.SwingH2 = (newpos >> kMitsubishiHeavy88SwingHSize);
}

/// Get the Horizontal Swing mode of the A/C.
/// @return The native position/mode setting.
uint8_t IRMitsubishiHeavy88Ac::getSwingHorizontal(void) const {
    return _.SwingH1 | (_.SwingH2 << kMitsubishiHeavy88SwingHSize);
}

/// Set the Turbo mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMitsubishiHeavy88Ac::setTurbo(const bool on) {
    if (on)
        setFan(kMitsubishiHeavy88FanTurbo);
    else if (getTurbo()) setFan(kMitsubishiHeavy88FanAuto);
}

/// Get the Turbo mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRMitsubishiHeavy88Ac::getTurbo(void) const {
    return _.Fan == kMitsubishiHeavy88FanTurbo;
}

/// Set the Economical mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMitsubishiHeavy88Ac::setEcono(const bool on) {
    if (on)
        setFan(kMitsubishiHeavy88FanEcono);
    else if (getEcono()) setFan(kMitsubishiHeavy88FanAuto);
}

/// Get the Economical mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRMitsubishiHeavy88Ac::getEcono(void) const {
    return _.Fan == kMitsubishiHeavy88FanEcono;
}

/// Set the 3D mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMitsubishiHeavy88Ac::set3D(const bool on) {
    if (on)
        setSwingHorizontal(kMitsubishiHeavy88SwingH3D);
    else if (get3D())
        setSwingHorizontal(kMitsubishiHeavy88SwingHOff);
}

/// Get the 3D mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRMitsubishiHeavy88Ac::get3D(void) const {
    return getSwingHorizontal() == kMitsubishiHeavy88SwingH3D;
}

/// Set the Clean mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMitsubishiHeavy88Ac::setClean(const bool on) {
    _.Clean = on;
}

/// Get the Clean mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRMitsubishiHeavy88Ac::getClean(void) const {
    return _.Clean;
}

/// Verify the given state has a ZJ-S signature.
/// @param[in] state A ptr to a state to be checked.
/// @return true, the check passed. Otherwise, false.
bool IRMitsubishiHeavy88Ac::checkZjsSig(const uint8_t *state) {
    for (uint8_t i = 0; i < kMitsubishiHeavySigLength; i++)
        if (state[i] != kMitsubishiHeavyZjsSig[i]) return false;
    return true;
}

/// Calculate the checksum for the current internal state of the remote.
/// Note: Technically it has no checksum, but does have inverted byte pairs.
void IRMitsubishiHeavy88Ac::checksum(void) {
    const uint8_t kOffset = kMitsubishiHeavySigLength - 2;
    invertBytePairs(_.raw + kOffset, kMitsubishiHeavy88StateLength - kOffset);
}

/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @param[in] length The length/size of the state array.
/// @return true, if the state has a valid checksum. Otherwise, false.
/// Note: Technically it has no checksum, but does have inverted byte pairs.
bool IRMitsubishiHeavy88Ac::validChecksum(const uint8_t *state,
                                          const uint16_t length) {
    return IRMitsubishiHeavy152Ac::validChecksum(state, length);
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRMitsubishiHeavy88Ac::convertMode(const stdAc::opmode_t mode) {
    return IRMitsubishiHeavy152Ac::convertMode(mode);
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRMitsubishiHeavy88Ac::convertFan(const stdAc::fanspeed_t speed) {
    switch (speed) {
        // Assumes Econo is slower than Low.
        case stdAc::fanspeed_t::kMin:    return kMitsubishiHeavy88FanEcono;
        case stdAc::fanspeed_t::kLow:    return kMitsubishiHeavy88FanLow;
        case stdAc::fanspeed_t::kMedium: return kMitsubishiHeavy88FanMed;
        case stdAc::fanspeed_t::kHigh:   return kMitsubishiHeavy88FanHigh;
        case stdAc::fanspeed_t::kMax:    return kMitsubishiHeavy88FanTurbo;
        default:                         return kMitsubishiHeavy88FanAuto;
    }
}

/// Convert a stdAc::swingv_t enum into it's native setting.
/// @param[in] position The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRMitsubishiHeavy88Ac::convertSwingV(const stdAc::swingv_t position) {
    switch (position) {
        case stdAc::swingv_t::kAuto:    return kMitsubishiHeavy88SwingVAuto;
        case stdAc::swingv_t::kHighest: return kMitsubishiHeavy88SwingVHighest;
        case stdAc::swingv_t::kHigh:    return kMitsubishiHeavy88SwingVHigh;
        case stdAc::swingv_t::kMiddle:  return kMitsubishiHeavy88SwingVMiddle;
        case stdAc::swingv_t::kLow:     return kMitsubishiHeavy88SwingVLow;
        case stdAc::swingv_t::kLowest:  return kMitsubishiHeavy88SwingVLowest;
        default:                        return kMitsubishiHeavy88SwingVOff;
    }
}

/// Convert a stdAc::swingh_t enum into it's native setting.
/// @param[in] position The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRMitsubishiHeavy88Ac::convertSwingH(const stdAc::swingh_t position) {
    switch (position) {
        case stdAc::swingh_t::kAuto:     return kMitsubishiHeavy88SwingHAuto;
        case stdAc::swingh_t::kLeftMax:  return kMitsubishiHeavy88SwingHLeftMax;
        case stdAc::swingh_t::kLeft:     return kMitsubishiHeavy88SwingHLeft;
        case stdAc::swingh_t::kMiddle:   return kMitsubishiHeavy88SwingHMiddle;
        case stdAc::swingh_t::kRight:    return kMitsubishiHeavy88SwingHRight;
        case stdAc::swingh_t::kRightMax: return kMitsubishiHeavy88SwingHRightMax;
        default:                         return kMitsubishiHeavy88SwingHOff;
    }
}


/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/


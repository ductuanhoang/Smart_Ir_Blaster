/*
 * IrMitsubishi112.cpp
 *
 *  Created on: Mar 9, 2021
 *      Author: ductu
 */




/***********************************************************************************************************************
 * Pragma directive
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes <System Includes>
 ***********************************************************************************************************************/
#include "IrMitsubishi112.h"
#include "IRutils.h"
#include "IrTCL112Ac.h"
/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/
// Constants
// Mitsubishi TV
// period time is 1/33000Hz = 30.303 uSeconds (T)
const uint16_t kMitsubishiTick = 30;
const uint16_t kMitsubishiBitMarkTicks = 10;
const uint16_t kMitsubishiBitMark = kMitsubishiBitMarkTicks * kMitsubishiTick;
const uint16_t kMitsubishiOneSpaceTicks = 70;
const uint16_t kMitsubishiOneSpace = kMitsubishiOneSpaceTicks * kMitsubishiTick;
const uint16_t kMitsubishiZeroSpaceTicks = 30;
const uint16_t kMitsubishiZeroSpace =
        kMitsubishiZeroSpaceTicks * kMitsubishiTick;
const uint16_t kMitsubishiMinCommandLengthTicks = 1786;
const uint16_t kMitsubishiMinCommandLength =
        kMitsubishiMinCommandLengthTicks * kMitsubishiTick;
const uint16_t kMitsubishiMinGapTicks = 936;
const uint16_t kMitsubishiMinGap = kMitsubishiMinGapTicks * kMitsubishiTick;

// Mitsubishi Projector (HC3000)
const uint16_t kMitsubishi2HdrMark = 8400;
const uint16_t kMitsubishi2HdrSpace = kMitsubishi2HdrMark / 2;
const uint16_t kMitsubishi2BitMark = 560;
const uint16_t kMitsubishi2ZeroSpace = 520;
const uint16_t kMitsubishi2OneSpace = kMitsubishi2ZeroSpace * 3;
const uint16_t kMitsubishi2MinGap = 28500;

// Mitsubishi A/C
const uint16_t kMitsubishiAcHdrMark = 3400;
const uint16_t kMitsubishiAcHdrSpace = 1750;
const uint16_t kMitsubishiAcBitMark = 450;
const uint16_t kMitsubishiAcOneSpace = 1300;
const uint16_t kMitsubishiAcZeroSpace = 420;
const uint16_t kMitsubishiAcRptMark = 440;
const uint16_t kMitsubishiAcRptSpace = 17100;
const uint8_t  kMitsubishiAcExtraTolerance = 5;

// Mitsubishi 136 bit A/C
const uint16_t kMitsubishi136HdrMark = 3324;
const uint16_t kMitsubishi136HdrSpace = 1474;
const uint16_t kMitsubishi136BitMark = 467;
const uint16_t kMitsubishi136OneSpace = 1137;
const uint16_t kMitsubishi136ZeroSpace = 351;
const uint32_t kMitsubishi136Gap = kDefaultMessageGap;

// Mitsubishi 112 bit A/C
const uint16_t kMitsubishi112HdrMark = 3450;
const uint16_t kMitsubishi112HdrSpace = 1696;
const uint16_t kMitsubishi112BitMark = 450;
const uint16_t kMitsubishi112OneSpace = 1250;
const uint16_t kMitsubishi112ZeroSpace = 385;
const uint32_t kMitsubishi112Gap = kDefaultMessageGap;
// Total tolerance percentage to use for matching the header mark.
const uint8_t  kMitsubishi112HdrMarkTolerance = 5;
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

IRMitsubishiAC::IRMitsubishiAC(uint32_t frequency, uint16_t dutycycle):_ir_remote(frequency, dutycycle) {
    stateReset();
}

IRMitsubishiAC::~IRMitsubishiAC() {

}

IRMitsubishi136::IRMitsubishi136(uint32_t frequency, uint16_t dutycycle):_ir_remote(frequency, dutycycle) {
    stateReset();
}

IRMitsubishi136::~IRMitsubishi136() {

}

IRMitsubishi112::IRMitsubishi112(uint32_t frequency, uint16_t dutycycle):_ir_remote(frequency, dutycycle) {
    stateReset();
}

IRMitsubishi112::~IRMitsubishi112() {

}

void IRMitsubishiAC::setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp) {
    setPower(power);
    setMode(convertMode(stdAc::opmode_t(mode)));
    setFan(convertFan(stdAc::fanspeed_t(speed)));
    setTemp(temp);
}

void IRMitsubishi112::setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp) {
    setPower(power);
    setMode(convertMode(stdAc::opmode_t(mode)));
    setFan(convertFan(stdAc::fanspeed_t(speed)));
    setTemp(temp);
}

void IRMitsubishi136::setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp) {
    setPower(power);
    setMode(convertMode(stdAc::opmode_t(mode)));
    setFan(convertFan(stdAc::fanspeed_t(speed)));
    setTemp(temp);
}
/***********************************************************************************************************************
 * Static Functions
 ***********************************************************************************************************************/


#if SEND_MITSUBISHI
/// Send the supplied Mitsubishi 16-bit message.
/// Status: STABLE / Working.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @note This protocol appears to have no header.
/// @see https://github.com/marcosamarinho/IRremoteESP8266/blob/master/ir_Mitsubishi.cpp
/// @see GlobalCache's Control Tower's Mitsubishi TV data.
void IRsend::sendMitsubishi(uint64_t data, uint16_t nbits, uint16_t repeat) {
  sendGeneric(0, 0,  // No Header
              kMitsubishiBitMark, kMitsubishiOneSpace, kMitsubishiBitMark,
              kMitsubishiZeroSpace, kMitsubishiBitMark, kMitsubishiMinGap,
              kMitsubishiMinCommandLength, data, nbits, 33, true, repeat, 50);
}
#endif  // SEND_MITSUBISHI


#if SEND_MITSUBISHI2
/// Send a supplied second variant Mitsubishi 16-bit message.
/// Status: BETA / Probably works.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @note Based on a Mitsubishi HC3000 projector's remote.
///   This protocol appears to have a mandatory in-protocol repeat.
///   That is in *addition* to the entire message needing to be sent twice
///   for the device to accept the command. That is separate from the repeat.
///   i.e. Allegedly, the real remote requires the "Off" button pressed twice.
///        You will need to add a suitable gap yourself.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/441
void IRsend::sendMitsubishi2(uint64_t data, uint16_t nbits, uint16_t repeat) {
  for (uint16_t i = 0; i <= repeat; i++) {
    // First half of the data.
    sendGeneric(kMitsubishi2HdrMark, kMitsubishi2HdrSpace, kMitsubishi2BitMark,
                kMitsubishi2OneSpace, kMitsubishi2BitMark,
                kMitsubishi2ZeroSpace, kMitsubishi2BitMark,
                kMitsubishi2HdrSpace, data >> (nbits / 2), nbits / 2, 33, true,
                0, 50);
    // Second half of the data.
    sendGeneric(0, 0,  // No header for the second data block
                kMitsubishi2BitMark, kMitsubishi2OneSpace, kMitsubishi2BitMark,
                kMitsubishi2ZeroSpace, kMitsubishi2BitMark, kMitsubishi2MinGap,
                data & ((1 << (nbits / 2)) - 1), nbits / 2, 33, true, 0, 50);
  }
}
#endif  // SEND_MITSUBISHI2


/// Send a Mitsubishi 144-bit A/C formatted message. (MITSUBISHI_AC)
/// Status: STABLE / Working.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRMitsubishiAC::send(const unsigned char data[], const uint16_t nbytes,
                              const uint16_t repeat) {
  if (nbytes < kMitsubishiACStateLength)
    return;  // Not enough bytes to send a proper message.

  _ir_remote.sendGeneric(kMitsubishiAcHdrMark, kMitsubishiAcHdrSpace, kMitsubishiAcBitMark,
              kMitsubishiAcOneSpace, kMitsubishiAcBitMark,
              kMitsubishiAcZeroSpace, kMitsubishiAcRptMark,
              kMitsubishiAcRptSpace, data, nbytes, 38, false, repeat, 50);
}



// Code to emulate Mitsubishi A/C IR remote control unit.
// Inspired and derived from the work done at:
//   https://github.com/r45635/HVAC-IR-Control

/// Reset the state of the remote to a known good state/sequence.
void IRMitsubishiAC::stateReset(void) {
    // The state of the IR remote in IR code form.
    // Known good state obtained from:
    //   https://github.com/r45635/HVAC-IR-Control/blob/master/HVAC_ESP8266/HVAC_ESP8266.ino#L108
    static const uint8_t kReset[kMitsubishiACStateLength] = {
            0x23, 0xCB, 0x26, 0x01, 0x00, 0x20, 0x08, 0x06, 0x30, 0x45, 0x67};
    setRaw(kReset);
}

/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRMitsubishiAC::sendMitsubishiAC(const uint16_t repeat) {
  send(getRaw(), kMitsubishiACStateLength, repeat);
}

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t *IRMitsubishiAC::getRaw(void) {
    checksum();
    return _.raw;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] data A valid code for this protocol.
void IRMitsubishiAC::setRaw(const uint8_t *data) {
    memcpy(_.raw, data, kMitsubishiACStateLength);
}

/// Calculate and set the checksum values for the internal state.
void IRMitsubishiAC::checksum(void) {
    _.Sum = calculateChecksum(_.raw);
}

/// Verify the checksum is valid for a given state.
/// @param[in] data The array to verify the checksum of.
/// @return true, if the state has a valid checksum. Otherwise, false.
bool IRMitsubishiAC::validChecksum(const uint8_t *data) {
    return calculateChecksum(data) == data[kMitsubishiACStateLength - 1];
}

/// Calculate the checksum for a given state.
/// @param[in] data The value to calc the checksum of.
/// @return The calculated checksum value.
uint8_t IRMitsubishiAC::calculateChecksum(const uint8_t *data) {
    return sumBytes(data, kMitsubishiACStateLength - 1);
}

/// Set the requested power state of the A/C to on.
void IRMitsubishiAC::on(void) { setPower(true); }

/// Set the requested power state of the A/C to off.
void IRMitsubishiAC::off(void) { setPower(false); }

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMitsubishiAC::setPower(bool on) {
    _.Power = on;
}

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRMitsubishiAC::getPower(void) const {
    return _.Power;
}

/// Set the temperature.
/// @param[in] degrees The temperature in degrees celsius.
void IRMitsubishiAC::setTemp(const uint8_t degrees) {
    uint8_t temp = std::max(kMitsubishiAcMinTemp, degrees);
    temp = std::min(kMitsubishiAcMaxTemp, temp);
    _.Temp = temp - kMitsubishiAcMinTemp;
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRMitsubishiAC::getTemp(void) const {
    return (_.Temp + kMitsubishiAcMinTemp);
}

/// Set the speed of the fan.
/// @param[in] speed The desired setting. 0 is auto, 1-5 is speed, 6 is silent.
void IRMitsubishiAC::setFan(const uint8_t speed) {
    uint8_t fan = speed;
    // Bounds check
    if (fan > kMitsubishiAcFanSilent)
        fan = kMitsubishiAcFanMax;        // Set the fan to maximum if out of range.
    // Auto has a special bit.
    _.FanAuto = (fan == kMitsubishiAcFanAuto);
    if (fan >= kMitsubishiAcFanMax)
        fan--;  // There is no spoon^H^H^Heed 5 (max), pretend it doesn't exist.
    _.Fan = fan;
}

/// Get the current fan speed setting.
/// @return The current fan speed/mode.
uint8_t IRMitsubishiAC::getFan(void) const {
    uint8_t fan = _.Fan;
    if (fan == kMitsubishiAcFanMax) return kMitsubishiAcFanSilent;
    return fan;
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRMitsubishiAC::getMode(void) const {
    return _.Mode;
}

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRMitsubishiAC::setMode(const uint8_t mode) {
    // If we get an unexpected mode, default to AUTO.
    switch (mode) {
        case kMitsubishiAcAuto: _.raw[8] = 0b00110000; break;
        case kMitsubishiAcCool: _.raw[8] = 0b00110110; break;
        case kMitsubishiAcDry:  _.raw[8] = 0b00110010; break;
        case kMitsubishiAcHeat: _.raw[8] = 0b00110000; break;
        default:
            _.raw[8] = 0b00110000;
            _.Mode = kMitsubishiAcAuto;
            return;
    }
    _.Mode = mode;
}

/// Set the requested vane (Vertical Swing) operation mode of the a/c unit.
/// @param[in] position The position/mode to set the vane to.
void IRMitsubishiAC::setVane(const uint8_t position) {
    uint8_t pos = std::min(position, kMitsubishiAcVaneAutoMove);  // bounds check
    _.VaneBit = 1;
    _.Vane = pos;
}

/// Set the requested wide-vane (Horizontal Swing) operation mode of the a/c.
/// @param[in] position The position/mode to set the wide vane to.
void IRMitsubishiAC::setWideVane(const uint8_t position) {
    _.WideVane = std::min(position, kMitsubishiAcWideVaneAuto);
}

/// Get the Vane (Vertical Swing) mode of the A/C.
/// @return The native position/mode setting.
uint8_t IRMitsubishiAC::getVane(void) const {
    return _.Vane;
}

/// Get the Wide Vane (Horizontal Swing) mode of the A/C.
/// @return The native position/mode setting.
uint8_t IRMitsubishiAC::getWideVane(void) const {
    return _.WideVane;
}

/// Get the clock time of the A/C unit.
/// @return Nr. of 10 minute increments past midnight.
/// @note 1 = 1/6 hour (10 minutes). e.g. 4pm = 48.
uint8_t IRMitsubishiAC::getClock(void) const { return _.Clock; }

/// Set the clock time on the A/C unit.
/// @param[in] clock Nr. of 10 minute increments past midnight.
/// @note 1 = 1/6 hour (10 minutes). e.g. 6am = 36.
void IRMitsubishiAC::setClock(const uint8_t clock) {
    _.Clock = clock;
}

/// Get the desired start time of the A/C unit.
/// @return Nr. of 10 minute increments past midnight.
/// @note 1 = 1/6 hour (10 minutes). e.g. 4pm = 48.
uint8_t IRMitsubishiAC::getStartClock(void) const { return _.StartClock; }

/// Set the desired start time of the A/C unit.
/// @param[in] clock Nr. of 10 minute increments past midnight.
/// @note 1 = 1/6 hour (10 minutes). e.g. 8pm = 120.
void IRMitsubishiAC::setStartClock(const uint8_t clock) {
    _.StartClock = clock;
}

/// Get the desired stop time of the A/C unit.
/// @return Nr. of 10 minute increments past midnight.
/// @note 1 = 1/6 hour (10 minutes). e.g. 10pm = 132.
uint8_t IRMitsubishiAC::getStopClock(void) const { return _.StopClock; }

/// Set the desired stop time of the A/C unit.
/// @param[in] clock Nr. of 10 minute increments past midnight.
/// @note 1 = 1/6 hour (10 minutes). e.g. 10pm = 132.
void IRMitsubishiAC::setStopClock(const uint8_t clock) {
    _.StopClock = clock;
}

/// Get the timers active setting of the A/C.
/// @return The current timers enabled.
/// @note Possible values: kMitsubishiAcNoTimer,
///   kMitsubishiAcStartTimer, kMitsubishiAcStopTimer,
///   kMitsubishiAcStartStopTimer
uint8_t IRMitsubishiAC::getTimer(void) const {
    return _.Timer;
}

/// Set the timers active setting of the A/C.
/// @param[in] timer The timer code indicating which ones are active.
/// @note Possible values: kMitsubishiAcNoTimer,
///   kMitsubishiAcStartTimer, kMitsubishiAcStopTimer,
///   kMitsubishiAcStartStopTimer
void IRMitsubishiAC::setTimer(const uint8_t timer) {
    _.Timer = timer;
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRMitsubishiAC::convertMode(const stdAc::opmode_t mode) {
    switch (mode) {
        case stdAc::opmode_t::kCool: return kMitsubishiAcCool;
        case stdAc::opmode_t::kHeat: return kMitsubishiAcHeat;
        case stdAc::opmode_t::kDry:  return kMitsubishiAcDry;
        default:                     return kMitsubishiAcAuto;
    }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRMitsubishiAC::convertFan(const stdAc::fanspeed_t speed) {
    switch (speed) {
        case stdAc::fanspeed_t::kMin:    return kMitsubishiAcFanSilent;
        case stdAc::fanspeed_t::kLow:    return kMitsubishiAcFanRealMax - 3;
        case stdAc::fanspeed_t::kMedium: return kMitsubishiAcFanRealMax - 2;
        case stdAc::fanspeed_t::kHigh:   return kMitsubishiAcFanRealMax - 1;
        case stdAc::fanspeed_t::kMax:    return kMitsubishiAcFanRealMax;
        default:                         return kMitsubishiAcFanAuto;
    }
}


/// Convert a stdAc::swingv_t enum into it's native setting.
/// @param[in] position The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRMitsubishiAC::convertSwingV(const stdAc::swingv_t position) {
    switch (position) {
        case stdAc::swingv_t::kHighest: return kMitsubishiAcVaneAutoMove - 6;
        case stdAc::swingv_t::kHigh:    return kMitsubishiAcVaneAutoMove - 5;
        case stdAc::swingv_t::kMiddle:  return kMitsubishiAcVaneAutoMove - 4;
        case stdAc::swingv_t::kLow:     return kMitsubishiAcVaneAutoMove - 3;
        case stdAc::swingv_t::kLowest:  return kMitsubishiAcVaneAutoMove - 2;
        case stdAc::swingv_t::kAuto:    return kMitsubishiAcVaneAutoMove;
        default:                        return kMitsubishiAcVaneAuto;
    }
}

/// Convert a stdAc::swingh_t enum into it's native setting.
/// @param[in] position The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRMitsubishiAC::convertSwingH(const stdAc::swingh_t position) {
    switch (position) {
        case stdAc::swingh_t::kLeftMax:  return kMitsubishiAcWideVaneAuto - 7;
        case stdAc::swingh_t::kLeft:     return kMitsubishiAcWideVaneAuto - 6;
        case stdAc::swingh_t::kMiddle:   return kMitsubishiAcWideVaneAuto - 5;
        case stdAc::swingh_t::kRight:    return kMitsubishiAcWideVaneAuto - 4;
        case stdAc::swingh_t::kRightMax: return kMitsubishiAcWideVaneAuto - 3;
        case stdAc::swingh_t::kWide:     return kMitsubishiAcWideVaneAuto - 2;
        case stdAc::swingh_t::kAuto:     return kMitsubishiAcWideVaneAuto;
        default:                         return kMitsubishiAcWideVaneAuto - 5;
    }
}



/// Send a Mitsubishi 136-bit A/C message. (MITSUBISHI136)
/// Status: BETA / Probably working. Needs to be tested against a real device.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/888
void IRMitsubishi136::send(const unsigned char data[],
                               const uint16_t nbytes,
                               const uint16_t repeat) {
  if (nbytes < kMitsubishi136StateLength)
    return;  // Not enough bytes to send a proper message.

  _ir_remote.sendGeneric(kMitsubishi136HdrMark, kMitsubishi136HdrSpace,
              kMitsubishi136BitMark, kMitsubishi136OneSpace,
              kMitsubishi136BitMark, kMitsubishi136ZeroSpace,
              kMitsubishi136BitMark, kMitsubishi136Gap,
              data, nbytes, 38, false, repeat, 50);
}


// Code to emulate Mitsubishi 136bit A/C IR remote control unit.


/// Reset the state of the remote to a known good state/sequence.
void IRMitsubishi136::stateReset(void) {
    // The state of the IR remote in IR code form.
    // Known good state obtained from:
    //   https://docs.google.com/spreadsheets/d/1f8EGfIbBUo2B-CzUFdrgKQprWakoYNKM80IKZN4KXQE/edit#gid=312397579&range=A10
    static const uint8_t kReset[kMitsubishi136StateLength] = {
            0x23, 0xCB, 0x26, 0x21, 0x00, 0x40, 0xC2, 0xC7, 0x04};
    memcpy(_.raw, kReset, kMitsubishi136StateLength);
}

/// Calculate the checksum for the current internal state of the remote.
void IRMitsubishi136::checksum(void) {
    for (uint8_t i = 0; i < 6; i++)
        _.raw[kMitsubishi136PowerByte + 6 + i] =
                ~_.raw[kMitsubishi136PowerByte + i];
}

/// Verify the checksum is valid for a given state.
/// @param[in] data The array to verify the checksum of.
/// @param[in] len The length of the data array.
/// @return true, if the state has a valid checksum. Otherwise, false.
bool IRMitsubishi136::validChecksum(const uint8_t *data, const uint16_t len) {
    if (len < kMitsubishi136StateLength) return false;
    const uint16_t half = (len - kMitsubishi136PowerByte) / 2;
    for (uint8_t i = 0; i < half; i++) {
        // This variable is needed to avoid the warning: (known compiler issue)
        // warning: comparison of promoted ~unsigned with unsigned [-Wsign-compare]
        const uint8_t inverted = ~data[kMitsubishi136PowerByte + half + i];
        if (data[kMitsubishi136PowerByte + i] != inverted) return false;
    }
    return true;
}



/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRMitsubishi136::sendMitsubishi136(const uint16_t repeat) {
  send(getRaw(), kMitsubishi136StateLength, repeat);
}


/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t *IRMitsubishi136::getRaw(void) {
    checksum();
    return _.raw;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] data A valid code for this protocol.
void IRMitsubishi136::setRaw(const uint8_t *data) {
    memcpy(_.raw, data, kMitsubishi136StateLength);
}

/// Set the requested power state of the A/C to on.
void IRMitsubishi136::on(void) { setPower(true); }

/// Set the requested power state of the A/C to off.
void IRMitsubishi136::off(void) { setPower(false); }

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMitsubishi136::setPower(bool on) {
    _.Power = on;
}

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRMitsubishi136::getPower(void) const {
    return _.Power;
}

/// Set the temperature.
/// @param[in] degrees The temperature in degrees celsius.
void IRMitsubishi136::setTemp(const uint8_t degrees) {
    uint8_t temp = std::max((uint8_t)kMitsubishi136MinTemp, degrees);
    temp = std::min((uint8_t)kMitsubishi136MaxTemp, temp);
    _.Temp = temp - kMitsubishiAcMinTemp;
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRMitsubishi136::getTemp(void) const {
    return _.Temp + kMitsubishiAcMinTemp;
}

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
void IRMitsubishi136::setFan(const uint8_t speed) {
    _.Fan = std::min(speed, kMitsubishi136FanMax);
}

/// Get the current fan speed setting.
/// @return The current fan speed/mode.
uint8_t IRMitsubishi136::getFan(void) const {
    return _.Fan;
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRMitsubishi136::getMode(void) const {
    return _.Mode;
}

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRMitsubishi136::setMode(const uint8_t mode) {
    // If we get an unexpected mode, default to AUTO.
    switch (mode) {
        case kMitsubishi136Fan:
        case kMitsubishi136Cool:
        case kMitsubishi136Heat:
        case kMitsubishi136Auto:
        case kMitsubishi136Dry:
            _.Mode = mode;
            break;
        default:
            _.Mode = kMitsubishi136Auto;
    }
}

/// Set the Vertical Swing mode of the A/C.
/// @param[in] position The position/mode to set the swing to.
void IRMitsubishi136::setSwingV(const uint8_t position) {
    // If we get an unexpected mode, default to auto.
    switch (position) {
        case kMitsubishi136SwingVLowest:
        case kMitsubishi136SwingVLow:
        case kMitsubishi136SwingVHigh:
        case kMitsubishi136SwingVHighest:
        case kMitsubishi136SwingVAuto:
            _.SwingV = position;
            break;
        default:
            _.SwingV = kMitsubishi136SwingVAuto;
    }
}

/// Get the Vertical Swing mode of the A/C.
/// @return The native position/mode setting.
uint8_t IRMitsubishi136::getSwingV(void) const {
    return _.SwingV;
}

/// Set the Quiet mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMitsubishi136::setQuiet(bool on) {
    if (on) setFan(kMitsubishi136FanQuiet);
    else if (getQuiet()) setFan(kMitsubishi136FanLow);
}


/// Get the Quiet mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRMitsubishi136::getQuiet(void) const {
    return _.Fan == kMitsubishi136FanQuiet;
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRMitsubishi136::convertMode(const stdAc::opmode_t mode) {
    switch (mode) {
        case stdAc::opmode_t::kCool: return kMitsubishi136Cool;
        case stdAc::opmode_t::kHeat: return kMitsubishi136Heat;
        case stdAc::opmode_t::kDry:  return kMitsubishi136Dry;
        case stdAc::opmode_t::kFan:  return kMitsubishi136Fan;
        default:                     return kMitsubishi136Auto;
    }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRMitsubishi136::convertFan(const stdAc::fanspeed_t speed) {
    switch (speed) {
        case stdAc::fanspeed_t::kMin: return kMitsubishi136FanMin;
        case stdAc::fanspeed_t::kLow: return kMitsubishi136FanLow;
        case stdAc::fanspeed_t::kHigh:
        case stdAc::fanspeed_t::kMax: return kMitsubishi136FanMax;
        default:                      return kMitsubishi136FanMed;
    }
}

/// Convert a stdAc::swingv_t enum into it's native setting.
/// @param[in] position The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRMitsubishi136::convertSwingV(const stdAc::swingv_t position) {
    switch (position) {
        case stdAc::swingv_t::kHighest: return kMitsubishi136SwingVHighest;
        case stdAc::swingv_t::kHigh:
        case stdAc::swingv_t::kMiddle:  return kMitsubishi136SwingVHigh;
        case stdAc::swingv_t::kLow:     return kMitsubishi136SwingVLow;
        case stdAc::swingv_t::kLowest:  return kMitsubishi136SwingVLowest;
        default:                        return kMitsubishi136SwingVAuto;
    }
}


/// Send a Mitsubishi 112-bit A/C formatted message. (MITSUBISHI112)
/// Status: Stable / Reported as working.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/947
void IRMitsubishi112::send(const unsigned char data[],
                               const uint16_t nbytes,
                               const uint16_t repeat) {
  if (nbytes < kMitsubishi112StateLength)
    return;  // Not enough bytes to send a proper message.

  _ir_remote.sendGeneric(kMitsubishi112HdrMark, kMitsubishi112HdrSpace,
              kMitsubishi112BitMark, kMitsubishi112OneSpace,
              kMitsubishi112BitMark, kMitsubishi112ZeroSpace,
              kMitsubishi112BitMark, kMitsubishi112Gap,
              data, nbytes, 38, false, repeat, 50);
}


/// Reset the state of the remote to a known good state/sequence.
void IRMitsubishi112::stateReset(void) {
    const uint8_t kReset[kMitsubishi112StateLength] = {
            0x23, 0xCB, 0x26, 0x01, 0x00, 0x24, 0x03, 0x0B, 0x10,
            0x00, 0x00, 0x00, 0x30};
    setRaw(kReset);
}

/// Calculate the checksum for the current internal state of the remote.
void IRMitsubishi112::checksum(void) {
    _.Sum = Ir_TCL_112Ac::calcChecksum(_.raw, kMitsubishi112StateLength);
}



/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRMitsubishi112::sendMitsubishi112(const uint16_t repeat) {
  send(getRaw(), kMitsubishi112StateLength, repeat);
}


/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t *IRMitsubishi112::getRaw(void) {
    checksum();
    return _.raw;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] data A valid code for this protocol.
void IRMitsubishi112::setRaw(const uint8_t *data) {
    memcpy(_.raw, data, kMitsubishi112StateLength);
}

/// Set the requested power state of the A/C to off.
void IRMitsubishi112::on(void) { setPower(true); }

/// Set the requested power state of the A/C to off.
void IRMitsubishi112::off(void) { setPower(false); }

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMitsubishi112::setPower(bool on) {
    _.Power = on;
}

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRMitsubishi112::getPower(void) const {
    return _.Power;
}

/// Set the temperature.
/// @param[in] degrees The temperature in degrees celsius.
void IRMitsubishi112::setTemp(const uint8_t degrees) {
    uint8_t temp = std::max((uint8_t)kMitsubishi112MinTemp, degrees);
    temp = std::min((uint8_t)kMitsubishi112MaxTemp, temp);
    _.Temp = kMitsubishiAcMaxTemp - temp;
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRMitsubishi112::getTemp(void) const {
    return kMitsubishiAcMaxTemp - _.Temp;
}

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
void IRMitsubishi112::setFan(const uint8_t speed) {
    switch (speed) {
        case kMitsubishi112FanMin:
        case kMitsubishi112FanLow:
        case kMitsubishi112FanMed:
        case kMitsubishi112FanMax:
            _.Fan = speed;
            break;
        default:
            _.Fan = kMitsubishi112FanMax;
    }
}

/// Get the current fan speed setting.
/// @return The current fan speed/mode.
uint8_t IRMitsubishi112::getFan(void) const {
    return _.Fan;
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRMitsubishi112::getMode(void) const {
    return _.Mode;
}

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRMitsubishi112::setMode(const uint8_t mode) {
    // If we get an unexpected mode, default to AUTO.
    switch (mode) {
        // Note: No Fan Only mode.
        case kMitsubishi112Cool:
        case kMitsubishi112Heat:
        case kMitsubishi112Auto:
        case kMitsubishi112Dry:
            _.Mode = mode;
            break;
        default:
            _.Mode = kMitsubishi112Auto;
    }
}

/// Set the Vertical Swing mode of the A/C.
/// @param[in] position The position/mode to set the swing to.
void IRMitsubishi112::setSwingV(const uint8_t position) {
    // If we get an unexpected mode, default to auto.
    switch (position) {
        case kMitsubishi112SwingVLowest:
        case kMitsubishi112SwingVLow:
        case kMitsubishi112SwingVMiddle:
        case kMitsubishi112SwingVHigh:
        case kMitsubishi112SwingVHighest:
        case kMitsubishi112SwingVAuto:
            _.SwingV = position;
            break;
        default:
            _.SwingV = kMitsubishi112SwingVAuto;
    }
}

/// Get the Vertical Swing mode of the A/C.
/// @return The native position/mode setting.
uint8_t IRMitsubishi112::getSwingV(void) const {
    return _.SwingV;
}

/// Set the Horizontal Swing mode of the A/C.
/// @param[in] position The position/mode to set the swing to.
void IRMitsubishi112::setSwingH(const uint8_t position) {
    // If we get an unexpected mode, default to auto.
    switch (position) {
        case kMitsubishi112SwingHLeftMax:
        case kMitsubishi112SwingHLeft:
        case kMitsubishi112SwingHMiddle:
        case kMitsubishi112SwingHRight:
        case kMitsubishi112SwingHRightMax:
        case kMitsubishi112SwingHWide:
        case kMitsubishi112SwingHAuto:
            _.SwingH = position;
            break;
        default:
            _.SwingH = kMitsubishi112SwingHAuto;
    }
}


/// Get the Horizontal Swing mode of the A/C.
/// @return The native position/mode setting.
uint8_t IRMitsubishi112::getSwingH(void) const {
    return _.SwingH;
}

/// Set the Quiet mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
/// @note There is no true quiet setting on this A/C.
void IRMitsubishi112::setQuiet(bool on) {
    if (on)
        setFan(kMitsubishi112FanQuiet);
    else if (getQuiet()) setFan(kMitsubishi112FanLow);
}


/// Get the Quiet mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
/// @note There is no true quiet setting on this A/C.
bool IRMitsubishi112::getQuiet(void) const {
    return _.Fan == kMitsubishi112FanQuiet;
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRMitsubishi112::convertMode(const stdAc::opmode_t mode) {
    switch (mode) {
        case stdAc::opmode_t::kCool: return kMitsubishi112Cool;
        case stdAc::opmode_t::kHeat: return kMitsubishi112Heat;
        case stdAc::opmode_t::kDry: return kMitsubishi112Dry;
            // Note: No Fan Only mode.
        default: return kMitsubishi112Auto;
    }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRMitsubishi112::convertFan(const stdAc::fanspeed_t speed) {
    switch (speed) {
        case stdAc::fanspeed_t::kMin: return kMitsubishi112FanMin;
        case stdAc::fanspeed_t::kLow: return kMitsubishi112FanLow;
        case stdAc::fanspeed_t::kMedium: return kMitsubishi112FanMed;
        case stdAc::fanspeed_t::kHigh:
        case stdAc::fanspeed_t::kMax: return kMitsubishi112FanMax;
        default: return kMitsubishi112FanMed;
    }
}

/// Convert a stdAc::swingv_t enum into it's native setting.
/// @param[in] position The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRMitsubishi112::convertSwingV(const stdAc::swingv_t position) {
    switch (position) {
        case stdAc::swingv_t::kHighest: return kMitsubishi112SwingVHighest;
        case stdAc::swingv_t::kHigh: return kMitsubishi112SwingVHigh;
        case stdAc::swingv_t::kMiddle: return kMitsubishi112SwingVMiddle;
        case stdAc::swingv_t::kLow: return kMitsubishi112SwingVLow;
        case stdAc::swingv_t::kLowest: return kMitsubishi112SwingVLowest;
        default: return kMitsubishi112SwingVAuto;
    }
}

/// Convert a stdAc::swingh_t enum into it's native setting.
/// @param[in] position The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRMitsubishi112::convertSwingH(const stdAc::swingh_t position) {
    switch (position) {
        case stdAc::swingh_t::kLeftMax: return kMitsubishi112SwingHLeftMax;
        case stdAc::swingh_t::kLeft: return kMitsubishi112SwingHLeft;
        case stdAc::swingh_t::kMiddle: return kMitsubishi112SwingHMiddle;
        case stdAc::swingh_t::kRight: return kMitsubishi112SwingHRight;
        case stdAc::swingh_t::kRightMax: return kMitsubishi112SwingHRightMax;
        case stdAc::swingh_t::kWide: return kMitsubishi112SwingHWide;
        case stdAc::swingh_t::kAuto: return kMitsubishi112SwingHAuto;
        default: return kMitsubishi112SwingHAuto;
    }
}
/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/


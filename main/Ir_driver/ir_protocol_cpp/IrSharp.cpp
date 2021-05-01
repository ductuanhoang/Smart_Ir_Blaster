/*
 * IrSharp.cpp
 *
 *  Created on: Jan 18, 2021
 *      Author: ductu
 */

#include "IrSharp.h"



/***********************************************************************************************************************
 * Pragma directive
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes <System Includes>
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/
using irutils::setBit;
using irutils::setBits;
/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/
// Constants
// period time = 1/38000Hz = 26.316 microseconds.
const uint16_t kSharpTick = 26;
const uint16_t kSharpBitMarkTicks = 10;
const uint16_t kSharpBitMark = kSharpBitMarkTicks * kSharpTick;
const uint16_t kSharpOneSpaceTicks = 70;
const uint16_t kSharpOneSpace = kSharpOneSpaceTicks * kSharpTick;
const uint16_t kSharpZeroSpaceTicks = 30;
const uint16_t kSharpZeroSpace = kSharpZeroSpaceTicks * kSharpTick;
const uint16_t kSharpGapTicks = 1677;
const uint16_t kSharpGap = kSharpGapTicks * kSharpTick;
// Address(5) + Command(8) + Expansion(1) + Check(1)
const uint64_t kSharpToggleMask =
    ((uint64_t)1 << (kSharpBits - kSharpAddressBits)) - 1;
const uint64_t kSharpAddressMask = ((uint64_t)1 << kSharpAddressBits) - 1;
const uint64_t kSharpCommandMask = ((uint64_t)1 << kSharpCommandBits) - 1;
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
Ir_Sharp::Ir_Sharp(uint32_t frequency, uint16_t dutycycle): _ir_sharp_send(frequency, dutycycle) {
	// TODO Auto-generated constructor stub
	stateReset();
}

Ir_Sharp::~Ir_Sharp() {
	// TODO Auto-generated destructor stub
}

/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void Ir_Sharp::sendSharpAC(const uint16_t repeat) {
	send(getRaw(), kSharpAcStateLength, repeat);

}

/***********************************************************************************************************************
 * Static Functions
 ***********************************************************************************************************************/
/// Send a Sharp A/C message.
/// Status: Alpha / Untested.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/638
/// @see https://github.com/ToniA/arduino-heatpumpir/blob/master/SharpHeatpumpIR.cpp
void Ir_Sharp::send(const unsigned char data[], const uint16_t nbytes,
                         const uint16_t repeat) {
  if (nbytes < kSharpAcStateLength)
    return;  // Not enough bytes to send a proper message.

  _ir_sharp_send.sendGeneric(kSharpAcHdrMark, kSharpAcHdrSpace,
              kSharpAcBitMark, kSharpAcOneSpace,
              kSharpAcBitMark, kSharpAcZeroSpace,
              kSharpAcBitMark, kSharpAcGap,
              data, nbytes, 38000, false, repeat, 50);
}
void Ir_Sharp::setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp) {
    setPower(power);
    setMode(convertMode(stdAc::opmode_t(mode)));
    setFan(convertFan(stdAc::fanspeed_t(speed)));
    setTemp(temp);
}
/// Send a (raw) Sharp message
/// @note Status: STABLE / Working fine.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @note his procedure handles the inversion of bits required per protocol.
///   The protocol spec says to send the LSB first, but legacy code & usage
///   has us sending the MSB first. Grrrr. Normal invocation of encodeSharp()
///   handles this for you, assuming you are using the correct/standard values.
///   e.g. sendSharpRaw(encodeSharp(address, command));
void Ir_Sharp::sendSharpRaw(const uint64_t data, const uint16_t nbits,
                          const uint16_t repeat) {
  uint64_t tempdata = data;
  for (uint16_t i = 0; i <= repeat; i++) {
    // Protocol demands that the data be sent twice; once normally,
    // then with all but the address bits inverted.
    // Note: Previously this used to be performed 3 times (normal, inverted,
    //       normal), however all data points to that being incorrect.
    for (uint8_t n = 0; n < 2; n++) {
      _ir_sharp_send.sendGeneric(0, 0,  // No Header
                  kSharpBitMark, kSharpOneSpace, kSharpBitMark, kSharpZeroSpace,
                  kSharpBitMark, kSharpGap, tempdata, nbits, 38, true,
                  0,  // Repeats are handled already.
                  33);
      // Invert the data per protocol. This is always called twice, so it's
      // returned to original upon exiting the inner loop.
      tempdata ^= kSharpToggleMask;
    }
  }
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t Ir_Sharp::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kSharpAcCool;
    case stdAc::opmode_t::kHeat: return kSharpAcHeat;
    case stdAc::opmode_t::kDry:  return kSharpAcDry;
    // No Fan mode.
    default:                     return kSharpAcAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t Ir_Sharp::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:    return kSharpAcFanMin;
    case stdAc::fanspeed_t::kMedium: return kSharpAcFanMed;
    case stdAc::fanspeed_t::kHigh:   return kSharpAcFanHigh;
    case stdAc::fanspeed_t::kMax:    return kSharpAcFanMax;
    default:                         return kSharpAcFanAuto;
  }
}

/// Calculate the checksum for a given state.
/// @param[in] state The array to calc the checksum of.
/// @param[in] length The length/size of the array.
/// @return The calculated 4-bit checksum value.
uint8_t Ir_Sharp::calcChecksum(uint8_t state[], const uint16_t length) {
  uint8_t xorsum = xorBytes(state, length - 1);
  xorsum ^= GETBITS8(state[length - 1], kLowNibble, kNibbleSize);
  xorsum ^= GETBITS8(xorsum, kHighNibble, kNibbleSize);
  return GETBITS8(xorsum, kLowNibble, kNibbleSize);
}

/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @param[in] length The length/size of the array.
/// @return true, if the state has a valid checksum. Otherwise, false.
bool Ir_Sharp::validChecksum(uint8_t state[], const uint16_t length) {
  return GETBITS8(state[length - 1], kHighNibble, kNibbleSize) ==
      Ir_Sharp::calcChecksum(state, length);
}

/// Calculate and set the checksum values for the internal state.
void Ir_Sharp::checksum(void) {
  setBits(&remote[kSharpAcStateLength - 1], kHighNibble, kNibbleSize,
          calcChecksum(remote));
}

/// Reset the state of the remote to a known good state/sequence.
void Ir_Sharp::stateReset(void) {
  static const uint8_t reset[kSharpAcStateLength] = {
      0xAA, 0x5A, 0xCF, 0x10, 0x00, 0x01, 0x00, 0x00, 0x08, 0x80, 0x00, 0xE0,
      0x01};
  memcpy(remote, reset, kSharpAcStateLength);
  _temp = getTemp();
  _mode = getMode();
  _fan = getFan();
  _model = getModel(true);
}

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t *Ir_Sharp::getRaw(void) {
  checksum();  // Ensure correct settings before sending.
  return remote;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] new_code A valid code for this protocol.
/// @param[in] length The length/size of the new_code array.
void Ir_Sharp::setRaw(const uint8_t new_code[], const uint16_t length) {

  memcpy(remote, new_code, length < kSharpAcStateLength ? length : kSharpAcStateLength);
  _model = getModel(true);
}

/// Set the model of the A/C to emulate.
/// @param[in] model The enum of the appropriate model.
void Ir_Sharp::setModel(const sharp_ac_remote_model_t model) {
  switch (model) {
    case sharp_ac_remote_model_t::A705:
      _model = model;
      break;
    default:
      _model = sharp_ac_remote_model_t::A907;
  }
  setBit(&remote[kSharpAcByteIon], kSharpAcModelBit,
         _model == sharp_ac_remote_model_t::A705);
  // Redo the operating mode as some models don't support all modes.
  setMode(getMode());
}

/// Get/Detect the model of the A/C.
/// @param[in] raw Try to determine the model from the raw code only.
/// @return The enum of the compatible model.
sharp_ac_remote_model_t Ir_Sharp::getModel(const bool raw) {
  if (raw)
    return (GETBIT8(remote[kSharpAcByteIon],
                    kSharpAcModelBit) &&
            GETBIT8(remote[kSharpAcByteTemp],
                    kSharpAcModelBit)) ? sharp_ac_remote_model_t::A705
                                       : sharp_ac_remote_model_t::A907;
  return _model;
}

/// Set the value of the Power Special setting without any checks.
/// @param[in] value The value to set Power Special to.
void Ir_Sharp::setPowerSpecial(const uint8_t value) {
  setBits(&remote[kSharpAcBytePowerSpecial], kSharpAcPowerSetSpecialOffset,
          kSharpAcPowerSpecialSize, value);
}

/// Get the value of the Power Special setting.
/// @return The setting's value.
uint8_t Ir_Sharp::getPowerSpecial(void) {
  return GETBITS8(remote[kSharpAcBytePowerSpecial],
                  kSharpAcPowerSetSpecialOffset, kSharpAcPowerSpecialSize);
}

/// Clear the "special"/non-normal bits in the power section.
/// e.g. for normal/common command modes.
void Ir_Sharp::clearPowerSpecial(void) {
  setPowerSpecial(getPowerSpecial() & kSharpAcPowerOn);
}

/// Is one of the special power states in use?
/// @return true, it is. false, it isn't.
bool Ir_Sharp::isPowerSpecial(void) {
  switch (getPowerSpecial()) {
    case kSharpAcPowerSetSpecialOff:
    case kSharpAcPowerSetSpecialOn:
    case kSharpAcPowerTimerSetting: return true;
    default: return false;
  }
}

/// Set the requested power state of the A/C to on.
void Ir_Sharp::on(void) { setPower(true); }

/// Set the requested power state of the A/C to off.
void Ir_Sharp::off(void) { setPower(false); }

/// Change the power setting, including the previous power state.
/// @param[in] on true, the setting is on. false, the setting is off.
/// @param[in] prev_on true, the setting is on. false, the setting is off.
void Ir_Sharp::setPower(const bool on, const bool prev_on) {
  setPowerSpecial(on ? (prev_on ? kSharpAcPowerOn : kSharpAcPowerOnFromOff)
                     : kSharpAcPowerOff);
  // Power operations are incompatible with clean mode.
  if (getClean()) setClean(false);
  setSpecial(kSharpAcSpecialPower);
}

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool Ir_Sharp::getPower(void) {
  switch (getPowerSpecial()) {
    case kSharpAcPowerUnknown:
    case kSharpAcPowerOff: return false;
    default: return true;  // Everything else is "probably" on.
  }
}

/// Set the value of the Special (button/command?) setting.
/// @param[in] mode The value to set Special to.
void Ir_Sharp::setSpecial(const uint8_t mode) {
  switch (mode) {
    case kSharpAcSpecialPower:
    case kSharpAcSpecialTurbo:
    case kSharpAcSpecialTempEcono:
    case kSharpAcSpecialFan:
    case kSharpAcSpecialSwing:
    case kSharpAcSpecialTimer:
    case kSharpAcSpecialTimerHalfHour:
      remote[kSharpAcByteSpecial] = mode;
      break;
    default:
      setSpecial(kSharpAcSpecialPower);
  }
}

/// Get the value of the Special (button/command?) setting.
/// @return The setting's value.
uint8_t Ir_Sharp::getSpecial(void) { return remote[kSharpAcByteSpecial]; }

/// Set the temperature.
/// @param[in] temp The temperature in degrees celsius.
/// @param[in] save Do we save this setting as a user set one?
void Ir_Sharp::setTemp(const uint8_t temp, const bool save) {
  switch (getMode()) {
    // Auto & Dry don't allow temp changes and have a special temp.
    case kSharpAcAuto:
    case kSharpAcDry:
      remote[kSharpAcByteTemp] = 0;
      return;
    default:
      switch (getModel()) {
        case sharp_ac_remote_model_t::A705:
          remote[kSharpAcByteTemp] = 0xD0;
          break;
        default:
          remote[kSharpAcByteTemp] = 0xC0;
      }
  }
  uint8_t degrees = std::max(temp, kSharpAcMinTemp);
  degrees = std::min(degrees, kSharpAcMaxTemp);
  if (save) _temp = degrees;
  setBits(&remote[kSharpAcByteTemp], kLowNibble, kNibbleSize,
          degrees - kSharpAcMinTemp);
  setSpecial(kSharpAcSpecialTempEcono);
  clearPowerSpecial();
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t Ir_Sharp::getTemp(void) {
  return GETBITS8(remote[kSharpAcByteTemp], kLowNibble, kNibbleSize) +
      kSharpAcMinTemp;
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t Ir_Sharp::getMode(void) {
  return GETBITS8(remote[kSharpAcByteMode], kLowNibble, kSharpAcModeSize);
}

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
/// @param[in] save Do we save this setting as a user set one?
void Ir_Sharp::setMode(const uint8_t mode, const bool save) {
  if (mode == kSharpAcHeat && getModel() == sharp_ac_remote_model_t::A705) {
    // A705 has no heat mode, use Fan mode instead.
    setMode(kSharpAcFan, save);
    return;
  }

  switch (mode) {
    case kSharpAcAuto:  // Also kSharpAcFan
    case kSharpAcDry:
      // When Dry or Auto, Fan always 2(Auto)
      setFan(kSharpAcFanAuto, false);
      // FALLTHRU
      	  break;
    case kSharpAcCool:
    	break;
    case kSharpAcHeat:
      setBits(&remote[kSharpAcByteMode], kLowNibble, kSharpAcModeSize, mode);
      break;
    default:
      setMode(kSharpAcAuto, save);
      return;
  }
  // Dry/Auto have no temp setting. This step will enforce it.
  setTemp(_temp, false);
  // Save the mode in case we need to revert to it. eg. Clean
  if (save) _mode = mode;

  setSpecial(kSharpAcSpecialPower);
  clearPowerSpecial();
}

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
/// @param[in] save Do we save this setting as a user set one?
void Ir_Sharp::setFan(const uint8_t speed, const bool save) {
  switch (speed) {
    case kSharpAcFanAuto:
    case kSharpAcFanMin:
    case kSharpAcFanMed:
    case kSharpAcFanHigh:
    case kSharpAcFanMax:
      setBits(&remote[kSharpAcByteFan], kSharpAcFanOffset, kSharpAcFanSize,
              speed);
      break;
    default:
      setFan(kSharpAcFanAuto);
      return;
  }
  if (save) _fan = speed;
  setSpecial(kSharpAcSpecialFan);
  clearPowerSpecial();
}

/// Get the current fan speed setting.
/// @return The current fan speed/mode.
uint8_t Ir_Sharp::getFan(void) {
  return GETBITS8(remote[kSharpAcByteFan], kSharpAcFanOffset, kSharpAcFanSize);
}

/// Get the Turbo setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool Ir_Sharp::getTurbo(void) {
  return (getPowerSpecial() == kSharpAcPowerSetSpecialOn) &&
         (getSpecial() == kSharpAcSpecialTurbo);
}

/// Set the Turbo setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
/// @note If you use this method, you will need to send it before making
///   other changes to the settings, as they may overwrite some of the bits
///   used by this setting.
void Ir_Sharp::setTurbo(const bool on) {
  if (on) setFan(kSharpAcFanMax);
  setPowerSpecial(on ? kSharpAcPowerSetSpecialOn : kSharpAcPowerSetSpecialOff);
  setSpecial(kSharpAcSpecialTurbo);
}

/// Get the (vertical) Swing Toggle setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool Ir_Sharp::getSwingToggle(void) {
  return GETBITS8(remote[kSharpAcByteSwing], kSharpAcSwingOffset,
                  kSharpAcSwingSize) == kSharpAcSwingToggle;
}

/// Set the (vertical) Swing Toggle setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void Ir_Sharp::setSwingToggle(const bool on) {
  setBits(&remote[kSharpAcByteSwing], kSharpAcSwingOffset, kSharpAcSwingSize,
          on ? kSharpAcSwingToggle : kSharpAcSwingNoToggle);
  if (on) setSpecial(kSharpAcSpecialSwing);
}

/// Get the Ion (Filter) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool Ir_Sharp::getIon(void) {
  return GETBIT8(remote[kSharpAcByteIon], kSharpAcBitIonOffset);
}

/// Set the Ion (Filter) setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void Ir_Sharp::setIon(const bool on) {
  setBit(&remote[kSharpAcByteIon], kSharpAcBitIonOffset, on);
  clearPowerSpecial();
  if (on) setSpecial(kSharpAcSpecialSwing);
}

/// Get the Economical mode toggle setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
/// @note Shares the same location as the Light setting on A705.
bool Ir_Sharp::_getEconoToggle(void) {
  return (getPowerSpecial() == kSharpAcPowerSetSpecialOn) &&
         (getSpecial() == kSharpAcSpecialTempEcono);
}

/// Set the Economical mode toggle setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
/// @warning Probably incompatible with `setTurbo()`
/// @note Shares the same location as the Light setting on A705.
void Ir_Sharp::_setEconoToggle(const bool on) {
  if (on) setSpecial(kSharpAcSpecialTempEcono);
  setPowerSpecial(on ? kSharpAcPowerSetSpecialOn : kSharpAcPowerSetSpecialOff);
}

/// Set the Economical mode toggle setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
/// @warning Probably incompatible with `setTurbo()`
/// @note Not available on the A705 model.
void Ir_Sharp::setEconoToggle(const bool on) {
  if (_model != sharp_ac_remote_model_t::A705) _setEconoToggle(on);
}

/// Get the Economical mode toggle setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
/// @note Not available on the A705 model.
bool Ir_Sharp::getEconoToggle(void) {
  return _model != sharp_ac_remote_model_t::A705 && _getEconoToggle();
}

/// Set the Light mode toggle setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
/// @warning Probably incompatible with `setTurbo()`
/// @note Not available on the A907 model.
void Ir_Sharp::setLightToggle(const bool on) {
  if (_model != sharp_ac_remote_model_t::A705) _setEconoToggle(on);
}

/// Get the Light toggle setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
/// @note Not available on the A907 model.
bool Ir_Sharp::getLightToggle(void) {
  return _model != sharp_ac_remote_model_t::A907 && _getEconoToggle();
}

/// Get how long the timer is set for, in minutes.
/// @return The time in nr of minutes.
uint16_t Ir_Sharp::getTimerTime(void) {
  return GETBITS8(remote[kSharpAcByteTimer], kSharpAcTimerHoursOffset,
                  kSharpAcTimerHoursSize) * kSharpAcTimerIncrement * 2 +
      ((getSpecial() == kSharpAcSpecialTimerHalfHour) ? kSharpAcTimerIncrement
                                                      : 0);
}

/// Is the Timer enabled?
/// @return true, the setting is on. false, the setting is off.
bool Ir_Sharp::getTimerEnabled(void) {
  return GETBIT8(remote[kSharpAcByteTimer], kSharpAcBitTimerEnabled);
}

/// Get the current timer type.
/// @return true, It's an "On" timer. false, It's an "Off" timer.
bool Ir_Sharp::getTimerType(void) {
  return GETBIT8(remote[kSharpAcByteTimer], kSharpAcBitTimerType);
}

/// Set or cancel the timer function.
/// @param[in] enable Is the timer to be enabled (true) or canceled(false)?
/// @param[in] timer_type An On (true) or an Off (false). Ignored if canceled.
/// @param[in] mins Nr. of minutes the timer is to be set to.
/// @note Rounds down to 30 min increments. (max: 720 mins (12h), 0 is Off)
void Ir_Sharp::setTimer(bool enable, bool timer_type, uint16_t mins) {
  uint8_t half_hours = std::min(mins / kSharpAcTimerIncrement,
                                kSharpAcTimerHoursMax * 2);
  if (half_hours == 0) enable = false;
  if (!enable) {
    half_hours = 0;
    timer_type = kSharpAcOffTimerType;
  }
  setBit(&remote[kSharpAcByteTimer], kSharpAcBitTimerEnabled, enable);
  setBit(&remote[kSharpAcByteTimer], kSharpAcBitTimerType, timer_type);
  setBits(&remote[kSharpAcByteTimer], kSharpAcTimerHoursOffset,
          kSharpAcTimerHoursSize, half_hours / 2);
  // Handle non-round hours.
  setSpecial((half_hours % 2) ? kSharpAcSpecialTimerHalfHour
                              : kSharpAcSpecialTimer);
  setPowerSpecial(kSharpAcPowerTimerSetting);
}

/// Get the Clean setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool Ir_Sharp::getClean(void) {
  return GETBIT8(remote[kSharpAcByteClean], kSharpAcBitCleanOffset);
}

/// Set the Economical mode toggle setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
/// @note Officially A/C unit needs to be "Off" before clean mode can be entered
void Ir_Sharp::setClean(const bool on) {
  // Clean mode appears to be just default dry mode, with an extra bit set.
  if (on) {
    setMode(kSharpAcDry, false);
    setPower(true, false);
  } else {
    // Restore the previous operation mode & fan speed.
    setMode(_mode, false);
    setFan(_fan, false);
  }
  setBit(&remote[kSharpAcByteClean], kSharpAcBitCleanOffset, on);
  clearPowerSpecial();
}
/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/


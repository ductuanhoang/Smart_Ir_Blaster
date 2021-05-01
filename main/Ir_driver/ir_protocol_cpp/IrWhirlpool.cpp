/*
 * IrWhirlpool.cpp
 *
 *  Created on: Jan 17, 2021
 *      Author: ductu
 */
/***********************************************************************************************************************
 * Pragma directive
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes <System Includes>
 ***********************************************************************************************************************/
#include "IrWhirlpool.h"
#include "IRutils.h"
/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/
// Constants
const uint16_t kWhirlpoolAcHdrMark = 8950;
const uint16_t kWhirlpoolAcHdrSpace = 4484;
const uint16_t kWhirlpoolAcBitMark = 597;
const uint16_t kWhirlpoolAcOneSpace = 1649;
const uint16_t kWhirlpoolAcZeroSpace = 533;
const uint16_t kWhirlpoolAcGap = 7920;
const uint32_t kWhirlpoolAcMinGap = kDefaultMessageGap;  // Just a guess.
const uint8_t kWhirlpoolAcSections = 3;

using irutils::setBit;
using irutils::setBits;

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
Ir_Whirlpool::Ir_Whirlpool(uint32_t frequency, uint16_t dutycycle): _ir_whirlpool_send(frequency, dutycycle) {
	// TODO Auto-generated constructor stub
	stateReset();
}

Ir_Whirlpool::~Ir_Whirlpool() {
	// TODO Auto-generated destructor stub
}

void Ir_Whirlpool::sendWhirlpool(const uint16_t repeat, const bool calcchecksum) {
    if (calcchecksum) checksum();
    send(remote_state, kWhirlpoolAcStateLength, repeat);
}
void Ir_Whirlpool::setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp) {
    setPowerToggle(power);
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
uint8_t Ir_Whirlpool::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kAuto: return kWhirlpoolAcAuto;
    case stdAc::opmode_t::kHeat: return kWhirlpoolAcHeat;
    case stdAc::opmode_t::kDry:  return kWhirlpoolAcDry;
    case stdAc::opmode_t::kFan:  return kWhirlpoolAcFan;
    // Default to Cool as some Whirlpool models don't have an Auto mode.
    // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1283
    default:                     return kWhirlpoolAcCool;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t Ir_Whirlpool::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:    return kWhirlpoolAcFanLow;
    case stdAc::fanspeed_t::kMedium: return kWhirlpoolAcFanMedium;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:    return kWhirlpoolAcFanHigh;
    default:                         return kWhirlpoolAcFanAuto;
  }
}

void Ir_Whirlpool::send(const unsigned char *data, const uint16_t nbytes, const uint16_t repeat) {
    if (nbytes < kWhirlpoolAcStateLength)
        return;  // Not enough bytes to send a proper message.
    for (uint16_t r = 0; r <= repeat; r++) {
        // Section 1
    	_ir_whirlpool_send.sendGeneric(kWhirlpoolAcHdrMark, kWhirlpoolAcHdrSpace, kWhirlpoolAcBitMark,
                    kWhirlpoolAcOneSpace, kWhirlpoolAcBitMark,
                    kWhirlpoolAcZeroSpace, kWhirlpoolAcBitMark, kWhirlpoolAcGap,
                    data, 6,  // 6 bytes == 48 bits
                    38000,    // Complete guess of the modulation frequency.
                    false, 0, 50);
        // Section 2
    	_ir_whirlpool_send.sendGeneric(0, 0, kWhirlpoolAcBitMark, kWhirlpoolAcOneSpace,
                    kWhirlpoolAcBitMark, kWhirlpoolAcZeroSpace, kWhirlpoolAcBitMark,
                    kWhirlpoolAcGap, data + 6, 8,  // 8 bytes == 64 bits
                    38000,  // Complete guess of the modulation frequency.
                    false, 0, 50);
        // Section 3
    	_ir_whirlpool_send.sendGeneric(0, 0, kWhirlpoolAcBitMark, kWhirlpoolAcOneSpace,
                    kWhirlpoolAcBitMark, kWhirlpoolAcZeroSpace, kWhirlpoolAcBitMark,
                    kWhirlpoolAcMinGap, data + 14, 7,  // 7 bytes == 56 bits
                    38000,  // Complete guess of the modulation frequency.
                    false, 0, 50);
    }
}

void Ir_Whirlpool::setPowerToggle(const bool on) {
    setBit(&remote_state[kWhirlpoolAcPowerTogglePos],
           kWhirlpoolAcPowerToggleOffset, on);
    setSuper(false);  // Changing power cancels Super/Jet mode.
    setCommand(kWhirlpoolAcCommandPower);
}

bool Ir_Whirlpool::getPowerToggle() {
    return GETBIT8(remote_state[kWhirlpoolAcPowerTogglePos],
                   kWhirlpoolAcPowerToggleOffset);
}
/// Set the Sleep setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void Ir_Whirlpool::setSleep(const bool on) {
    setBit(&remote_state[kWhirlpoolAcSleepPos],
           kWhirlpoolAcSleepOffset, on);
    if (on) setFan(kWhirlpoolAcFanLow);
    setCommand(kWhirlpoolAcCommandSleep);
}
/// Get the Sleep setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool Ir_Whirlpool::getSleep() {
    return GETBIT8(remote_state[kWhirlpoolAcSleepPos], kWhirlpoolAcSleepOffset);
}
/// Set the Super (Turbo/Jet) setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void Ir_Whirlpool::setSuper(const bool on) {
    if (on) {
        setFan(kWhirlpoolAcFanHigh);
        switch (getMode()) {
            case kWhirlpoolAcHeat:
                setTemp(kWhirlpoolAcMaxTemp + getTempOffset());
                break;
            case kWhirlpoolAcCool:
            default:
                setTemp(kWhirlpoolAcMinTemp + getTempOffset());
                setMode(kWhirlpoolAcCool);
                break;
        }
        remote_state[kWhirlpoolAcSuperPos] |= kWhirlpoolAcSuperMask;
    } else {
        remote_state[kWhirlpoolAcSuperPos] &= ~kWhirlpoolAcSuperMask;
    }
    setCommand(kWhirlpoolAcCommandSuper);
}
/// Get the Super (Turbo/Jet) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool Ir_Whirlpool::getSuper() {
    return remote_state[kWhirlpoolAcSuperPos] & kWhirlpoolAcSuperMask;
}
/// Set the temperature.
/// @param[in] temp The temperature in degrees celsius.
void Ir_Whirlpool::setTemp(const uint8_t temp) {
    _setTemp(temp);
    setSuper(false);  // Changing temp cancels Super/Jet mode.
    setCommand(kWhirlpoolAcCommandTemp);
}
/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t Ir_Whirlpool::getTemp() {
    return GETBITS8(remote_state[kWhirlpoolAcTempPos], kHighNibble, kNibbleSize) +
           kWhirlpoolAcMinTemp + getTempOffset();
}

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
void Ir_Whirlpool::setFan(const uint8_t speed) {
    switch (speed) {
        case kWhirlpoolAcFanAuto:
        case kWhirlpoolAcFanLow:
        case kWhirlpoolAcFanMedium:
        case kWhirlpoolAcFanHigh:
            setBits(&remote_state[kWhirlpoolAcFanPos], kWhirlpoolAcFanOffset,
                    kWhirlpoolAcFanSize, speed);
            setSuper(false);  // Changing fan speed cancels Super/Jet mode.
            setCommand(kWhirlpoolAcCommandFanSpeed);
            break;
    }
}
/// Get the current fan speed setting.
/// @return The current fan speed/mode.
uint8_t Ir_Whirlpool::getFan() {
    return GETBITS8(remote_state[kWhirlpoolAcFanPos], kWhirlpoolAcFanOffset,
                    kWhirlpoolAcFanSize);
}
/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void Ir_Whirlpool::setMode(const uint8_t mode) {
    setSuper(false);  // Changing mode cancels Super/Jet mode.
    _setMode(mode);
}
/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t Ir_Whirlpool::getMode() {
    return GETBITS8(remote_state[kWhirlpoolAcModePos], kWhirlpoolAcModeOffset,
                    kModeBitsSize);
}

void Ir_Whirlpool::setSwing(const bool on) {
    setBit(&remote_state[kWhirlpoolAcFanPos], kWhirlpoolAcSwing1Offset, on);
    setBit(&remote_state[kWhirlpoolAcOffTimerPos], kWhirlpoolAcSwing2Offset, on);
    setCommand(kWhirlpoolAcCommandSwing);
}

bool Ir_Whirlpool::getSwing() {
    return GETBIT8(remote_state[kWhirlpoolAcFanPos], kWhirlpoolAcSwing1Offset) &&
           GETBIT8(remote_state[kWhirlpoolAcOffTimerPos],
                   kWhirlpoolAcSwing2Offset);
}

void Ir_Whirlpool::setLight(const bool on) {
    // Cleared when on.
    setBit(&remote_state[kWhirlpoolAcClockPos], kWhirlpoolAcLightOffset, !on);
}

bool Ir_Whirlpool::getLight() {
    return !GETBIT8(remote_state[kWhirlpoolAcClockPos], kWhirlpoolAcLightOffset);
}

uint16_t Ir_Whirlpool::getClock() {
    return getTime(kWhirlpoolAcClockPos);
}

void Ir_Whirlpool::setClock(const uint16_t minspastmidnight) {
    setTime(kWhirlpoolAcClockPos, minspastmidnight);
}

uint16_t Ir_Whirlpool::getOnTimer() {
    return getTime(kWhirlpoolAcOnTimerPos);
}

void Ir_Whirlpool::setOnTimer(const uint16_t minspastmidnight) {
    setTime(kWhirlpoolAcOnTimerPos, minspastmidnight);
}

void Ir_Whirlpool::enableOnTimer(const bool on) {
    enableTimer(kWhirlpoolAcOnTimerPos, on);
    setCommand(kWhirlpoolAcCommandOnTimer);
}

bool Ir_Whirlpool::isOnTimerEnabled() {
    return isTimerEnabled(kWhirlpoolAcOnTimerPos);
}

uint16_t Ir_Whirlpool::getOffTimer() {
    return getTime(kWhirlpoolAcOffTimerPos);
}

void Ir_Whirlpool::setOffTimer(const uint16_t minspastmidnight) {
    setTime(kWhirlpoolAcOffTimerPos, minspastmidnight);
}

void Ir_Whirlpool::enableOffTimer(const bool on) {
    enableTimer(kWhirlpoolAcOffTimerPos, on);
    setCommand(kWhirlpoolAcCommandOffTimer);
}

bool Ir_Whirlpool::isOffTimerEnabled() {
    return isTimerEnabled(kWhirlpoolAcOffTimerPos);
}
void Ir_Whirlpool::setCommand(const uint8_t code) {
    remote_state[kWhirlpoolAcCommandPos] = code;
}

uint8_t Ir_Whirlpool::getCommand() {
    return remote_state[kWhirlpoolAcCommandPos];
}

whirlpool_ac_remote_model_t Ir_Whirlpool::getModel() {
    if (GETBIT8(remote_state[kWhirlpoolAcAltTempPos], kWhirlpoolAcAltTempOffset))
        return DG11J191;
    else
        return DG11J13A;
}

void Ir_Whirlpool::setModel(const whirlpool_ac_remote_model_t model) {
    switch (model) {
        case DG11J191:
            setBit(&remote_state[kWhirlpoolAcAltTempPos], kWhirlpoolAcAltTempOffset);
            break;
        case DG11J13A:
            // FALL THRU
        default:
            setBit(&remote_state[kWhirlpoolAcAltTempPos], kWhirlpoolAcAltTempOffset,
                   false);
    }
    _setTemp(_desiredtemp);  // Different models have different temp values.

}

void Ir_Whirlpool::setRaw(const uint8_t new_code[], const uint16_t length) {
	  memcpy(remote_state, new_code, length < kWhirlpoolAcStateLength ? length :  kWhirlpoolAcStateLength);
}

uint8_t * Ir_Whirlpool::getRaw(const bool calcchecksum) {
	  if (calcchecksum) checksum();
	  return remote_state;
}
/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @param[in] length The length/size of the array.
/// @return true, if the state has a valid checksum. Otherwise, false.
bool Ir_Whirlpool::validChecksum(const uint8_t *state, const uint16_t length) {
    if (length > kWhirlpoolAcChecksumByte1 &&
        state[kWhirlpoolAcChecksumByte1] !=
        xorBytes(state + 2, kWhirlpoolAcChecksumByte1 - 1 - 2)) {
        APP_LOGE("DEBUG: First Whirlpool AC checksum failed.");
        return false;
    }
    if (length > kWhirlpoolAcChecksumByte2 &&
        state[kWhirlpoolAcChecksumByte2] !=
        xorBytes(state + kWhirlpoolAcChecksumByte1 + 1,
                 kWhirlpoolAcChecksumByte2 - kWhirlpoolAcChecksumByte1 - 1)) {
    	APP_LOGE("DEBUG: Second Whirlpool AC checksum failed.");
        return false;
    }
    // State is too short to have a checksum or everything checked out.
    return true;
}

void Ir_Whirlpool::checksum(const uint16_t length) {
    if (length >= kWhirlpoolAcChecksumByte1)
        remote_state[kWhirlpoolAcChecksumByte1] =
                xorBytes(remote_state + 2, kWhirlpoolAcChecksumByte1 - 1 - 2);
    if (length >= kWhirlpoolAcChecksumByte2)
        remote_state[kWhirlpoolAcChecksumByte2] =
                xorBytes(remote_state + kWhirlpoolAcChecksumByte1 + 1,
                         kWhirlpoolAcChecksumByte2 - kWhirlpoolAcChecksumByte1 - 1);
}

uint16_t Ir_Whirlpool::getTime(const uint16_t pos) {
    return GETBITS8(remote_state[pos], kWhirlpoolAcHourOffset,
                    kWhirlpoolAcHourSize) * 60 +
           GETBITS8(remote_state[pos + 1], kWhirlpoolAcMinuteOffset,
                    kWhirlpoolAcMinuteSize);
}

void Ir_Whirlpool::setTime(const uint16_t pos, const uint16_t minspastmidnight) {
    // Hours
    setBits(&remote_state[pos], kWhirlpoolAcHourOffset, kWhirlpoolAcHourSize,
            (minspastmidnight / 60) % 24);
    // Minutes
    setBits(&remote_state[pos + 1], kWhirlpoolAcMinuteOffset,
            kWhirlpoolAcMinuteSize, minspastmidnight % 60);
}
bool Ir_Whirlpool::isTimerEnabled(const uint16_t pos) {
    return GETBIT8(remote_state[pos - 1], kWhirlpoolAcTimerEnableOffset);
}

void Ir_Whirlpool::enableTimer(const uint16_t pos, const bool on) {
    setBit(&remote_state[pos - 1], kWhirlpoolAcTimerEnableOffset, on);
}

void Ir_Whirlpool::_setTemp(const uint8_t temp, const bool remember) {
    if (remember) _desiredtemp = temp;
    int8_t offset = getTempOffset();  // Cache the min temp for the model.
    uint8_t newtemp = std::max((uint8_t)(kWhirlpoolAcMinTemp + offset), temp);
    newtemp = std::min((uint8_t)(kWhirlpoolAcMaxTemp + offset), newtemp);
    setBits(&remote_state[kWhirlpoolAcTempPos], kHighNibble, kNibbleSize,
            newtemp - (kWhirlpoolAcMinTemp + offset));

}

void Ir_Whirlpool::_setMode(const uint8_t mode) {
    switch (mode) {
        case kWhirlpoolAcAuto:
            setFan(kWhirlpoolAcFanAuto);
            _setTemp(kWhirlpoolAcAutoTemp, false);
            setSleep(false);  // Cancel sleep mode when in auto/6thsense mode.
            // FALL THRU
            break;
        case kWhirlpoolAcHeat:
        case kWhirlpoolAcCool:
        case kWhirlpoolAcDry:
        case kWhirlpoolAcFan:
            setBits(&remote_state[kWhirlpoolAcModePos], kWhirlpoolAcModeOffset,
                    kModeBitsSize, mode);
            setCommand(kWhirlpoolAcCommandMode);
            break;
        default:
            return;
    }
    if (mode == kWhirlpoolAcAuto) setCommand(kWhirlpoolAcCommand6thSense);

}

int8_t Ir_Whirlpool::getTempOffset() {
    switch (getModel()) {
        case whirlpool_ac_remote_model_t::DG11J191: return -2;
        default:                                    return 0;
    }
}

void Ir_Whirlpool::stateReset() {
    for (uint8_t i = 2; i < kWhirlpoolAcStateLength; i++) remote_state[i] = 0x0;
    remote_state[0] = 0x83;
    remote_state[1] = 0x06;
    remote_state[6] = 0x80;
    _setTemp(kWhirlpoolAcAutoTemp);  // Default to a sane value.
}
/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/


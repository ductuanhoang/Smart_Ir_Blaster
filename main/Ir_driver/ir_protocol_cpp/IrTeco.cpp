/*
 * IrTeco.cpp
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
#include "IrTeco.h"
#include "IRutils.h"
/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/
// Constants
// using SPACE modulation.
const uint16_t kTecoHdrMark = 9000;
const uint16_t kTecoHdrSpace = 4440;
const uint16_t kTecoBitMark = 620;
const uint16_t kTecoOneSpace = 1650;
const uint16_t kTecoZeroSpace = 580;
const uint32_t kTecoGap = kDefaultMessageGap;  // Made-up value. Just a guess.

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/
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
Ir_Teco::Ir_Teco(uint32_t frequency, uint16_t dutycycle): _ir_remote(frequency, dutycycle){
    // TODO Auto-generated constructor stub
    stateReset();
}

Ir_Teco::~Ir_Teco() {
    // TODO Auto-generated destructor stub
}

void Ir_Teco::setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp) {
    setPower(power);
    setMode(convertMode(stdAc::opmode_t(mode)));
    setFan(convertFan(stdAc::fanspeed_t(speed)));
    setTemp(temp);
}
/***********************************************************************************************************************
 * Static Functions
 ***********************************************************************************************************************/
// Copyright 2019 Fabien Valthier

/// Send a Teco A/C message.
/// Status: Beta / Probably working.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void Ir_Teco::send(const uint64_t data, const uint16_t nbits,
                      const uint16_t repeat) {
	_ir_remote.sendGeneric(kTecoHdrMark, kTecoHdrSpace, kTecoBitMark, kTecoOneSpace,
              kTecoBitMark, kTecoZeroSpace, kTecoBitMark, kTecoGap,
              data, nbits, 38000, false, repeat, 50);
}



/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t Ir_Teco::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kTecoCool;
    case stdAc::opmode_t::kHeat: return kTecoHeat;
    case stdAc::opmode_t::kDry:  return kTecoDry;
    case stdAc::opmode_t::kFan:  return kTecoFan;
    default:                     return kTecoAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t Ir_Teco::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:    return kTecoFanLow;
    case stdAc::fanspeed_t::kMedium: return kTecoFanMed;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:    return kTecoFanHigh;
    default:                         return kTecoFanAuto;
  }
}

/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void Ir_Teco::sendTecoAc(const uint16_t repeat) {
	send(remote_state, kTecoBits, repeat);
}


/// Reset the internal state of the emulation.
/// @note Mode:auto, Power:Off, fan:auto, temp:16, swing:off, sleep:off
void Ir_Teco::stateReset(void) {
    remote_state = kTecoReset;
}

/// Get a copy of the internal state/code for this protocol.
/// @return A code for this protocol based on the current internal state.
uint64_t Ir_Teco::getRaw(void) { return remote_state; }

/// Set the internal state from a valid code for this protocol.
/// @param[in] new_code A valid code for this protocol.
void Ir_Teco::setRaw(const uint64_t new_code) { remote_state = new_code; }

/// Set the requested power state of the A/C to on.
void Ir_Teco::on(void) { setPower(true); }

/// Set the requested power state of the A/C to off.
void Ir_Teco::off(void) { setPower(false); }

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void Ir_Teco::setPower(const bool on) {
    setBit(&remote_state, kTecoPowerOffset, on);
}

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool Ir_Teco::getPower(void) {
    return GETBIT64(remote_state, kTecoPowerOffset);
}

/// Set the temperature.
/// @param[in] temp The temperature in degrees celsius.
void Ir_Teco::setTemp(const uint8_t temp) {
    uint8_t newtemp = temp;
    newtemp = std::min(newtemp, kTecoMaxTemp);
    newtemp = std::max(newtemp, kTecoMinTemp);
    setBits(&remote_state, kTecoTempOffset, kTecoTempSize,
            newtemp - kTecoMinTemp);
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t Ir_Teco::getTemp(void) {
    return GETBITS64(remote_state, kTecoTempOffset, kTecoTempSize) + kTecoMinTemp;
}

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
void Ir_Teco::setFan(const uint8_t speed) {
    uint8_t newspeed = speed;
    switch (speed) {
        case kTecoFanAuto:
        case kTecoFanHigh:
        case kTecoFanMed:
        case kTecoFanLow: break;
        default: newspeed = kTecoFanAuto;
    }
    setBits(&remote_state, kTecoFanOffset, kTecoFanSize, newspeed);
}

/// Get the current fan speed setting.
/// @return The current fan speed/mode.
uint8_t Ir_Teco::getFan(void) {
    return GETBITS64(remote_state, kTecoFanOffset, kTecoFanSize);
}

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void Ir_Teco::setMode(const uint8_t mode) {
    uint8_t newmode = mode;
    switch (mode) {
        case kTecoAuto:
        case kTecoCool:
        case kTecoDry:
        case kTecoFan:
        case kTecoHeat: break;
        default: newmode = kTecoAuto;
    }
    setBits(&remote_state, kTecoModeOffset, kModeBitsSize, newmode);
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t Ir_Teco::getMode(void) {
    return GETBITS64(remote_state, kTecoModeOffset, kModeBitsSize);
}

/// Set the (vertical) swing setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void Ir_Teco::setSwing(const bool on) {
    setBit(&remote_state, kTecoSwingOffset, on);
}

/// Get the (vertical) swing setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool Ir_Teco::getSwing(void) {
    return GETBIT64(remote_state, kTecoSwingOffset);
}

/// Set the Sleep setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void Ir_Teco::setSleep(const bool on) {
    setBit(&remote_state, kTecoSleepOffset, on);
}

/// Get the Sleep setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool Ir_Teco::getSleep(void) {
    return GETBIT64(remote_state, kTecoSleepOffset);
}

/// Set the Light (LED/Display) setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void Ir_Teco::setLight(const bool on) {
    setBit(&remote_state, kTecoLightOffset, on);
}

/// Get the Light (LED/Display) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool Ir_Teco::getLight(void) {
    return GETBIT64(remote_state,  kTecoLightOffset);
}

/// Set the Humid setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void Ir_Teco::setHumid(const bool on) {
    setBit(&remote_state, kTecoHumidOffset, on);
}

/// Get the Humid setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool Ir_Teco::getHumid(void) {
    return GETBIT64(remote_state, kTecoHumidOffset);
}

/// Set the Save setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void Ir_Teco::setSave(const bool on) {
    setBit(&remote_state, kTecoSaveOffset, on);
}

/// Get the Save setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool Ir_Teco::getSave(void) {
    return GETBIT64(remote_state, kTecoSaveOffset);
}

/// Is the timer function enabled?
/// @return true, the setting is on. false, the setting is off.
bool Ir_Teco::getTimerEnabled(void) {
    return GETBIT64(remote_state, kTecoTimerOnOffset);
}

/// Get the timer time for when the A/C unit will switch power state.
/// @return The number of minutes left on the timer. `0` means off.
uint16_t Ir_Teco::getTimer(void) {
    uint16_t mins = 0;
    if (getTimerEnabled()) {
        mins = GETBITS64(remote_state, kTecoTimerTensHoursOffset,
                         kTecoTimerTensHoursSize) * 60 * 10 +
               GETBITS64(remote_state, kTecoTimerUnitHoursOffset,
                         kTecoTimerUnitHoursSize) * 60;
        if (GETBIT64(remote_state, kTecoTimerHalfHourOffset)) mins += 30;
    }
    return mins;
}

/// Set the timer for when the A/C unit will switch power state.
/// @param[in] nr_mins Number of minutes before power state change.
///   `0` will clear the timer. Max is 24 hrs.
/// @note Time is stored internally in increments of 30 mins.
void Ir_Teco::setTimer(const uint16_t nr_mins) {
    uint16_t mins = std::min(nr_mins, (uint16_t)(24 * 60));  // Limit to 24 hrs.
    uint8_t hours = mins / 60;
    setBit(&remote_state, kTecoTimerOnOffset, mins);  // Set the timer flag.
    // Set the half hour bit.
    setBit(&remote_state, kTecoTimerHalfHourOffset, (mins % 60) >= 30);
    // Set the unit hours.
    setBits(&remote_state, kTecoTimerUnitHoursOffset, kTecoTimerUnitHoursSize,
            hours % 10);
    // Set the tens of hours.
    setBits(&remote_state, kTecoTimerTensHoursOffset, kTecoTimerTensHoursSize,
            hours / 10);
}


/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/


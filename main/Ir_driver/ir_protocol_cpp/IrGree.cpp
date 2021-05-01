/*
 * IrGree.cpp
 *
 *  Created on: Mar 9, 2021
 *      Author: ductu
 */

#include "IrGree.h"
#include "IRutils.h"
#include "IrKelvinator.h"

/***********************************************************************************************************************
 * Pragma directive
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes <System Includes>
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/
// Constants
const uint16_t kGreeHdrMark = 9000;
const uint16_t kGreeHdrSpace = 4500;  ///< See #684 & real example in unit tests
const uint16_t kGreeBitMark = 620;
const uint16_t kGreeOneSpace = 1600;
const uint16_t kGreeZeroSpace = 540;
const uint16_t kGreeMsgSpace = 19000;
const uint8_t kGreeBlockFooter = 0b010;
const uint8_t kGreeBlockFooterBits = 3;
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
Ir_Gree::Ir_Gree(uint32_t frequency, uint16_t dutycycle): _ir_gree(frequency, dutycycle){
	// TODO Auto-generated constructor stub
    stateReset();
    setModel( gree_ac_remote_model_t::YAW1F);
}

Ir_Gree::~Ir_Gree() {
	// TODO Auto-generated destructor stub
}
void Ir_Gree::setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp) {
    setPower(power);
    setMode(convertMode(stdAc::opmode_t(mode)));
    setFan(convertFan(stdAc::fanspeed_t(speed)));
    setTemp(temp);
}

/// Send a Gree Heat Pump formatted message.
/// Status: STABLE / Working.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void Ir_Gree::send(const uint8_t data[], const uint16_t nbytes,
                      const uint16_t repeat) {
  if (nbytes < kGreeStateLength)
    return;  // Not enough bytes to send a proper message.

  for (uint16_t r = 0; r <= repeat; r++) {
    // Block #1
	  _ir_gree.sendGeneric(kGreeHdrMark, kGreeHdrSpace, kGreeBitMark, kGreeOneSpace,
                kGreeBitMark, kGreeZeroSpace, 0, 0,  // No Footer.
                data, 4, 38, false, 0, 50);
    // Footer #1
	  _ir_gree.sendGeneric(0, 0,  // No Header
                kGreeBitMark, kGreeOneSpace, kGreeBitMark, kGreeZeroSpace,
                kGreeBitMark, kGreeMsgSpace, 0b010, 3, 38, false, 0, 50);

    // Block #2
	  _ir_gree.sendGeneric(0, 0,  // No Header for Block #2
                kGreeBitMark, kGreeOneSpace, kGreeBitMark, kGreeZeroSpace,
                kGreeBitMark, kGreeMsgSpace, data + 4, nbytes - 4, 38, false, 0,
                50);
  }
}

/// Send a Gree Heat Pump formatted message.
/// Status: STABLE / Working.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void Ir_Gree::send(const uint64_t data, const uint16_t nbits,
                      const uint16_t repeat) {
  if (nbits != kGreeBits)
    return;  // Wrong nr. of bits to send a proper message.
  // Set IR carrier frequency

  for (uint16_t r = 0; r <= repeat; r++) {
    // Header
	  _ir_gree.mark_space(kGreeHdrMark, kGreeHdrSpace);

    // Data
    for (int16_t i = 8; i <= nbits; i += 8) {
    	 _ir_gree.sendData(kGreeBitMark, kGreeOneSpace, kGreeBitMark, kGreeZeroSpace,
               (data >> (nbits - i)) & 0xFF, 8, false);
      if (i == nbits / 2) {
        // Send the mid-message Footer.
    	  _ir_gree.sendData(kGreeBitMark, kGreeOneSpace, kGreeBitMark, kGreeZeroSpace, 0b010, 3);
    	  _ir_gree.mark_space(kGreeBitMark, kGreeMsgSpace);
      }
    }
    // Footer
    _ir_gree.mark_space(kGreeBitMark, kGreeMsgSpace);
  }
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t Ir_Gree::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kGreeCool;
    case stdAc::opmode_t::kHeat: return kGreeHeat;
    case stdAc::opmode_t::kDry:  return kGreeDry;
    case stdAc::opmode_t::kFan:  return kGreeFan;
    default:                     return kGreeAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t Ir_Gree::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:    return kGreeFanMin;
    case stdAc::fanspeed_t::kLow:
    case stdAc::fanspeed_t::kMedium: return kGreeFanMax - 1;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:    return kGreeFanMax;
    default:                         return kGreeFanAuto;
  }
}

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] model The enum of the model to be emulated.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?

/// Reset the internal state to a fixed known good state.
void Ir_Gree::stateReset(void) {
    // This resets to a known-good state to Power Off, Fan Auto, Mode Auto, 25C.
    memset(_.remote_state, 0, sizeof _.remote_state);
    _.Temp = 9;  // _.remote_state[1] = 0x09;
    _.Light = true;  // _.remote_state[2] = 0x20;
    _.unknown1 = 5;  // _.remote_state[3] = 0x50;
    _.unknown2 = 4;  // _.remote_state[5] = 0x20;
}

/// Fix up the internal state so it is correct.
/// @note Internal use only.
void Ir_Gree::fixup(void) {
    setPower(getPower());  // Redo the power bits as they differ between models.
    checksum();  // Calculate the checksums
}

/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void Ir_Gree::sendGree(const uint16_t repeat) {
    send(getRaw(), kGreeStateLength, repeat);
}


/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t* Ir_Gree::getRaw(void) {
    fixup();  // Ensure correct settings before sending.
    return _.remote_state;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] new_code A valid code for this protocol.
void Ir_Gree::setRaw(const uint8_t new_code[]) {
    memcpy(_.remote_state, new_code, kGreeStateLength);
    // We can only detect the difference between models when the power is on.
    if (_.Power) {
        if (_.ModelA)
            _model = gree_ac_remote_model_t::YAW1F;
        else
            _model = gree_ac_remote_model_t::YBOFB;
    }
}

/// Calculate and set the checksum values for the internal state.
/// @param[in] length The size/length of the state array to fix the checksum of.
void Ir_Gree::checksum(const uint16_t length) {
    // Gree uses the same checksum alg. as Kelvinator's block checksum.
    _.Sum = Ir_Kelvinator::calcBlockChecksum(_.remote_state, length);
}

/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @param[in] length The length of the state array.
/// @return true, if the state has a valid checksum. Otherwise, false.
bool Ir_Gree::validChecksum(const uint8_t state[], const uint16_t length) {
    // Top 4 bits of the last byte in the state is the state's checksum.
    return GETBITS8(state[length - 1], kHighNibble, kNibbleSize) ==
    		Ir_Kelvinator::calcBlockChecksum(state, length);
}

/// Set the model of the A/C to emulate.
/// @param[in] model The enum of the appropriate model.
void Ir_Gree::setModel(const gree_ac_remote_model_t model) {
    switch (model) {
        case gree_ac_remote_model_t::YAW1F:
        case gree_ac_remote_model_t::YBOFB: _model = model; break;
        default: _model = gree_ac_remote_model_t::YAW1F;
    }
}

/// Get/Detect the model of the A/C.
/// @return The enum of the compatible model.
gree_ac_remote_model_t Ir_Gree::getModel(void) const { return _model; }

/// Change the power setting to On.
void Ir_Gree::on(void) { setPower(true); }

/// Change the power setting to Off.
void Ir_Gree::off(void) { setPower(false); }

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/814
void Ir_Gree::setPower(const bool on) {
    _.Power = on;
    // May not be needed. See #814
    _.ModelA = (on && _model == gree_ac_remote_model_t::YAW1F);
}

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/814
bool Ir_Gree::getPower(void) const {
    //  See #814. Not checking/requiring: (_.ModelA)
    return _.Power;
}

/// Set the default temperature units to use.
/// @param[in] on Use Fahrenheit as the units.
///   true is Fahrenheit, false is Celsius.
void Ir_Gree::setUseFahrenheit(const bool on) {
    _.UseFahrenheit = on;
}

/// Get the default temperature units in use.
/// @return true is Fahrenheit, false is Celsius.
bool Ir_Gree::getUseFahrenheit(void) const {
    return _.UseFahrenheit;
}

/// Set the temp. in degrees
/// @param[in] temp Desired temperature in Degrees.
/// @param[in] fahrenheit Use units of Fahrenheit and set that as units used.
///   false is Celsius (Default), true is Fahrenheit.
/// @note The unit actually works in Celsius with a special optional
///   "extra degree" when sending Fahrenheit.
void Ir_Gree::setTemp(const uint8_t temp, const bool fahrenheit) {
    float safecelsius = temp;
    if (fahrenheit)
        // Covert to F, and add a fudge factor to round to the expected degree.
        // Why 0.6 you ask?! Because it works. Ya'd thing 0.5 would be good for
        // rounding, but Noooooo!
        safecelsius = fahrenheitToCelsius(temp + 0.6);
    setUseFahrenheit(fahrenheit);  // Set the correct Temp units.

    // Make sure we have desired temp in the correct range.
    safecelsius = std::max(static_cast<float>(kGreeMinTempC), safecelsius);
    safecelsius = std::min(static_cast<float>(kGreeMaxTempC), safecelsius);
    // An operating mode of Auto locks the temp to a specific value. Do so.
    if (_.Mode == kGreeAuto) safecelsius = 25;

    // Set the "main" Celsius degrees.
    _.Temp = safecelsius - kGreeMinTempC;
    // Deal with the extra degree fahrenheit difference.
    _.TempExtraDegreeF = (static_cast<uint8_t>(safecelsius * 2) & 1);
}

/// Get the set temperature
/// @return The temperature in degrees in the current units (C/F) set.
uint8_t Ir_Gree::getTemp(void) const {
    uint8_t deg = kGreeMinTempC + _.Temp;
    if (_.UseFahrenheit) {
        deg = celsiusToFahrenheit(deg);
        // Retrieve the "extra" fahrenheit from elsewhere in the code.
        if (_.TempExtraDegreeF) deg++;
        deg = std::max(deg, kGreeMinTempF);  // Cover the fact that 61F is < 16C
    }
    return deg;
}

/// Set the speed of the fan.
/// @param[in] speed The desired setting. 0 is auto, 1-3 is the speed.
void Ir_Gree::setFan(const uint8_t speed) {
    uint8_t fan = std::min(kGreeFanMax, speed);  // Bounds check
    if (_.Mode == kGreeDry) fan = 1;  // DRY mode is always locked to fan 1.
    // Set the basic fan values.
    _.Fan = fan;
}

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t Ir_Gree::getFan(void) const {
    return _.Fan;
}

/// Set the operating mode of the A/C.
/// @param[in] new_mode The desired operating mode.
void Ir_Gree::setMode(const uint8_t new_mode) {
    uint8_t mode = new_mode;
    switch (mode) {
        // AUTO is locked to 25C
        case kGreeAuto: setTemp(25); break;
            // DRY always sets the fan to 1.
        case kGreeDry: setFan(1); break;
        case kGreeCool:
        case kGreeFan:
        case kGreeHeat: break;
            // If we get an unexpected mode, default to AUTO.
        default: mode = kGreeAuto;
    }
    _.Mode = mode;
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t Ir_Gree::getMode(void) const {
    return _.Mode;
}

/// Set the Light (LED) setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void Ir_Gree::setLight(const bool on) {
    _.Light = on;
}

/// Get the Light (LED) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool Ir_Gree::getLight(void) const {
    return _.Light;
}

/// Set the IFeel setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void Ir_Gree::setIFeel(const bool on) {
    _.IFeel = on;
}

/// Get the IFeel setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool Ir_Gree::getIFeel(void) const {
    return _.IFeel;
}

/// Set the Wifi (enabled) setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void Ir_Gree::setWiFi(const bool on) {
    _.WiFi = on;
}

/// Get the Wifi (enabled) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool Ir_Gree::getWiFi(void) const {
    return _.WiFi;
}

/// Set the XFan (Mould) setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void Ir_Gree::setXFan(const bool on) {
    _.Xfan = on;
}

/// Get the XFan (Mould) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool Ir_Gree::getXFan(void) const {
    return _.Xfan;
}

/// Set the Sleep setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void Ir_Gree::setSleep(const bool on) {
    _.Sleep = on;
}

/// Get the Sleep setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool Ir_Gree::getSleep(void) const {
    return _.Sleep;
}

/// Set the Turbo setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void Ir_Gree::setTurbo(const bool on) {
    _.Turbo = on;
}

/// Get the Turbo setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool Ir_Gree::getTurbo(void) const {
    return _.Turbo;
}

/// Set the Vertical Swing mode of the A/C.
/// @param[in] automatic Do we use the automatic setting?
/// @param[in] position The position/mode to set the vanes to.
void Ir_Gree::setSwingVertical(const bool automatic, const uint8_t position) {
    _.SwingAuto = automatic;
    uint8_t new_position = position;
    if (!automatic) {
        switch (position) {
            case kGreeSwingUp:
            case kGreeSwingMiddleUp:
            case kGreeSwingMiddle:
            case kGreeSwingMiddleDown:
            case kGreeSwingDown:
                break;
            default:
                new_position = kGreeSwingLastPos;
        }
    } else {
        switch (position) {
            case kGreeSwingAuto:
            case kGreeSwingDownAuto:
            case kGreeSwingMiddleAuto:
            case kGreeSwingUpAuto:
                break;
            default:
                new_position = kGreeSwingAuto;
        }
    }
    _.Swing = new_position;
}

/// Get the Vertical Swing Automatic mode setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool Ir_Gree::getSwingVerticalAuto(void) const {
    return _.SwingAuto;
}

/// Get the Vertical Swing position setting of the A/C.
/// @return The native position/mode.
uint8_t Ir_Gree::getSwingVerticalPosition(void) const {
    return _.Swing;
}

/// Set the timer enable setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void Ir_Gree::setTimerEnabled(const bool on) {
    _.TimerEnabled = on;
}

/// Get the timer enabled setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool Ir_Gree::getTimerEnabled(void) const {
    return _.TimerEnabled;
}

/// Get the timer time value from the A/C.
/// @return The number of minutes the timer is set for.
uint16_t Ir_Gree::getTimer(void) const {
    uint16_t hrs = irutils::bcdToUint8((_.TimerTensHr << kNibbleSize) |
                                       _.TimerHours);
    return hrs * 60 + (_.TimerHalfHr ? 30 : 0);
}

/// Set the A/C's timer to turn off in X many minutes.
/// @param[in] minutes The number of minutes the timer should be set for.
/// @note Stores time internally in 30 min units.
///  e.g. 5 mins means 0 (& Off), 95 mins is  90 mins (& On). Max is 24 hours.
void Ir_Gree::setTimer(const uint16_t minutes) {
    uint16_t mins = std::min(kGreeTimerMax, minutes);  // Bounds check.
    setTimerEnabled(mins >= 30);  // Timer is enabled when >= 30 mins.
    uint8_t hours = mins / 60;
    // Set the half hour bit.
    _.TimerHalfHr = (mins % 60) >= 30;
    // Set the "tens" digit of hours.
    _.TimerTensHr = hours / 10;
    // Set the "units" digit of hours.
    _.TimerHours = hours % 10;
}

/// Set temperature display mode.
/// i.e. Internal, External temperature sensing.
/// @param[in] mode The desired temp source to display.
/// @note In order for the A/C unit properly accept these settings. You must
///   cycle (send) in the following order:
///   kGreeDisplayTempOff(0) -> kGreeDisplayTempSet(1) ->
///   kGreeDisplayTempInside(2) ->kGreeDisplayTempOutside(3) ->
///   kGreeDisplayTempOff(0).
///   The unit will no behave correctly if the changes of this setting are sent
///   out of order.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1118#issuecomment-628242152
void Ir_Gree::setDisplayTempSource(const uint8_t mode) {
    _.DisplayTemp = mode;
}

/// Get the temperature display mode.
/// i.e. Internal, External temperature sensing.
/// @return The current temp source being displayed.
uint8_t Ir_Gree::getDisplayTempSource(void) const {
    return _.DisplayTemp;
}


/***********************************************************************************************************************
 * Static Functions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/


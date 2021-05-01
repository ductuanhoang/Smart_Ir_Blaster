/*
 * IrMedia.cpp
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
#include "IrMidea.h"
#include "IRutils.h"
/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/

// Constants
const uint16_t kMideaTick = 80;
const uint16_t kMideaBitMarkTicks = 7;
const uint16_t kMideaBitMark = kMideaBitMarkTicks * kMideaTick;
const uint16_t kMideaOneSpaceTicks = 21;
const uint16_t kMideaOneSpace = kMideaOneSpaceTicks * kMideaTick;
const uint16_t kMideaZeroSpaceTicks = 7;
const uint16_t kMideaZeroSpace = kMideaZeroSpaceTicks * kMideaTick;
const uint16_t kMideaHdrMarkTicks = 56;
const uint16_t kMideaHdrMark = kMideaHdrMarkTicks * kMideaTick;
const uint16_t kMideaHdrSpaceTicks = 56;
const uint16_t kMideaHdrSpace = kMideaHdrSpaceTicks * kMideaTick;
const uint16_t kMideaMinGapTicks =
        kMideaHdrMarkTicks + kMideaZeroSpaceTicks + kMideaBitMarkTicks;
const uint16_t kMideaMinGap = kMideaMinGapTicks * kMideaTick;
const uint8_t kMideaTolerance = 30;  // Percent
const uint16_t kMidea24MinGap = 13000;  ///< uSecs
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
Ir_Midea::Ir_Midea(uint32_t frequency, uint16_t dutycycle):_ir_remote(frequency, dutycycle) {
	// TODO Auto-generated constructor stub
	stateReset();
}

Ir_Midea::~Ir_Midea() {
	// TODO Auto-generated destructor stub
}

void Ir_Midea::sendMidea(const uint16_t repeat) {
	send(getRaw(), kMideaBits, repeat);
	  // Handle the toggle/special "one-off" settings if we need to.
	  if (_SwingVToggle && !isSwingVToggle())
		  send(kMideaACToggleSwingV, kMideaBits, repeat);
	  _SwingVToggle = false;
	#if KAYSUN_AC
	  if (_SwingVStep && !isSwingVStep())
	    _irsend.sendMidea(kMideaACSwingVStep, kMideaBits, repeat);
	  _SwingVStep = false;
	#endif  // KAYSUN_AC
	  if (_EconoToggle && !isEconoToggle())
		  send(kMideaACToggleEcono, kMideaBits, repeat);
	  _EconoToggle = false;
	  if (_TurboToggle && !isTurboToggle())
		  send(kMideaACToggleTurbo, kMideaBits, repeat);
	  _TurboToggle = false;
	  if (_LightToggle && !isLightToggle())
		  send(kMideaACToggleLight, kMideaBits, repeat);
	  _LightToggle = false;
}

void Ir_Midea::setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp) {
    setPower(power);
    setMode(convertMode(stdAc::opmode_t(mode)));
    setFan(convertFan(stdAc::fanspeed_t(speed)));
    setTemp(temp);
}


/***********************************************************************************************************************
 * Static Functions
 ***********************************************************************************************************************/
/// Send a Midea message
/// Status: Alpha / Needs testing against a real device.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void Ir_Midea::send(uint64_t data, uint16_t nbits, uint16_t repeat) {
  if (nbits % 8 != 0) return;  // nbits is required to be a multiple of 8.

  for (uint16_t r = 0; r <= repeat; r++) {
    // The protocol sends the message, then follows up with an entirely
    // inverted payload.
    for (size_t inner_loop = 0; inner_loop < 2; inner_loop++) {
      // Header
    	_ir_remote.mark_space(kMideaHdrMark, kMideaHdrSpace);
		// Data
		//   Break data into byte segments, starting at the Most Significant
		//   Byte. Each byte then being sent normal, then followed inverted.
		for (uint16_t i = 8; i <= nbits; i += 8) {
		// Grab a bytes worth of data.
		uint8_t segment = (data >> (nbits - i)) & 0xFF;
		_ir_remote.sendData(kMideaBitMark, kMideaOneSpace, kMideaBitMark, kMideaZeroSpace,
				 segment, 8, true);
		}
		// Footer
//		mark(kMideaBitMark);
//		space(kMideaMinGap);  // Pause before repeating
		_ir_remote.mark_space(kMideaBitMark, kMideaMinGap);
		// Invert the data for the 2nd phase of the message.
		// As we get called twice in the inner loop, we will always revert
		// to the original 'data' state.
		data = ~data;
    }
    _ir_remote.space(kDefaultMessageGap);
  }
}


/// Reset the state of the remote to a known good state/sequence.
void Ir_Midea::stateReset(void) {
    // Power On, Mode Auto, Fan Auto, Temp = 25C/77F
    _.remote_state = 0xA1826FFFFF62;
    _SwingVToggle = false;
    _EconoToggle = false;
    _TurboToggle = false;
    _LightToggle = false;
#if KAYSUN_AC
    _SwingVStep = false;
#endif  // KAYSUN_AC
}

/// Get a copy of the internal state/code for this protocol.
/// @return The code for this protocol based on the current internal state.
uint64_t Ir_Midea::getRaw(void) {
    checksum();  // Ensure correct checksum before sending.
    return _.remote_state;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] newState A valid code for this protocol.
void Ir_Midea::setRaw(const uint64_t newState) { _.remote_state = newState; }

/// Set the requested power state of the A/C to on.
void Ir_Midea::on(void) { setPower(true); }

/// Set the requested power state of the A/C to off.
void Ir_Midea::off(void) { setPower(false); }

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void Ir_Midea::setPower(const bool on) {
    _.Power = on;
}

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool Ir_Midea::getPower(void) const {
    return _.Power;
}

/// Is the device currently using Celsius or the Fahrenheit temp scale?
/// @return true, the A/C unit uses Celsius natively, false, is Fahrenheit.
bool Ir_Midea::getUseCelsius(void) const {
    return !_.useFahrenheit;
}

/// Set the A/C unit to use Celsius natively.
/// @param[in] on true, the setting is on. false, the setting is off.
void Ir_Midea::setUseCelsius(const bool on) {
    if (on == _.useFahrenheit) {  // We need to change.
        uint8_t native_temp = getTemp(!on);  // Get the old native temp.
        _.useFahrenheit = !on;  // Cleared is on.
        setTemp(native_temp, !on);  // Reset temp using the old native temp.
    }
}

/// Set the temperature.
/// @param[in] temp The temperature in degrees celsius.
/// @param[in] useCelsius true, use the Celsius temp scale. false, is Fahrenheit
void Ir_Midea::setTemp(const uint8_t temp, const bool useCelsius) {
    uint8_t max_temp = kMideaACMaxTempF;
    uint8_t min_temp = kMideaACMinTempF;
    if (useCelsius) {
        max_temp = kMideaACMaxTempC;
        min_temp = kMideaACMinTempC;
    }
    uint8_t new_temp = std::min(max_temp, std::max(min_temp, temp));
    if (!_.useFahrenheit && !useCelsius)  // Native is in C, new_temp is in F
        new_temp = fahrenheitToCelsius(new_temp) - kMideaACMinTempC;
    else if (_.useFahrenheit && useCelsius)  // Native is in F, new_temp is in C
        new_temp = celsiusToFahrenheit(new_temp) - kMideaACMinTempF;
    else  // Native and desired are the same units.
        new_temp -= min_temp;
    // Set the actual data.
    _.Temp = new_temp;
}

/// Get the current temperature setting.
/// @param[in] celsius true, the results are in Celsius. false, in Fahrenheit.
/// @return The current setting for temp. in the requested units/scale.
uint8_t Ir_Midea::getTemp(const bool celsius) const {
    uint8_t temp = _.Temp;
    if (!_.useFahrenheit)
        temp += kMideaACMinTempC;
    else
        temp += kMideaACMinTempF;
    if (celsius && _.useFahrenheit) temp = fahrenheitToCelsius(temp) + 0.5;
    if (!celsius && !_.useFahrenheit) temp = celsiusToFahrenheit(temp);
    return temp;
}

/// Set the Sensor temperature.
/// @param[in] temp The temperature in degrees celsius.
/// @param[in] useCelsius true, use the Celsius temp scale. false, is Fahrenheit
/// @note Also known as FollowMe
void Ir_Midea::setSensorTemp(const uint8_t temp, const bool useCelsius) {
    uint8_t max_temp = kMideaACMaxSensorTempF;
    uint8_t min_temp = kMideaACMinSensorTempF;
    if (useCelsius) {
        max_temp = kMideaACMaxSensorTempC;
        min_temp = kMideaACMinSensorTempC;
    }
    uint8_t new_temp = std::min(max_temp, std::max(min_temp, temp));
    if (!_.useFahrenheit && !useCelsius)  // Native is in C, new_temp is in F
        new_temp = fahrenheitToCelsius(new_temp) - kMideaACMinSensorTempC;
    else if (_.useFahrenheit && useCelsius)  // Native is in F, new_temp is in C
        new_temp = celsiusToFahrenheit(new_temp) - kMideaACMinSensorTempF;
    else  // Native and desired are the same units.
        new_temp -= min_temp;
    // Set the actual data.
    _.SensorTemp = new_temp + 1;
    setEnableSensorTemp(true);
}

/// Get the current Sensor temperature setting.
/// @param[in] celsius true, the results are in Celsius. false, in Fahrenheit.
/// @return The current setting for temp. in the requested units/scale.
/// @note Also known as FollowMe
uint8_t Ir_Midea::getSensorTemp(const bool celsius) const {
    uint8_t temp = _.SensorTemp - 1;
    if (!_.useFahrenheit)
        temp += kMideaACMinSensorTempC;
    else
        temp += kMideaACMinSensorTempF;
    if (celsius && _.useFahrenheit) temp = fahrenheitToCelsius(temp) + 0.5;
    if (!celsius && !_.useFahrenheit) temp = celsiusToFahrenheit(temp);
    return temp;
}

/// Enable the remote's Sensor temperature.
/// @param[in] on true, the setting is on. false, the setting is off.
/// @note Also known as FollowMe
void Ir_Midea::setEnableSensorTemp(const bool on) {
    _.disableSensor = !on;
    if (on) {
        setType(kMideaACTypeFollow);
    } else {
        setType(kMideaACTypeCommand);
        _.SensorTemp = kMideaACSensorTempOnTimerOff;  // Apply special value if off.
    }
}

/// Is the remote temperature sensor enabled?
/// @return A boolean indicating if it is enabled or not.
/// @note Also known as FollowMe
bool Ir_Midea::getEnableSensorTemp(void) const { return !_.disableSensor; }

/// Set the speed of the fan.
/// @param[in] fan The desired setting. 1-3 set the speed, 0 for auto.
void Ir_Midea::setFan(const uint8_t fan) {
    _.Fan = (fan > kMideaACFanHigh) ? kMideaACFanAuto : fan;
}

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t Ir_Midea::getFan(void) const {
    return _.Fan;
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t Ir_Midea::getMode(void) const {
    return _.Mode;
}

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void Ir_Midea::setMode(const uint8_t mode) {
    switch (mode) {
        case kMideaACAuto:
        case kMideaACCool:
        case kMideaACHeat:
        case kMideaACDry:
        case kMideaACFan:
            _.Mode = mode;
            break;
        default:
            _.Mode = kMideaACAuto;
    }
}

/// Set the Sleep setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void Ir_Midea::setSleep(const bool on) {
    _.Sleep = on;
}

/// Get the Sleep setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool Ir_Midea::getSleep(void) const {
    return _.Sleep;
}

/// Set the A/C to toggle the vertical swing toggle for the next send.
/// @note On Danby A/C units, this is associated with the Ion Filter instead.
/// @param[in] on true, the setting is on. false, the setting is off.
void Ir_Midea::setSwingVToggle(const bool on) { _SwingVToggle = on; }

/// Is the current state a vertical swing toggle message?
/// @note On Danby A/C units, this is associated with the Ion Filter instead.
/// @return true, it is. false, it isn't.
bool Ir_Midea::isSwingVToggle(void) const {
    return _.remote_state == kMideaACToggleSwingV;
}

// Get the vertical swing toggle state of the A/C.
/// @note On Danby A/C units, this is associated with the Ion Filter instead.
/// @return true, the setting is on. false, the setting is off.
bool Ir_Midea::getSwingVToggle(void) {
    _SwingVToggle |= isSwingVToggle();
    return _SwingVToggle;
}

#if KAYSUN_AC
/// Set the A/C to step the vertical swing for the next send.
/// @param[in] on true, the setting is on. false, the setting is off.
void Ir_Midea::setSwingVStep(const bool on) { _SwingVStep = on; }

/// Is the current state a step vertical swing message?
/// @return true, it is. false, it isn't.
bool Ir_Midea::isSwingVStep(void) const {
  return _.remote_state == kMideaACSwingVStep;
}

// Get the step vertical swing state of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool Ir_Midea::getSwingVStep(void) {
  _SwingVStep |= isSwingVStep();
  return _SwingVStep;
}
#endif  // KAYSUN_AC

/// Set the A/C to toggle the Econo (energy saver) mode for the next send.
/// @param[in] on true, the setting is on. false, the setting is off.
void Ir_Midea::setEconoToggle(const bool on) { _EconoToggle = on; }

/// Is the current state an Econo (energy saver) toggle message?
/// @return true, it is. false, it isn't.
bool Ir_Midea::isEconoToggle(void) const {
    return _.remote_state == kMideaACToggleEcono;
}

// Get the Econo (energy saver) toggle state of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool Ir_Midea::getEconoToggle(void) {
    _EconoToggle |= isEconoToggle();
    return _EconoToggle;
}

/// Set the A/C to toggle the Turbo mode for the next send.
/// @param[in] on true, the setting is on. false, the setting is off.
void Ir_Midea::setTurboToggle(const bool on) { _TurboToggle = on; }

/// Is the current state a Turbo toggle message?
/// @return true, it is. false, it isn't.
bool Ir_Midea::isTurboToggle(void) const {
    return _.remote_state == kMideaACToggleTurbo;
}

// Get the Turbo toggle state of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool Ir_Midea::getTurboToggle(void) {
    _TurboToggle |= isTurboToggle();
    return _TurboToggle;
}

/// Set the A/C to toggle the Light (LED) mode for the next send.
/// @param[in] on true, the setting is on. false, the setting is off.
void Ir_Midea::setLightToggle(const bool on) { _LightToggle = on; }

/// Is the current state a Light (LED) toggle message?
/// @return true, it is. false, it isn't.
bool Ir_Midea::isLightToggle(void) const {
    return _.remote_state == kMideaACToggleLight;
}

// Get the Light (LED) toggle state of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool Ir_Midea::getLightToggle(void) {
    _LightToggle |= isLightToggle();
    return _LightToggle;
}

/// Calculate the checksum for a given state.
/// @param[in] state The value to calc the checksum of.
/// @return The calculated checksum value.
uint8_t Ir_Midea::calcChecksum(const uint64_t state) {
    uint8_t sum = 0;
    uint64_t temp_state = state;

    for (uint8_t i = 0; i < 5; i++) {
        temp_state >>= 8;
        sum += reverseBits((temp_state & 0xFF), 8);
    }
    sum = 256 - sum;
    return reverseBits(sum, 8);
}

/// Verify the checksum is valid for a given state.
/// @param[in] state The state to verify the checksum of.
/// @return true, if the state has a valid checksum. Otherwise, false.
bool Ir_Midea::validChecksum(const uint64_t state) {
    return GETBITS64(state, 0, 8) == calcChecksum(state);
}

/// Calculate & set the checksum for the current internal state of the remote.
void Ir_Midea::checksum(void) {
    // Stored the checksum value in the last byte.
    _.Sum = calcChecksum(_.remote_state);
}

/// Get the message type setting of the A/C message.
/// @return The message type setting.
uint8_t Ir_Midea::getType(void) const { return _.Type; }

/// Set the message type setting of the A/C message.
/// @param[in] setting The desired message type setting.
void Ir_Midea::setType(const uint8_t setting) {
    switch (setting) {
        case kMideaACTypeFollow:
            _.BeepDisable = false;
            // FALL-THRU
            break;
        case kMideaACTypeSpecial:
            _.Type = setting;
            break;
        default:
            _.Type = kMideaACTypeCommand;
            _.BeepDisable = true;
    }
}

/// Is the OnTimer enabled?
/// @return true for yes, false for no.
bool Ir_Midea::isOnTimerEnabled(void) const {
    return getType() == kMideaACTypeCommand &&
           _.SensorTemp != kMideaACSensorTempOnTimerOff;
}

/// Get the value of the OnTimer is currently set to.
/// @return The number of minutes.
uint16_t Ir_Midea::getOnTimer(void) const {
    return (_.SensorTemp >> 1) * 30 + 30;
}

/// Set the value of the On Timer.
/// @param[in] mins The number of minutes for the timer.
/// @note Time will be rounded down to nearest 30 min as that is the resolution
///       of the actual device/protocol.
/// @note A value of less than 30 will disable the Timer.
/// @warning On Timer is incompatible with Sensor Temp/Follow Me messages.
///          Setting it will disable that mode/settings.
void Ir_Midea::setOnTimer(const uint16_t mins) {
    setEnableSensorTemp(false);
    uint8_t halfhours = std::min((uint16_t)(24 * 60), mins) / 30;
    if (halfhours)
        _.SensorTemp = ((halfhours - 1) << 1) | 1;
    else
        _.SensorTemp = kMideaACSensorTempOnTimerOff;
}

/// Is the OffTimer enabled?
/// @return true for yes, false for no.
bool Ir_Midea::isOffTimerEnabled(void) const {
    return _.OffTimer != kMideaACTimerOff;
}

/// Get the value of the OffTimer is currently set to.
/// @return The number of minutes.
uint16_t Ir_Midea::getOffTimer(void) const { return _.OffTimer * 30 + 30; }

/// Set the value of the Off Timer.
/// @param[in] mins The number of minutes for the timer.
/// @note Time will be rounded down to nearest 30 min as that is the resolution
///       of the actual device/protocol.
/// @note A value of less than 30 will disable the Timer.
void Ir_Midea::setOffTimer(const uint16_t mins) {
    uint8_t halfhours = std::min((uint16_t)(24 * 60), mins) / 30;
    if (halfhours)
        _.OffTimer = halfhours - 1;
    else
        _.OffTimer = kMideaACTimerOff;
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t Ir_Midea::convertMode(const stdAc::opmode_t mode) {
    switch (mode) {
        case stdAc::opmode_t::kCool: return kMideaACCool;
        case stdAc::opmode_t::kHeat: return kMideaACHeat;
        case stdAc::opmode_t::kDry:  return kMideaACDry;
        case stdAc::opmode_t::kFan:  return kMideaACFan;
        default:                     return kMideaACAuto;
    }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t Ir_Midea::convertFan(const stdAc::fanspeed_t speed) {
    switch (speed) {
        case stdAc::fanspeed_t::kMin:
        case stdAc::fanspeed_t::kLow:    return kMideaACFanLow;
        case stdAc::fanspeed_t::kMedium: return kMideaACFanMed;
        case stdAc::fanspeed_t::kHigh:
        case stdAc::fanspeed_t::kMax:    return kMideaACFanHigh;
        default:                         return kMideaACFanAuto;
    }
}


#if SEND_MIDEA24
/// Send a Midea24 formatted message.
/// Status: STABLE / Confirmed working on a real device.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1170
/// @note This protocol is basically a 48-bit version of the NEC protocol with
///   alternate bytes inverted, thus only 24 bits of real data, and with at
///   least a single repeat.
/// @warning Can't be used beyond 32 bits.
void IRsend::sendMidea24(const uint64_t data, const uint16_t nbits,
                          const uint16_t repeat) {
  uint64_t newdata = 0;
  // Construct the data into bye & inverted byte pairs.
  for (int16_t i = nbits - 8; i >= 0; i -= 8) {
    // Shuffle the data to be sent so far.
    newdata <<= 16;
    uint8_t next = GETBITS64(data, i, 8);
    newdata |= ((next << 8) | (next ^ 0xFF));
  }
  sendNEC(newdata, nbits * 2, repeat);
}
#endif  // SEND_MIDEA24

/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/


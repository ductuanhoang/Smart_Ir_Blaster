/*
 * IrNeoclimaAc.cpp
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
#include "IrNeoclimaAc.h"
#include "IRutils.h"
/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/
using std::min;
using std::max;
/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/

// Constants
const uint16_t kNeoclimaHdrMark = 6112;
const uint16_t kNeoclimaHdrSpace = 7391;
const uint16_t kNeoclimaBitMark = 537;
const uint16_t kNeoclimaOneSpace = 1651;
const uint16_t kNeoclimaZeroSpace = 571;
const uint32_t kNeoclimaMinGap = kDefaultMessageGap;
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
Ir_NeoclimaAc::Ir_NeoclimaAc(uint32_t frequency, uint16_t dutycycle): _ir_Neoclima_remote(frequency, dutycycle){
	// TODO Auto-generated constructor stub
    stateReset();
}

Ir_NeoclimaAc::~Ir_NeoclimaAc() {
	// TODO Auto-generated destructor stub
}

void Ir_NeoclimaAc::setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp) {
    setPower(power);
    setMode(convertMode(stdAc::opmode_t(mode)));
    setFan(convertFan(stdAc::fanspeed_t(speed)));
    setTemp(temp);
}

/***********************************************************************************************************************
 * Static Functions
 ***********************************************************************************************************************/

/// Send a Neoclima message.
/// Status: STABLE / Known to be working.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void Ir_NeoclimaAc::send(const unsigned char data[], const uint16_t nbytes,
                         const uint16_t repeat) {
    // Set IR carrier frequency

    for (uint16_t i = 0; i <= repeat; i++) {
    	_ir_Neoclima_remote.sendGeneric(kNeoclimaHdrMark, kNeoclimaHdrSpace,
                    kNeoclimaBitMark, kNeoclimaOneSpace,
                    kNeoclimaBitMark, kNeoclimaZeroSpace,
                    kNeoclimaBitMark, kNeoclimaHdrSpace,
                    data, nbytes, 38000, false, 0,  // Repeats are already handled.
                    50);
        // Extra footer.
    	_ir_Neoclima_remote.mark_space(kNeoclimaBitMark, kNeoclimaMinGap);
//        mark(kNeoclimaBitMark);
//        space(kNeoclimaMinGap);
    }
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t Ir_NeoclimaAc::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kNeoclimaCool;
    case stdAc::opmode_t::kHeat: return kNeoclimaHeat;
    case stdAc::opmode_t::kDry:  return kNeoclimaDry;
    case stdAc::opmode_t::kFan:  return kNeoclimaFan;
    default:                     return kNeoclimaAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t Ir_NeoclimaAc::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:    return kNeoclimaFanLow;
    case stdAc::fanspeed_t::kMedium: return kNeoclimaFanMed;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:    return kNeoclimaFanHigh;
    default:                         return kNeoclimaFanAuto;
  }
}
/// Reset the state of the remote to a known good state/sequence.
void Ir_NeoclimaAc::stateReset(void) {
    static const uint8_t kReset[kNeoclimaStateLength] = {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6A, 0x00, 0x2A, 0xA5};
    setRaw(kReset);
}


/// Calculate the checksum for a given state.
/// @param[in] state The array to calc the checksum of.
/// @param[in] length The length/size of the array.
/// @return The calculated checksum value.
uint8_t Ir_NeoclimaAc::calcChecksum(const uint8_t state[],
                                    const uint16_t length) {
    if (length == 0) return state[0];
    return sumBytes(state, length - 1);
}

/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @param[in] length The length/size of the array.
/// @return true, if the state has a valid checksum. Otherwise, false.
bool Ir_NeoclimaAc::validChecksum(const uint8_t state[], const uint16_t length) {
    if (length < 2)
        return true;  // No checksum to compare with. Assume okay.
    return (state[length - 1] == calcChecksum(state, length));
}

/// Calculate & update the checksum for the internal state.
/// @param[in] length The length/size of the internal state.
void Ir_NeoclimaAc::checksum(uint16_t length) {
    if (length < 2) return;
    _.Sum = calcChecksum(_.raw, length);
}

/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void Ir_NeoclimaAc::sendNeoclimaAc(const uint16_t repeat) {
    send(getRaw(), kNeoclimaStateLength, repeat);
}

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t *Ir_NeoclimaAc::getRaw(void) {
    checksum();
    return _.raw;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] new_code A valid code for this protocol.
/// @param[in] length The length/size of the new_code array.
void Ir_NeoclimaAc::setRaw(const uint8_t new_code[], const uint16_t length) {
	if (length < kNeoclimaStateLength)
		memcpy(_.raw, new_code, length);
	else
		memcpy(_.raw, new_code, kNeoclimaStateLength);
}

/// Set the Button/Command pressed setting of the A/C.
/// @param[in] button The value of the button/command that was pressed.
void Ir_NeoclimaAc::setButton(const uint8_t button) {
    switch (button) {
        case kNeoclimaButtonPower:
        case kNeoclimaButtonMode:
        case kNeoclimaButtonTempUp:
        case kNeoclimaButtonTempDown:
        case kNeoclimaButtonSwing:
        case kNeoclimaButtonFanSpeed:
        case kNeoclimaButtonAirFlow:
        case kNeoclimaButtonHold:
        case kNeoclimaButtonSleep:
        case kNeoclimaButtonLight:
        case kNeoclimaButtonEye:
        case kNeoclimaButtonFollow:
        case kNeoclimaButtonIon:
        case kNeoclimaButtonFresh:
        case kNeoclimaButton8CHeat:
        case kNeoclimaButtonTurbo:
        case kNeoclimaButtonEcono:
        case kNeoclimaButtonTempUnit:
            _.Button = button;
            break;
        default:
            _.Button = kNeoclimaButtonPower;
    }
}

/// Get the Button/Command setting of the A/C.
/// @return The value of the button/command that was pressed.
uint8_t Ir_NeoclimaAc::getButton(void) const {
    return _.Button;
}

/// Set the requested power state of the A/C to on.
void Ir_NeoclimaAc::on(void) { setPower(true); }

/// Set the requested power state of the A/C to off.
void Ir_NeoclimaAc::off(void) { setPower(false); }

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void Ir_NeoclimaAc::setPower(const bool on) {
    _.Button = kNeoclimaButtonPower;
    _.Power = on;
}

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool Ir_NeoclimaAc::getPower(void) const {
    return _.Power;
}

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void Ir_NeoclimaAc::setMode(const uint8_t mode) {
    switch (mode) {
        case kNeoclimaDry:
            // In this mode fan speed always LOW
            setFan(kNeoclimaFanLow);
            // FALL THRU
        case kNeoclimaAuto:
        case kNeoclimaCool:
        case kNeoclimaFan:
        case kNeoclimaHeat:
            _.Mode = mode;
            _.Button = kNeoclimaButtonMode;
            break;
        default:
            // If we get an unexpected mode, default to AUTO.
            _.Mode = kNeoclimaAuto;
            _.Button = kNeoclimaButtonMode;
    }
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t Ir_NeoclimaAc::getMode(void) const {
    return _.Mode;
}

/// Set the temperature.
/// @param[in] temp The temperature in degrees celsius.
/// @param[in] celsius Use Fahrenheit (false) or Celsius (true).
void Ir_NeoclimaAc::setTemp(const uint8_t temp, const bool celsius) {
    uint8_t oldtemp = getTemp();
    _.UseFah = !celsius;
    const uint8_t min_temp = celsius ? kNeoclimaMinTempC : kNeoclimaMinTempF;
    const uint8_t max_temp = celsius ? kNeoclimaMaxTempC : kNeoclimaMaxTempF;
    const uint8_t newtemp = std::min(max_temp, std::max(min_temp, temp));
    if (oldtemp > newtemp)
        _.Button = kNeoclimaButtonTempDown;
    else if (newtemp > oldtemp)
        _.Button = kNeoclimaButtonTempUp;
    _.Temp = newtemp - min_temp;
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees.
/// @note The units of the temperature (F/C) is determined by `getTempUnits()`.
uint8_t Ir_NeoclimaAc::getTemp(void) const {
    const uint8_t min_temp = getTempUnits() ? kNeoclimaMinTempC
                                            : kNeoclimaMinTempF;
    return _.Temp + min_temp;
}

/// Set the speed of the fan.
/// @param[in] speed The desired setting. 0-3, 0 is auto, 1-3 is the speed
void Ir_NeoclimaAc::setFan(const uint8_t speed) {
    _.Button = kNeoclimaButtonFanSpeed;
    if (_.Mode == kNeoclimaDry) {  // Dry mode only allows low speed.
        _.Fan = kNeoclimaFanLow;
        return;
    }
    switch (speed) {
        case kNeoclimaFanAuto:
        case kNeoclimaFanHigh:
        case kNeoclimaFanMed:
        case kNeoclimaFanLow:
            _.Fan = speed;
            break;
        default:
            // If we get an unexpected speed, default to Auto.
            _.Fan = kNeoclimaFanAuto;
    }
}

/// Get the current fan speed setting.
/// @return The current fan speed/mode.
uint8_t Ir_NeoclimaAc::getFan(void) const {
    return _.Fan;
}



/// Set the Sleep setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void Ir_NeoclimaAc::setSleep(const bool on) {
    _.Button = kNeoclimaButtonSleep;
    _.Sleep = on;
}

/// Get the Sleep setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool Ir_NeoclimaAc::getSleep(void) const {
    return _.Sleep;
}

/// Set the vertical swing setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void Ir_NeoclimaAc::setSwingV(const bool on) {
    _.Button = kNeoclimaButtonSwing;
    _.SwingV = (on ? kNeoclimaSwingVOn : kNeoclimaSwingVOff);
}

/// Get the vertical swing setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool Ir_NeoclimaAc::getSwingV(void) const {
    return _.SwingV == kNeoclimaSwingVOn;
}

/// Set the horizontal swing setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void Ir_NeoclimaAc::setSwingH(const bool on) {
    _.Button = kNeoclimaButtonAirFlow;
    _.SwingH = !on;  // Cleared when `on`
}

/// Get the horizontal swing (Air Flow) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool Ir_NeoclimaAc::getSwingH(void) const {
    return !_.SwingH;
}

/// Set the Turbo setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void Ir_NeoclimaAc::setTurbo(const bool on) {
    _.Button = kNeoclimaButtonTurbo;
    _.Turbo = on;
}

/// Get the Turbo setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool Ir_NeoclimaAc::getTurbo(void) const {
    return _.Turbo;
}

/// Set the Economy (Energy Saver) setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void Ir_NeoclimaAc::setEcono(const bool on) {
    _.Button = kNeoclimaButtonEcono;
    _.Econo = on;
}

/// Get the Economy (Energy Saver) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool Ir_NeoclimaAc::getEcono(void) const {
    return _.Econo;
}

/// Set the Fresh (air) setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void Ir_NeoclimaAc::setFresh(const bool on) {
    _.Button = kNeoclimaButtonFresh;
    _.Fresh = on;
}

/// Get the Fresh (air) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool Ir_NeoclimaAc::getFresh(void) const {
    return _.Fresh;
}

/// Set the Hold setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void Ir_NeoclimaAc::setHold(const bool on) {
    _.Button = kNeoclimaButtonHold;
    _.Hold = on;
}

/// Get the Hold setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool Ir_NeoclimaAc::getHold(void) const {
    return _.Hold;
}

/// Set the Ion (filter) setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void Ir_NeoclimaAc::setIon(const bool on) {
    _.Button = kNeoclimaButtonIon;
    _.Ion = on;
}

/// Get the Ion (filter) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool Ir_NeoclimaAc::getIon(void) const {
    return _.Ion;
}

/// Set the Light(LED display) setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void Ir_NeoclimaAc::setLight(const bool on) {
    _.Button = kNeoclimaButtonLight;
    _.Light = on;
}

/// Get the Light (LED display) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool Ir_NeoclimaAc::getLight(void) const {
    return _.Light;
}

/// Set the 8°C Heat setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
/// @note This feature maintains the room temperature steadily at 8°C and
///   prevents the room from freezing by activating the heating operation
///   automatically when nobody is at home over a longer period during severe
///   winter.
void Ir_NeoclimaAc::set8CHeat(const bool on) {
    _.Button = kNeoclimaButton8CHeat;
    _.CHeat = on;
}

/// Get the 8°C Heat setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool Ir_NeoclimaAc::get8CHeat(void) const {
    return _.CHeat;
}

/// Set the Eye (Sensor) setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void Ir_NeoclimaAc::setEye(const bool on) {
    _.Button = kNeoclimaButtonEye;
    _.Eye = on;
}

/// Get the Eye (Sensor) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool Ir_NeoclimaAc::getEye(void) const {
    return _.Eye;
}

/// Is the A/C unit using Fahrenheit or Celsius for temperature units.
/// @return false, Fahrenheit. true, Celsius.
bool Ir_NeoclimaAc::getTempUnits(void) const {
    return !_.UseFah;
}

/* DISABLED
   TODO(someone): Work out why "on" is either 0x5D or 0x5F
void Ir_NeoclimaAc::setFollow(const bool on) {
  setButton(kNeoclimaButtonFollow);
  if (on)
    remote_state[8] = kNeoclimaFollowMe;
  else
    remote_state[8] = 0;
}
*/

/// Get the Follow Me setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool Ir_NeoclimaAc::getFollow(void) const {
    return (_.Follow & kNeoclimaFollowMe) == kNeoclimaFollowMe;
}

/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/


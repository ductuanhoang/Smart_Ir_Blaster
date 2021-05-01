/*
 * IrHitachi.cpp
 *
 *  Created on: Mar 6, 2021
 *      Author: ductu
 */



/***********************************************************************************************************************
 * Pragma directive
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes <System Includes>
 ***********************************************************************************************************************/
#include "IrHitachi.h"
#include "IRutils.h"

/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables and functions
 ***********************************************************************************************************************/
// Constants
const uint16_t kHitachiAcHdrMark = 3300;
const uint16_t kHitachiAcHdrSpace = 1700;
const uint16_t kHitachiAc1HdrMark = 3400;
const uint16_t kHitachiAc1HdrSpace = 3400;
const uint16_t kHitachiAcBitMark = 400;
const uint16_t kHitachiAcOneSpace = 1250;
const uint16_t kHitachiAcZeroSpace = 500;
const uint32_t kHitachiAcMinGap = kDefaultMessageGap;  // Just a guess.
// Support for HitachiAc424 protocol
const uint16_t kHitachiAc424LdrMark = 29784;   // Leader
const uint16_t kHitachiAc424LdrSpace = 49290;  // Leader
const uint16_t kHitachiAc424HdrMark = 3416;    // Header
const uint16_t kHitachiAc424HdrSpace = 1604;   // Header
const uint16_t kHitachiAc424BitMark = 463;
const uint16_t kHitachiAc424OneSpace = 1208;
const uint16_t kHitachiAc424ZeroSpace = 372;

// Support for HitachiAc3 protocol
const uint16_t kHitachiAc3HdrMark = 3400;    // Header
const uint16_t kHitachiAc3HdrSpace = 1660;   // Header
const uint16_t kHitachiAc3BitMark = 460;
const uint16_t kHitachiAc3OneSpace = 1250;
const uint16_t kHitachiAc3ZeroSpace = 410;

using irutils::checkInvertedBytePairs;
using irutils::invertBytePairs;

using std::max;
//using std::min;
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

IRHitachiAc::IRHitachiAc(uint32_t frequency, uint16_t dutycycle):_ir_hitachiAC(frequency, dutycycle)
{
	stateReset();
}

IRHitachiAc::~IRHitachiAc() {
	// TODO Auto-generated destructor stub
}

void IRHitachiAc::sendHitachiAC(const uint16_t repeat)
{
	send(getRaw(), kHitachiAcStateLength, repeat);
}
/// Send a Hitachi 28-byte/224-bit A/C formatted message. (HITACHI_AC)
/// Status: STABLE / Working.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/417
void IRHitachiAc::send(const unsigned char data[], const uint16_t nbytes,
                                const uint16_t repeat) {
    if (nbytes < kHitachiAcStateLength)
        return;  // Not enough bytes to send a proper message.

    const bool MSBfirst = (nbytes == kHitachiAc344StateLength) ? false : true;
    _ir_hitachiAC.sendGeneric(kHitachiAcHdrMark, kHitachiAcHdrSpace, kHitachiAcBitMark,
                kHitachiAcOneSpace, kHitachiAcBitMark, kHitachiAcZeroSpace,
                kHitachiAcBitMark, kHitachiAcMinGap, data, nbytes, 38, MSBfirst,
                repeat, 50);
}




/// Reset the internal state to a fixed known good state.
void IRHitachiAc::stateReset(void) {
    _.raw[0] = 0x80;
    _.raw[1] = 0x08;
    _.raw[2] = 0x0C;
    _.raw[3] = 0x02;
    _.raw[4] = 0xFD;
    _.raw[5] = 0x80;
    _.raw[6] = 0x7F;
    _.raw[7] = 0x88;
    _.raw[8] = 0x48;
    _.raw[9] = 0x10;
    for (uint8_t i = 10; i < kHitachiAcStateLength; i++) _.raw[i] = 0x00;
    _.raw[14] = 0x60;
    _.raw[15] = 0x60;
    _.raw[24] = 0x80;
    setTemp(23);
}

/// Calculate the checksum for a given state.
/// @param[in] state The value to calc the checksum of.
/// @param[in] length The size/length of the state.
/// @return The calculated checksum value.
uint8_t IRHitachiAc::calcChecksum(const uint8_t state[],
                                  const uint16_t length) {
    uint8_t sum = 62;
    for (uint16_t i = 0; i < length - 1; i++) sum -= reverseBits(state[i], 8);
    return reverseBits(sum, 8);
}

/// Calculate and set the checksum values for the internal state.
/// @param[in] length The size/length of the state.
void IRHitachiAc::checksum(const uint16_t length) {
    _.Sum = calcChecksum(_.raw, length);
}

/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @param[in] length The length of the state array.
/// @return true, if the state has a valid checksum. Otherwise, false.
bool IRHitachiAc::validChecksum(const uint8_t state[], const uint16_t length) {
    if (length < 2) return true;  // Assume true for lengths that are too short.
    return (state[length - 1] == calcChecksum(state, length));
}

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t *IRHitachiAc::getRaw(void) {
    checksum();
    return _.raw;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] new_code A valid code for this protocol.
/// @param[in] length The length of the new_code array.
void IRHitachiAc::setRaw(const uint8_t new_code[], const uint16_t length) {
	if( length < kHitachiAcStateLength)
	{
		memcpy(_.raw, new_code, length);
	}
	else
	{
		memcpy(_.raw, new_code, kHitachiAcStateLength);
	}
}

#if SEND_HITACHI_AC
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRHitachiAc::send(const uint16_t repeat) {
  _irsend.sendHitachiAC(getRaw(), kHitachiAcStateLength, repeat);
}
#endif  // SEND_HITACHI_AC

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRHitachiAc::getPower(void) const {
    return _.Power;
}

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRHitachiAc::setPower(const bool on) {
    _.Power = on;
}

/// Change the power setting to On.
void IRHitachiAc::on(void) { setPower(true); }

/// Change the power setting to Off.
void IRHitachiAc::off(void) { setPower(false); }

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRHitachiAc::getMode(void) const { return reverseBits(_.Mode, 8); }

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRHitachiAc::setMode(const uint8_t mode) {
    uint8_t newmode = mode;
    switch (mode) {
        // Fan mode sets a special temp.
        case kHitachiAcFan: setTemp(64); break;
        case kHitachiAcAuto:
        case kHitachiAcHeat:
        case kHitachiAcCool:
        case kHitachiAcDry: break;
        default: newmode = kHitachiAcAuto;
    }
    _.Mode = reverseBits(newmode, 8);
    if (mode != kHitachiAcFan) setTemp(_previoustemp);
    setFan(getFan());  // Reset the fan speed after the mode change.
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRHitachiAc::getTemp(void) const {
    return reverseBits(_.Temp, 8) >> 1;
}

/// Set the temperature.
/// @param[in] celsius The temperature in degrees celsius.
void IRHitachiAc::setTemp(const uint8_t celsius) {
    uint8_t temp;
    if (celsius != 64) _previoustemp = celsius;
    switch (celsius) {
        case 64:
            temp = celsius;
            break;
        default:
            temp = std::min(celsius, kHitachiAcMaxTemp);
            temp = std::max(temp, kHitachiAcMinTemp);
    }
    _.Temp = reverseBits(temp << 1, 8);
    if (temp == kHitachiAcMinTemp)
        _.raw[9] = 0x90;
    else
        _.raw[9] = 0x10;
}

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t IRHitachiAc::getFan(void) const { return reverseBits(_.Fan, 8); }

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
void IRHitachiAc::setFan(const uint8_t speed) {
    uint8_t fanmin = kHitachiAcFanAuto;
    uint8_t fanmax = kHitachiAcFanHigh;
    switch (getMode()) {
        case kHitachiAcDry:  // Only 2 x low speeds in Dry mode.
            fanmin = kHitachiAcFanLow;
            fanmax = kHitachiAcFanLow + 1;
            break;
        case kHitachiAcFan:
            fanmin = kHitachiAcFanLow;  // No Auto in Fan mode.
            break;
    }
    uint8_t newspeed = std::max(speed, fanmin);
    newspeed = std::min(newspeed, fanmax);
    _.Fan = reverseBits(newspeed, 8);
}

/// Get the Vertical Swing setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRHitachiAc::getSwingVertical(void) const {
    return _.SwingV;
}

/// Set the Vertical Swing setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRHitachiAc::setSwingVertical(const bool on) {
    _.SwingV = on;
}

/// Get the Horizontal Swing setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRHitachiAc::getSwingHorizontal(void) const {
    return _.SwingH;
}

/// Set the Horizontal Swing setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRHitachiAc::setSwingHorizontal(const bool on) {
    _.SwingH = on;
}

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRHitachiAc1::IRHitachiAc1(uint32_t frequency, uint16_t dutycycle):_ir_hitachiAC1(frequency, dutycycle)
{
	stateReset();
}

IRHitachiAc1::~IRHitachiAc1()
{

}

void IRHitachiAc1::setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp) {
    setPower(power);
    setMode(convertMode(stdAc::opmode_t(mode)));
    setFan(convertFan(stdAc::fanspeed_t(speed)));
    setTemp(temp);
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRHitachiAc1::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kHitachiAc1Cool;
    case stdAc::opmode_t::kHeat: return kHitachiAc1Heat;
    case stdAc::opmode_t::kDry:  return kHitachiAc1Dry;
    case stdAc::opmode_t::kFan:  return kHitachiAc1Fan;
    default:                     return kHitachiAc1Auto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRHitachiAc1::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:    return kHitachiAc1FanLow;
    case stdAc::fanspeed_t::kMedium: return kHitachiAc1FanMed;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:    return kHitachiAc1FanHigh;
    default:                         return kHitachiAc1FanAuto;
  }
}

void IRHitachiAc1::send(const unsigned char *data, const uint16_t nbytes, const uint16_t repeat) {
    if (nbytes < kHitachiAc1StateLength)
        return;  // Not enough bytes to send a proper message.
    _ir_hitachiAC1.sendGeneric(kHitachiAc1HdrMark, kHitachiAc1HdrSpace, kHitachiAcBitMark,
                kHitachiAcOneSpace, kHitachiAcBitMark, kHitachiAcZeroSpace,
                kHitachiAcBitMark, kHitachiAcMinGap, data, nbytes, kHitachiAcFreq,
                true, repeat, 50);
}
/// Reset the internal state to a fixed known good state.
void IRHitachiAc1::stateReset(void) {
    for (uint8_t i = 0; i < kHitachiAc1StateLength; i++) _.raw[i] = 0x00;
    // Copy in a known good state.
    _.raw[0] = 0xB2;
    _.raw[1] = 0xAE;
    _.raw[2] = 0x4D;
    _.raw[3] = 0x91;
    _.raw[4] = 0xF0;
    _.raw[5] = 0xE1;
    _.raw[6] = 0xA4;
    _.raw[11] = 0x61;
    _.raw[12] = 0x24;
}

/// Calculate the checksum for a given state.
/// @param[in] state The value to calc the checksum of.
/// @param[in] length The size/length of the state.
/// @return The calculated checksum value.
uint8_t IRHitachiAc1::calcChecksum(const uint8_t state[],
                                   const uint16_t length) {
    uint8_t sum = 0;
    for (uint16_t i = kHitachiAc1ChecksumStartByte; i < length - 1; i++) {
        sum += reverseBits(GETBITS8(state[i], kLowNibble, kNibbleSize),
                           kNibbleSize);
        sum += reverseBits(GETBITS8(state[i], kHighNibble, kNibbleSize),
                           kNibbleSize);
    }
    return reverseBits(sum, 8);
}

/// Calculate and set the checksum values for the internal state.
/// @param[in] length The size/length of the state.
void IRHitachiAc1::checksum(const uint16_t length) {
    _.Sum = calcChecksum(_.raw, length);
}

/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @param[in] length The length of the state array.
/// @return true, if the state has a valid checksum. Otherwise, false.
bool IRHitachiAc1::validChecksum(const uint8_t state[], const uint16_t length) {
    if (length < 2) return true;  // Assume true for lengths that are too short.
    return (state[length - 1] == calcChecksum(state, length));
}

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t *IRHitachiAc1::getRaw(void) {
    checksum();
    return _.raw;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] new_code A valid code for this protocol.
/// @param[in] length The length of the new_code array.
void IRHitachiAc1::setRaw(const uint8_t new_code[], const uint16_t length) {
    if( length < kHitachiAc1StateLength)
    	memcpy(_.raw, new_code, length);
    else
    	memcpy(_.raw, new_code, kHitachiAc1StateLength);
}


/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRHitachiAc1::sendHitachiAc1(const uint16_t repeat) {
  send(getRaw(), kHitachiAc1StateLength, repeat);
  // Clear the toggle bits as we have actioned them by sending them.
  setPowerToggle(false);
  setSwingToggle(false);
}


/// Get/Detect the model of the A/C.
/// @return The enum of the compatible model.
hitachi_ac1_remote_model_t IRHitachiAc1::getModel(void) const {
    switch (_.Model) {
        case kHitachiAc1Model_B: return hitachi_ac1_remote_model_t::R_LT0541_HTA_B;
        default:                 return hitachi_ac1_remote_model_t::R_LT0541_HTA_A;
    }
}

/// Set the model of the A/C to emulate.
/// @param[in] model The enum of the appropriate model.
void IRHitachiAc1::setModel(const hitachi_ac1_remote_model_t model) {
    uint8_t value = 0;
    switch (model) {
        case hitachi_ac1_remote_model_t::R_LT0541_HTA_B:
            value = kHitachiAc1Model_B;
            break;
        default:
            value = kHitachiAc1Model_A;  // i.e. 'A' mode.
    }
    _.Model = value;
}

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRHitachiAc1::getPower(void) const {
    return _.Power;
}

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRHitachiAc1::setPower(const bool on) {
    // If the power changes, set the power toggle bit.
    if (on != _.Power) setPowerToggle(true);
    _.Power = on;
}

/// Get the value of the current power toggle setting.
/// @return true, the setting is on. false, the setting is off.
bool IRHitachiAc1::getPowerToggle(void) const {
    return _.PowerToggle;
}

/// Change the power toggle setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRHitachiAc1::setPowerToggle(const bool on) {
    _.PowerToggle = on;
}

/// Change the power setting to On.
void IRHitachiAc1::on(void) { setPower(true); }

/// Change the power setting to Off.
void IRHitachiAc1::off(void) { setPower(false); }

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRHitachiAc1::getMode(void) const {
    return _.Mode;
}

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRHitachiAc1::setMode(const uint8_t mode) {
    switch (mode) {
        case kHitachiAc1Auto:
            setTemp(kHitachiAc1TempAuto);
            // FALL THRU
            break;
        case kHitachiAc1Fan:
        case kHitachiAc1Heat:
        case kHitachiAc1Cool:
        case kHitachiAc1Dry:
            _.Mode = mode;
            break;
        default:
            setTemp(kHitachiAc1TempAuto);
            _.Mode = kHitachiAc1Auto;
            break;
    }
    setSleep(_.Sleep);  // Correct the sleep mode if required.
    setFan(_.Fan);  // Correct the fan speed if required.
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRHitachiAc1::getTemp(void) const {
    return reverseBits(_.Temp, kHitachiAc1TempSize) + kHitachiAc1TempDelta;
}

/// Set the temperature.
/// @param[in] celsius The temperature in degrees celsius.
void IRHitachiAc1::setTemp(const uint8_t celsius) {
    if (_.Mode == kHitachiAc1Auto) return;  // Can't change temp in Auto mode.
    uint8_t temp = std::min(celsius, kHitachiAcMaxTemp);
    temp = std::max(temp, kHitachiAcMinTemp);
    temp -= kHitachiAc1TempDelta;
    temp = reverseBits(temp, kHitachiAc1TempSize);
    _.Temp = temp;
}

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t IRHitachiAc1::getFan(void) const {
    return _.Fan;
}

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
/// @param[in] force Deprecated
void IRHitachiAc1::setFan(const uint8_t speed, const bool /*force*/) {
    // restrictions
    switch (_.Mode) {
        case kHitachiAc1Dry:
            _.Fan = kHitachiAc1FanLow;  // Dry is locked to Low speed.
            return;
        case kHitachiAc1Auto:
            _.Fan = kHitachiAc1FanAuto;  // Auto is locked to Auto speed.
            return;
        case kHitachiAc1Heat:
        case kHitachiAc1Fan:  // Auto speed not allowed in these modes.
            if (speed == kHitachiAc1FanAuto || _.Fan == kHitachiAc1FanAuto)
                _.Fan = kHitachiAc1FanLow;
            return;
    }

    switch (speed) {
        case kHitachiAc1FanAuto:
        case kHitachiAc1FanHigh:
        case kHitachiAc1FanMed:
        case kHitachiAc1FanLow:
            _.Fan = speed;
            break;
        default: _.Fan = kHitachiAc1FanAuto;
    }
}

/// Get the Swing Toggle setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRHitachiAc1::getSwingToggle(void) const {
    return _.SwingToggle;
}

/// Set the Swing toggle setting of the A/C.
/// @param[in] toggle true, the setting is on. false, the setting is off.
void IRHitachiAc1::setSwingToggle(const bool toggle) {
    _.SwingToggle = toggle;
}

/// Get the Vertical Swing setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRHitachiAc1::getSwingV(void) const {
    return _.SwingV;
}

/// Set the Vertical Swing setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRHitachiAc1::setSwingV(const bool on) {
    _.SwingV = on;
}

/// Get the Horizontal Swing setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRHitachiAc1::getSwingH(void) const {
    return _.SwingH;
}

/// Set the Horizontal Swing setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRHitachiAc1::setSwingH(const bool on) {
    _.SwingH = on;
}

/// Get the Sleep setting of the A/C.
/// @return The currently configured sleep mode.
/// @note Sleep modes only available in Auto & Cool modes, otherwise it's off.
uint8_t IRHitachiAc1::getSleep(void) const {
    return _.Sleep;
}

/// Set the Sleep setting of the A/C.
/// @param[in] mode The mode of sleep to set the A/C to.
/// @note Sleep modes only available in Auto & Cool modes, otherwise it's off.
void IRHitachiAc1::setSleep(const uint8_t mode) {
    switch (_.Mode) {
        case kHitachiAc1Auto:
        case kHitachiAc1Cool:
            _.Sleep = std::min(mode, kHitachiAc1Sleep4);
            break;
        default:
            _.Sleep = kHitachiAc1SleepOff;
    }
}

/// Set the On Timer time.
/// @param[in] mins The time expressed in total number of minutes.
void IRHitachiAc1::setOnTimer(const uint16_t mins) {
    const uint16_t mins_lsb = reverseBits(mins, kHitachiAc1TimerSize);
    _.OnTimerLow = GETBITS16(mins_lsb, 8, 8);
    _.OnTimerHigh = GETBITS16(mins_lsb, 0, 8);
}

/// Get the On Timer vtime of the A/C.
/// @return Nr of minutes the timer is set to.
uint16_t IRHitachiAc1::getOnTimer(void) const {
    return reverseBits(
            (_.OnTimerLow << 8) | _.OnTimerHigh, kHitachiAc1TimerSize);
}

/// Set the Off Timer time.
/// @param[in] mins The time expressed in total number of minutes.
void IRHitachiAc1::setOffTimer(const uint16_t mins) {
    const uint16_t mins_lsb = reverseBits(mins, kHitachiAc1TimerSize);
    _.OffTimerLow = GETBITS16(mins_lsb, 8, 8);
    _.OffTimerHigh = GETBITS16(mins_lsb, 0, 8);
}

/// Get the Off Timer vtime of the A/C.
/// @return Nr of minutes the timer is set to.
uint16_t IRHitachiAc1::getOffTimer(void) const {
    return reverseBits(
            (_.OffTimerLow << 8) | _.OffTimerHigh, kHitachiAc1TimerSize);
}


/// Send a Hitachi 53-byte/424-bit A/C formatted message. (HITACHI_AC424)
/// Status: STABLE / Reported as working.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @note This protocol is almost exactly the same as HitachiAC2 except this
///   variant has a leader section as well, and subtle timing differences.
///   It is also in LSBF order (per byte), rather than MSBF order.
void IRHitachiAc424::send(const uint8_t data[], const uint16_t nbytes,
                              const uint16_t repeat) {
  for (uint16_t r = 0; r <= repeat; r++) {
    // Leader
	  _ir_hitachiAC424.mark_space(kHitachiAc424LdrMark, kHitachiAc424LdrSpace);
    // Header + Data + Footer
	  _ir_hitachiAC424.sendGeneric(kHitachiAc424HdrMark, kHitachiAc424HdrSpace,
                kHitachiAc424BitMark, kHitachiAc424OneSpace,
                kHitachiAc424BitMark, kHitachiAc424ZeroSpace,
                kHitachiAc424BitMark, kHitachiAcMinGap,
                data, nbytes,  // Bytes
                kHitachiAcFreq, false, kNoRepeat, 50);
  }
}


/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRHitachiAc424::IRHitachiAc424(uint32_t frequency, uint16_t dutycycle):_ir_hitachiAC424(frequency, dutycycle)
{
	stateReset();
}

IRHitachiAc424::~IRHitachiAc424()
{

}

void IRHitachiAc424::sendHitachi_Ac424(const uint16_t repeat) {
	send(getRaw(), kHitachiAc424StateLength, repeat);
}
/// Reset the internal state to a fixed known good state.
/// @note Reset to auto fan, cooling, 23° Celsius
void IRHitachiAc424::stateReset(void) {
    for (uint8_t i = 0; i < kHitachiAc424StateLength; i++)
        _.raw[i] = 0x00;

    _.raw[0]  = 0x01;
    _.raw[1]  = 0x10;
    _.raw[3]  = 0x40;
    _.raw[5]  = 0xFF;
    _.raw[7]  = 0xCC;
    _.raw[33] = 0x80;
    _.raw[35] = 0x03;
    _.raw[37] = 0x01;
    _.raw[39] = 0x88;
    _.raw[45] = 0xFF;
    _.raw[47] = 0xFF;
    _.raw[49] = 0xFF;
    _.raw[51] = 0xFF;

    setTemp(23);
    setPower(true);
    setMode(kHitachiAc424Cool);
    setFan(kHitachiAc424FanAuto);
}

/// Update the internal consistency check for the protocol.
void IRHitachiAc424::setInvertedStates(void) {
    invertBytePairs(_.raw + 3, kHitachiAc424StateLength - 3);
}

/// Set up hardware to be able to send a message.

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t *IRHitachiAc424::getRaw(void) {
    setInvertedStates();
    return _.raw;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] new_code A valid code for this protocol.
/// @param[in] length The length of the new_code array.
void IRHitachiAc424::setRaw(const uint8_t new_code[], const uint16_t length) {
    if (length < kHitachiAc424StateLength)
    	memcpy(_.raw, new_code, length);
    else
    	memcpy(_.raw, new_code, kHitachiAc424StateLength);
}

#if SEND_HITACHI_AC424
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRHitachiAc424::send(const uint16_t repeat) {
  _irsend.sendHitachiAc424(getRaw(), kHitachiAc424StateLength, repeat);
}
#endif  // SEND_HITACHI_AC424

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRHitachiAc424::getPower(void) const {
    return _.Power == kHitachiAc424PowerOn;
}

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRHitachiAc424::setPower(const bool on) {
    setButton(kHitachiAc424ButtonPowerMode);
    _.Power = (on ? kHitachiAc424PowerOn : kHitachiAc424PowerOff);
}

/// Change the power setting to On.
void IRHitachiAc424::on(void) { setPower(true); }

/// Change the power setting to Off.
void IRHitachiAc424::off(void) { setPower(false); }

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRHitachiAc424::getMode(void) const {
    return _.Mode;
}

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRHitachiAc424::setMode(const uint8_t mode) {
    uint8_t newMode = mode;
    switch (mode) {
        // Fan mode sets a special temp.
        case kHitachiAc424Fan: setTemp(kHitachiAc424FanTemp, false); break;
        case kHitachiAc424Heat:
        case kHitachiAc424Cool:
        case kHitachiAc424Dry: break;
        default: newMode = kHitachiAc424Cool;
    }
    _.Mode = newMode;
    if (newMode != kHitachiAc424Fan) setTemp(_previoustemp);
    setFan(_.Fan);  // Reset the fan speed after the mode change.
    setButton(kHitachiAc424ButtonPowerMode);
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRHitachiAc424::getTemp(void) const {
    return _.Temp;
}

/// Set the temperature.
/// @param[in] celsius The temperature in degrees celsius.
/// @param[in] setPrevious true, remember this if we change mode. false, don't.
void IRHitachiAc424::setTemp(const uint8_t celsius, bool setPrevious) {
    uint8_t temp;
    temp = std::min(celsius, kHitachiAc424MaxTemp);
    temp = std::max(temp, kHitachiAc424MinTemp);
    _.Temp = temp;
    if (_previoustemp > temp)
        setButton(kHitachiAc424ButtonTempDown);
    else if (_previoustemp < temp)
        setButton(kHitachiAc424ButtonTempUp);
    if (setPrevious) _previoustemp = temp;
}

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t IRHitachiAc424::getFan(void) const {
    return _.Fan;
}

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
void IRHitachiAc424::setFan(const uint8_t speed) {
    uint8_t newSpeed = std::max(speed, kHitachiAc424FanMin);
    uint8_t fanMax = kHitachiAc424FanMax;

    // Only 2 x low speeds in Dry mode or Auto
    if (_.Mode == kHitachiAc424Dry && speed == kHitachiAc424FanAuto) {
        fanMax = kHitachiAc424FanAuto;
    } else if (_.Mode == kHitachiAc424Dry) {
        fanMax = kHitachiAc424FanMaxDry;
    } else if (_.Mode == kHitachiAc424Fan && speed == kHitachiAc424FanAuto) {
        // Fan Mode does not have auto. Set to safe low
        newSpeed = kHitachiAc424FanMin;
    }

    newSpeed = std::min(newSpeed, fanMax);
    // Handle the setting the button value if we are going to change the value.
    if (newSpeed != _.Fan) setButton(kHitachiAc424ButtonFan);
    // Set the values
    _.Fan = newSpeed;
    _.raw[9] = 0x92;
    _.raw[29] = 0x00;

    // When fan is at min/max, additional bytes seem to be set
    if (newSpeed == kHitachiAc424FanMin) _.raw[9] = 0x98;
    if (newSpeed == kHitachiAc424FanMax) {
        _.raw[9] = 0xA9;
        _.raw[29] = 0x30;
    }
}

/// Get the Button/Command setting of the A/C.
/// @return The value of the button/command that was pressed.
uint8_t IRHitachiAc424::getButton(void) const {
    return _.Button;
}

/// Set the Button/Command pressed setting of the A/C.
/// @param[in] button The value of the button/command that was pressed.
void IRHitachiAc424::setButton(const uint8_t button) {
    _.Button = button;
}

/// Set the Vertical Swing toggle setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
/// @note The remote does not keep state of the vertical swing.
///   A byte is sent indicating the swing button is pressed on the remote
void IRHitachiAc424::setSwingVToggle(const bool on) {
    uint8_t button = _.Button;  // Get the current button value.
    if (on)
        button = kHitachiAc424ButtonSwingV;  // Set the button to SwingV.
    else if (button == kHitachiAc424ButtonSwingV)  // Asked to unset it
        // It was set previous, so use Power as a default
        button = kHitachiAc424ButtonPowerMode;
    setButton(button);
}

/// Get the Vertical Swing toggle setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRHitachiAc424::getSwingVToggle(void) const {
    return _.Button == kHitachiAc424ButtonSwingV;
}


/// Send a Hitachi(3) A/C formatted message. (HITACHI_AC3)
/// Status: STABLE / Working fine.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @note This protocol is almost exactly the same as HitachiAC424 except this
///   variant has subtle timing differences. There are five(5) typical sizes:
///     kHitachiAc3MinStateLength (Cancel Timer),
///     kHitachiAc3MinStateLength + 2 (Change Temp),
///     kHitachiAc3StateLength - 6 (Change Mode),
///     kHitachiAc3StateLength - 4 (Normal), &
///     kHitachiAc3StateLength (Set Timer)
void IRHitachiAc3::send(const uint8_t data[], const uint16_t nbytes,
                              const uint16_t repeat) {
  // Header + Data + Footer
	_ir_hitachiAC3.sendGeneric(kHitachiAc3HdrMark, kHitachiAc3HdrSpace,
              kHitachiAc3BitMark, kHitachiAc3OneSpace,
              kHitachiAc3BitMark, kHitachiAc3ZeroSpace,
              kHitachiAc3BitMark, kHitachiAcMinGap,
              data, nbytes,  // Bytes
              kHitachiAcFreq, false, repeat, 50);
}



/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?

IRHitachiAc3::IRHitachiAc3(uint32_t frequency, uint16_t dutycycle):_ir_hitachiAC3(frequency, dutycycle)
{
	stateReset();
}

IRHitachiAc3::~IRHitachiAc3()
{

}

void IRHitachiAc3::sendHitachiAc3(const uint16_t repeat)
{

}
/// Reset the internal state to a fixed known good state.
/// @note Reset to auto fan, cooling, 23° Celsius
void IRHitachiAc3::stateReset(void) {
    for (uint8_t i = 0; i < kHitachiAc3StateLength; i++)
        remote_state[i] = 0x00;
    remote_state[0]  = 0x01;
    remote_state[1]  = 0x10;
    remote_state[3]  = 0x40;
    remote_state[5]  = 0xFF;
    remote_state[7]  = 0xE8;
    remote_state[9]  = 0x89;
    remote_state[11] = 0x0B;
    remote_state[13] = 0x3F;
    remote_state[15] = 0x15;
    remote_state[21] = 0x4B;
    remote_state[23] = 0x18;
    setInvertedStates();
}

/// Invert every second byte of the internal state, after the fixed header.
/// @param[in] length The size of the state array.
/// @note This is this protocols integrity check.
void IRHitachiAc3::setInvertedStates(const uint16_t length) {
    if (length > 3) invertBytePairs(remote_state + 3, length - 3);
}

/// Check if every second byte of the state, after the fixed header
///   is inverted to the previous byte.
/// @param[in] state The state array to be checked.
/// @param[in] length The size of the state array.
/// @note This is this protocols integrity check.
bool IRHitachiAc3::hasInvertedStates(const uint8_t state[],
                                     const uint16_t length) {
    return (length <= 3 || checkInvertedBytePairs(state + 3, length - 3));
}

/// Set up hardware to be able to send a message.

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t *IRHitachiAc3::getRaw(void) {
    setInvertedStates();
    return remote_state;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] new_code A valid code for this protocol.
/// @param[in] length The length of the new_code array.
void IRHitachiAc3::setRaw(const uint8_t new_code[], const uint16_t length) {
    if (length < kHitachiAc3StateLength)
    	memcpy(remote_state, new_code, length);
    else
    	memcpy(remote_state, new_code, kHitachiAc3StateLength);
}


/// Class constructor for handling detailed Hitachi_AC344 43 byte A/C messages.
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?

//IRHitachiAc344::IRHitachiAc344(uint32_t frequency, uint16_t dutycycle)
//{
//	stateReset();
//}

IRHitachiAc344::~IRHitachiAc344(){

}

void IRHitachiAc344::sendHitachiAc344(const uint16_t repeat) {
	send(getRaw(), kHitachiAc344StateLength, repeat);
}
/// Reset the internal state to auto fan, cooling, 23° Celsius
void IRHitachiAc344::stateReset(void) {
    IRHitachiAc424::stateReset();
    _.raw[37] = 0x00;
    _.raw[39] = 0x00;
}

/// Create and send the IR message to the A/C.
/// @param[in] repeat Nr. of times to repeat the message.
void IRHitachiAc344::send(const unsigned char *data, const uint16_t nbytes, const uint16_t repeat) {
	IRHitachiAc _ir_remote_hitachiAc(38000, 50);
	  if (nbytes < kHitachiAc344StateLength)
	    return;  // Not enough bytes to send a proper message.
	  _ir_remote_hitachiAc.send(data, nbytes, repeat);

}
/// Set the internal state from a valid code for this protocol.
/// @param[in] new_code A valid code for this protocol.
/// @param[in] length Size (in bytes) of the code for this protocol.
void IRHitachiAc344::setRaw(const uint8_t new_code[], const uint16_t length) {
    if (length < kHitachiAc344StateLength)
    	 memcpy(_.raw, new_code, length);
    else
    	memcpy(_.raw, new_code, kHitachiAc344StateLength);
}

/// Control the vertical swing setting.
/// @param[in] on True, turns on the feature. False, turns off the feature.
void IRHitachiAc344::setSwingV(const bool on) {
    setSwingVToggle(on);  // Set the button value.
    _.SwingV = on;
}

/// Get the current vertical swing setting.
/// @return True, if the setting is on. False, it is off.
bool IRHitachiAc344::getSwingV(void) const {
    return _.SwingV;
}

/// Control the horizontal swing setting.
/// @param[in] position The position to set the horizontal swing to.
void IRHitachiAc344::setSwingH(const uint8_t position) {
    if (position > kHitachiAc344SwingHLeftMax)
        _.SwingH = kHitachiAc344SwingHMiddle;
    else
        _.SwingH = position;
    setButton(kHitachiAc344ButtonSwingH);
}

/// Get the current horizontal swing setting.
/// @return The current position horizontal swing is set to.
uint8_t IRHitachiAc344::getSwingH(void) const {
    return _.SwingH;
}



IRHitachiAc2::IRHitachiAc2() {

}

IRHitachiAc2::~IRHitachiAc2() {

}

/// Send a Hitachi 53 byte/424-bit A/C formatted message. (HITACHI_AC2)
///   Basically the same as sendHitatchiAC() except different size.
/// Status: STABLE / Expected to work.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRHitachiAc2::sendHitachiAc2(const unsigned char data[], const uint16_t nbytes,
                            const uint16_t repeat) {
    IRHitachiAc _ir_remote_hitachiAc(38000, 50);
  if (nbytes < kHitachiAc2StateLength)
    return;  // Not enough bytes to send a proper message.
    _ir_remote_hitachiAc.send(data, nbytes, repeat);
}


/***********************************************************************************************************************
 * Static Functions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/


/*
 * IrSamsung.cpp
 *
 *  Created on: Jan 12, 2021
 *      Author: ductu
 */
/***********************************************************************************************************************
 * Pragma directive
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes <System Includes>
 ***********************************************************************************************************************/
#include "IrSamsung.h"
#include "IRutils.h"
/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/
const uint16_t kSamsungTick = 560;
const uint16_t kSamsungHdrMarkTicks = 8;
const uint16_t kSamsungHdrMark = kSamsungHdrMarkTicks * kSamsungTick;
const uint16_t kSamsungHdrSpaceTicks = 8;
const uint16_t kSamsungHdrSpace = kSamsungHdrSpaceTicks * kSamsungTick;
const uint16_t kSamsungBitMarkTicks = 1;
const uint16_t kSamsungBitMark = kSamsungBitMarkTicks * kSamsungTick;
const uint16_t kSamsungOneSpaceTicks = 3;
const uint16_t kSamsungOneSpace = kSamsungOneSpaceTicks * kSamsungTick;
const uint16_t kSamsungZeroSpaceTicks = 1;
const uint16_t kSamsungZeroSpace = kSamsungZeroSpaceTicks * kSamsungTick;
const uint16_t kSamsungRptSpaceTicks = 4;
const uint16_t kSamsungRptSpace = kSamsungRptSpaceTicks * kSamsungTick;
const uint16_t kSamsungMinMessageLengthTicks = 193;
const uint32_t kSamsungMinMessageLength =
    kSamsungMinMessageLengthTicks * kSamsungTick;
const uint16_t kSamsungMinGapTicks =
    kSamsungMinMessageLengthTicks -
    (kSamsungHdrMarkTicks + kSamsungHdrSpaceTicks +
     kSamsungBits * (kSamsungBitMarkTicks + kSamsungOneSpaceTicks) +
     kSamsungBitMarkTicks);
const uint32_t kSamsungMinGap = kSamsungMinGapTicks * kSamsungTick;

const uint16_t kSamsungAcHdrMark = 690;
const uint16_t kSamsungAcHdrSpace = 17844;
const uint8_t kSamsungAcSections = 2;
const uint16_t kSamsungAcSectionMark = 3086;
const uint16_t kSamsungAcSectionSpace = 8864;
const uint16_t kSamsungAcSectionGap = 2886;
const uint16_t kSamsungAcBitMark = 586;
const uint16_t kSamsungAcOneSpace = 1432;
const uint16_t kSamsungAcZeroSpace = 436;

// Data from https://github.com/crankyoldgit/IRremoteESP8266/issues/1220
// Values calculated based on the average of ten messages.
const uint16_t kSamsung36HdrMark = 4515;  /// < uSeconds
const uint16_t kSamsung36HdrSpace = 4438; /// < uSeconds
const uint16_t kSamsung36BitMark = 512;   /// < uSeconds
const uint16_t kSamsung36OneSpace = 1468; /// < uSeconds
const uint16_t kSamsung36ZeroSpace = 490; /// < uSeconds

using std::max;
using std::min;
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
Ir_Samsung::Ir_Samsung(uint32_t frequency, uint16_t dutycycle)
						: _ir_samsungAc_send(frequency, dutycycle)
{
    // TODO Auto-generated constructor stub
    stateReset();
}

Ir_Samsung::~Ir_Samsung()
{
    // TODO Auto-generated destructor stub
}

void Ir_Samsung::sendSamsung(const uint64_t data, const uint16_t nbits,
                             const uint16_t repeat)
{
	_ir_samsungAc_send.sendGeneric(kSamsungHdrMark, kSamsungHdrSpace, kSamsungBitMark,
                kSamsungOneSpace, kSamsungBitMark, kSamsungZeroSpace,
                kSamsungBitMark, kSamsungMinGap, kSamsungMinMessageLength, data,
                nbits, 38, true, repeat, 33);
}

void Ir_Samsung::sendSamsung36(const uint64_t data, const uint16_t nbits,
                               const uint16_t repeat)
{
    if (nbits < 16)
        return; // To small to send.
    for (uint16_t r = 0; r <= repeat; r++)
    {
        // Block #1 (16 bits)
    	_ir_samsungAc_send.sendGeneric(kSamsung36HdrMark, kSamsung36HdrSpace,
                    kSamsung36BitMark, kSamsung36OneSpace,
                    kSamsung36BitMark, kSamsung36ZeroSpace,
                    kSamsung36BitMark, kSamsung36HdrSpace,
                    data >> (nbits - 16), 16, 38, true, 0, 50);
        // Block #2 (The rest, typically 20 bits)
    	_ir_samsungAc_send.sendGeneric(0, 0, // No header
                    kSamsung36BitMark, kSamsung36OneSpace,
                    kSamsung36BitMark, kSamsung36ZeroSpace,
                    kSamsung36BitMark, kSamsungMinGap, // Gap is just a guess.
                    // Mask off the rest of the bits.
                    data & ((1ULL << (nbits - 16)) - 1),
                    nbits - 16, 38, true, 0, 50);
    }
}

void Ir_Samsung::sendSamsungAC(const uint16_t repeat, const bool calcchecksum)
{
    if (calcchecksum)
        checksum();
    // Do we need to send a the special power on/off message?
    if (getPower() != _lastsentpowerstate || _forcepower)
    {
        _forcepower = false; // It will now been sent, so clear the flag if set.
        if (getPower())
        {
            sendOn(repeat);
        }
        else
        {
            sendOff(repeat);
            return; // No point sending anything else if we are turning the unit off.
        }
    }
    send(_.raw, kSamsungAcStateLength, repeat);
}
void Ir_Samsung::setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp) {
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
uint8_t Ir_Samsung::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kSamsungAcCool;
    case stdAc::opmode_t::kHeat: return kSamsungAcHeat;
    case stdAc::opmode_t::kDry:  return kSamsungAcDry;
    case stdAc::opmode_t::kFan:  return kSamsungAcFan;
    default:                     return kSamsungAcAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t Ir_Samsung::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:    return kSamsungAcFanLow;
    case stdAc::fanspeed_t::kMedium: return kSamsungAcFanMed;
    case stdAc::fanspeed_t::kHigh:   return kSamsungAcFanHigh;
    case stdAc::fanspeed_t::kMax:    return kSamsungAcFanTurbo;
    default:                         return kSamsungAcFanAuto;
  }
}

void Ir_Samsung::send(const uint8_t data[], const uint16_t nbytes,
                               const uint16_t repeat)
{
    if (nbytes < kSamsungAcStateLength && nbytes % kSamsungAcSectionLength)
        return; // Not an appropriate number of bytes to send a proper message.

    for (uint16_t r = 0; r <= repeat; r++)
    {
        // Header
    	_ir_samsungAc_send.mark_space(kSamsungAcHdrMark, kSamsungAcHdrSpace);
        // Send in 7 byte sections.
        for (uint16_t offset = 0; offset < nbytes;
             offset += kSamsungAcSectionLength)
        {
        	_ir_samsungAc_send.sendGeneric(kSamsungAcSectionMark, kSamsungAcSectionSpace,
                        kSamsungAcBitMark, kSamsungAcOneSpace, kSamsungAcBitMark,
                        kSamsungAcZeroSpace, kSamsungAcBitMark, kSamsungAcSectionGap,
                        data + offset, kSamsungAcSectionLength, // 7 bytes == 56 bits
                        38000, false, 0, 50);                   // Send in LSBF order
        }
        // Complete made up guess at inter-message gap.
        //        space(kDefaultMessageGap - kSamsungAcSectionGap);
        _ir_samsungAc_send.mark_space(kSamsungAcSectionGap, 0x7FFF);
    }
}

/// Send the special extended "On" message as the library can't seem to
/// reproduce this message automatically.
/// @param[in] repeat Nr. of times the message will be repeated.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/604#issuecomment-475020036
void Ir_Samsung::sendOn(const uint16_t repeat)
{
    const uint8_t extended_state[21] = {
        0x02, 0x92, 0x0F, 0x00, 0x00, 0x00, 0xF0,
        0x01, 0xD2, 0x0F, 0x00, 0x00, 0x00, 0x00,
        0x01, 0xE2, 0xFE, 0x71, 0x80, 0x11, 0xF0};
    send(extended_state, kSamsungAcExtendedStateLength, repeat);
    _lastsentpowerstate = true; // On
}
/// Send the special extended "Off" message as the library can't seem to
/// reproduce this message automatically.
/// @param[in] repeat Nr. of times the message will be repeated.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/604#issuecomment-475020036
void Ir_Samsung::sendOff(const uint16_t repeat)
{
    const uint8_t extended_state[21] = {
        0x02, 0xB2, 0x0F, 0x00, 0x00, 0x00, 0xC0,
        0x01, 0xD2, 0x0F, 0x00, 0x00, 0x00, 0x00,
        0x01, 0x02, 0xFF, 0x71, 0x80, 0x11, 0xC0};
    send(extended_state, kSamsungAcExtendedStateLength, repeat);
    _lastsentpowerstate = false; // Off
}


/// Update the checksum for the internal state.
/// @param[in] length The length/size of the internal array to checksum.
void Ir_Samsung::checksum(uint16_t length) {
  if (length < 13) return;
  _.Sum2 = calcChecksum(_.raw, length);
  _.Sum1 = calcChecksum(_.raw, length - 7);
}

/// Calculate the checksum for a given state.
/// @param[in] state The array to calc the checksum of.
/// @param[in] length The length/size of the array.
/// @return The calculated checksum value.
uint8_t Ir_Samsung::calcChecksum(const uint8_t state[],
                                  const uint16_t length) {
  uint8_t sum = 0;
  // Safety check so we don't go outside the array.
  if (length < 7) return 255;
  // Shamelessly inspired by:
  //   https://github.com/adafruit/Raw-IR-decoder-for-Arduino/pull/3/files
  // Count most of the '1' bits after the checksum location.
  sum += countBits(state[length - 7], 8);
  sum -= countBits(GETBITS8(state[length - 6], kLowNibble, kNibbleSize), 8);
  sum += countBits(GETBITS8(state[length - 5], 1, 7), 8);
  sum += countBits(state + length - 4, 3);
  return GETBITS8(28 - sum, kLowNibble, kNibbleSize);
}


void Ir_Samsung::on() {
    setPower(true);
}

void Ir_Samsung::off() {
    setPower(false);
}
/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool Ir_Samsung::getPower(void)
{
    return (_.Power6 == 0b11) && !_.Power1;
}

void Ir_Samsung::setPower(const bool on) {
  _.Power1 = !on;  // Cleared when on.
  _.Power6 = (on ? 0b11 : 0b00);
}

void Ir_Samsung::setTemp(const uint8_t temp) {
    uint8_t newtemp = std::max(kSamsungAcMinTemp, temp);
    newtemp = std::min(kSamsungAcMaxTemp, newtemp);
    _.Temp = newtemp - kSamsungAcMinTemp;
}

uint8_t Ir_Samsung::getTemp() {
    return _.Temp + kSamsungAcMinTemp;
}

void Ir_Samsung::setFan(const uint8_t speed) {
    switch (speed) {
        case kSamsungAcFanAuto:
        case kSamsungAcFanLow:
        case kSamsungAcFanMed:
        case kSamsungAcFanHigh:
        case kSamsungAcFanTurbo:
            if (_.Mode == kSamsungAcAuto) return;  // Not valid in Auto mode.
            break;
        case kSamsungAcFanAuto2:  // Special fan setting for when in Auto mode.
            if (_.Mode != kSamsungAcAuto) return;
            break;
        default:
            return;
    }
    _.Fan = speed;
}

uint8_t Ir_Samsung::getFan() {
    return _.Fan;
}

void Ir_Samsung::setMode(const uint8_t mode) {
    // If we get an unexpected mode, default to AUTO.
    uint8_t newmode = mode;
    if (newmode > kSamsungAcHeat) newmode = kSamsungAcAuto;
    _.Mode = newmode;

    // Auto mode has a special fan setting valid only in auto mode.
    if (newmode == kSamsungAcAuto) {
        _.Fan = kSamsungAcFanAuto2;
    } else {
        // Non-Auto can't have this fan setting
        if (_.Fan == kSamsungAcFanAuto2)
            _.Fan = kSamsungAcFanAuto;  // Default to something safe.
    }
}

uint8_t Ir_Samsung::getMode() {
    return _.Mode;
}

void Ir_Samsung::setSwing(const bool on) {
    _.Swing = (on ? kSamsungAcSwingMove : kSamsungAcSwingStop);
}

bool Ir_Samsung::getSwing() {
    return _.Swing == kSamsungAcSwingMove;
}

void Ir_Samsung::setBeep(const bool on) {
    _.Beep = on;
}

bool Ir_Samsung::getBeep() {
    return _.Beep;
}

void Ir_Samsung::setClean(const bool on) {
    _.Clean10 = on;
    _.Clean11 = on;
}

bool Ir_Samsung::getClean() {
    return _.Clean10 && _.Clean11;
}

void Ir_Samsung::setQuiet(const bool on) {
    _.Quiet1 = !on;  // Cleared when on.
    _.Quiet5 = on;
    if (on) {
        // Quiet mode seems to set fan speed to auto.
        setFan(kSamsungAcFanAuto);
        setPowerful(false);  // Quiet 'on' is mutually exclusive to Powerful.
    }
}

bool Ir_Samsung::getQuiet() {
    return !_.Quiet1 && _.Quiet5;
}

void Ir_Samsung::setPowerful(const bool on) {
    uint8_t off_value = getBreeze() ? kSamsungAcBreezeOn : 0b000;
    _.Powerful10 = (on ? kSamsungAcPowerful10On : off_value);
    if (on) {
        _.Powerful8 &= ~kSamsungAcPowerfulMask8;  // Bit needs to be cleared.
        // Powerful mode sets fan speed to Turbo.
        setFan(kSamsungAcFanTurbo);
        setQuiet(false);  // Powerful 'on' is mutually exclusive to Quiet.
    } else {
        _.Powerful8 |= kSamsungAcPowerfulMask8;  // Bit needs to be set.
        // Turning off Powerful mode sets fan speed to Auto if we were in Turbo mode
        if (_.Fan == kSamsungAcFanTurbo) setFan(kSamsungAcFanAuto);
    }
}

bool Ir_Samsung::getPowerful() {
    return !(_.Powerful8 & kSamsungAcPowerfulMask8) &&
           (_.Powerful10 == kSamsungAcPowerful10On) &&
           (_.Fan == kSamsungAcFanTurbo);
}

void Ir_Samsung::setBreeze(const bool on) {
    uint8_t off_value = getPowerful() ? kSamsungAcPowerful10On : 0b000;
    _.Breeze = (on ? kSamsungAcBreezeOn : off_value);
    if (on) {
        setFan(kSamsungAcFanAuto);
        setSwing(false);
    }
}

bool Ir_Samsung::getBreeze() {
    return (_.Breeze == kSamsungAcBreezeOn) &&
           (_.Fan == kSamsungAcFanAuto && !getSwing());
}

void Ir_Samsung::setDisplay(const bool on) {
    _.Display = on;
}

bool Ir_Samsung::getDisplay() {
    return _.Display;
}

void Ir_Samsung::setIon(const bool on) {
    _.Ion = on;
}

bool Ir_Samsung::getIon() {
    return _.Ion;
}
void Ir_Samsung::stateReset(const bool forcepower, const bool initialPower) {
    static const uint8_t kReset[kSamsungAcExtendedStateLength] = {
            0x02, 0x92, 0x0F, 0x00, 0x00, 0x00, 0xF0, 0x01, 0x02, 0xAE, 0x71, 0x00,
            0x15, 0xF0};
    memcpy(_.raw, kReset, kSamsungAcExtendedStateLength);
    _forcepower = forcepower;
    _lastsentpowerstate = initialPower;
    setPower(initialPower);
}
/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/

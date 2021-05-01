/*
 * IrDaikin.cpp
 *
 *  Created on: Jan 15, 2021
 *      Author: ductu
 */
/***********************************************************************************************************************
 * Pragma directive
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes <System Includes>
 ***********************************************************************************************************************/
#include "IrDaikin.h"

/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/
using irutils::setBit;
using irutils::setBits;
using irutils::sumNibbles;
using irutils::uint8ToBcd;
using irutils::bcdToUint8;
/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/
#define SETTIME(x, n) do { \
  uint16_t mins = n;\
  if (n >= 24 * 60) mins = 0;\
  _.x##HalfHour = (mins % 60) >= 30;\
  _.x##Hours = uint8ToBcd(mins / 60);\
} while (0)

#define GETTIME(x) bcdToUint8(_.x##Hours) * 60 + (_.x##HalfHour ? 30 : 0)

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
Ir_DaikinESP::Ir_DaikinESP(uint32_t frequency, uint16_t dutycycle):_ir_daikinESP_send(frequency, dutycycle) {
    // TODO Auto-generated constructor stub need reset state of daikin
    stateReset();
}

Ir_DaikinESP::~Ir_DaikinESP() {
    // TODO Auto-generated destructor stub
}
void Ir_DaikinESP::sendDaikinESP(const uint16_t repeat) {
    send_esp(getRaw(), kDaikinStateLength, repeat);
}

void Ir_DaikinESP::setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp) {
    setPower(power);
    setMode(convertMode(stdAc::opmode_t(mode)));
    setFan(convertFan(stdAc::fanspeed_t(speed)));
    setTemp(temp);
}
/***********************************************************************************************************************
 * Static Functions
 ***********************************************************************************************************************/

void Ir_DaikinESP::send_esp(const unsigned char *data, const uint16_t nbytes, const uint16_t repeat) {
    if (nbytes < kDaikinStateLengthShort)
        return;  // Not enough bytes to send a proper message.

    for (uint16_t r = 0; r <= repeat; r++) {
        uint16_t offset = 0;
        // Send the header, 0b00000
        _ir_daikinESP_send.sendGeneric(0, 0,  // No header for the header
                    kDaikinBitMark, kDaikinOneSpace, kDaikinBitMark,
                    kDaikinZeroSpace, kDaikinBitMark, kDaikinZeroSpace + kDaikinGap,
                    (uint64_t)0b00000, kDaikinHeaderLength, 38, false, 0, 50);
        // Data #1
        if (nbytes < kDaikinStateLength) {  // Are we using the legacy size?
            // Do this as a constant to save RAM and keep in flash memory
            _ir_daikinESP_send.sendGeneric(kDaikinHdrMark, kDaikinHdrSpace, kDaikinBitMark,
                        kDaikinOneSpace, kDaikinBitMark, kDaikinZeroSpace,
                        kDaikinBitMark, kDaikinZeroSpace + kDaikinGap,
                        kDaikinFirstHeader64, 64, 38, false, 0, 50);
        } else {  // We are using the newer/more correct size.
            _ir_daikinESP_send.sendGeneric(kDaikinHdrMark, kDaikinHdrSpace, kDaikinBitMark,
                        kDaikinOneSpace, kDaikinBitMark, kDaikinZeroSpace,
                        kDaikinBitMark, kDaikinZeroSpace + kDaikinGap,
                        data, kDaikinSection1Length, 38, false, 0, 50);
            offset += kDaikinSection1Length;
        }
        // Data #2
        _ir_daikinESP_send.sendGeneric(kDaikinHdrMark, kDaikinHdrSpace, kDaikinBitMark,
                    kDaikinOneSpace, kDaikinBitMark, kDaikinZeroSpace,
                    kDaikinBitMark, kDaikinZeroSpace + kDaikinGap,
                    data + offset, kDaikinSection2Length, 38, false, 0, 50);
        offset += kDaikinSection2Length;
        // Data #3
        _ir_daikinESP_send.sendGeneric(kDaikinHdrMark, kDaikinHdrSpace, kDaikinBitMark,
                    kDaikinOneSpace, kDaikinBitMark, kDaikinZeroSpace,
                    kDaikinBitMark, kDaikinZeroSpace + kDaikinGap,
                    data + offset, nbytes - offset, 38, false, 0, 50);
    }
}
/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t Ir_DaikinESP::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kDaikinCool;
    case stdAc::opmode_t::kHeat: return kDaikinHeat;
    case stdAc::opmode_t::kDry: return kDaikinDry;
    case stdAc::opmode_t::kFan: return kDaikinFan;
    default: return kDaikinAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t Ir_DaikinESP::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin: return kDaikinFanQuiet;
    case stdAc::fanspeed_t::kLow: return kDaikinFanMin;
    case stdAc::fanspeed_t::kMedium: return kDaikinFanMed;
    case stdAc::fanspeed_t::kHigh: return kDaikinFanMax - 1;
    case stdAc::fanspeed_t::kMax: return kDaikinFanMax;
    default: return kDaikinFanAuto;
  }
}


void Ir_DaikinESP::on() {
    setPower(true);
}

void Ir_DaikinESP::off() {
    setPower(false);
}

void Ir_DaikinESP::setPower(const bool on) {
    _.Power = on;
}

bool Ir_DaikinESP::getPower() const {
    return _.Power;
}

void Ir_DaikinESP::setTemp(const uint8_t temp) {
    uint8_t degrees = std::max(temp, kDaikinMinTemp);
    degrees = std::min(degrees, kDaikinMaxTemp);
    _.Temp = degrees;
}
/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t Ir_DaikinESP::getTemp() {
    return _.Temp;
}
/// Set the speed of the fan.
/// @param[in] fan The desired setting.
/// @note 1-5 or kDaikinFanAuto or kDaikinFanQuiet
void Ir_DaikinESP::setFan(const uint8_t fan) {
    // Set the fan speed bits, leave low 4 bits alone
    uint8_t fanset;
    if (fan == kDaikinFanQuiet || fan == kDaikinFanAuto)
        fanset = fan;
    else if (fan < kDaikinFanMin || fan > kDaikinFanMax)
        fanset = kDaikinFanAuto;
    else
        fanset = 2 + fan;
    _.Fan = fanset;
}
/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t Ir_DaikinESP::getFan() {
    uint8_t fan = _.Fan;
    if (fan != kDaikinFanQuiet && fan != kDaikinFanAuto) fan -= 2;
    return fan;
}

uint8_t Ir_DaikinESP::getMode() {
    return _.Mode;
}

void Ir_DaikinESP::setMode(const uint8_t mode) {
    switch (mode) {
        case kDaikinAuto:
        case kDaikinCool:
        case kDaikinHeat:
        case kDaikinFan:
        case kDaikinDry:
            _.Mode = mode;
            break;
        default:
            _.Mode = kDaikinAuto;
    }
}
void Ir_DaikinESP::setSwingVertical(const bool on) {
    _.SwingV = (on ? kDaikinSwingOn : kDaikinSwingOff);
}
/// Get the Vertical Swing mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool Ir_DaikinESP::getSwingVertical() {
	return _.SwingV;
}
/// Get the Vertical Swing mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
void Ir_DaikinESP::setSwingHorizontal(const bool on) {
    _.SwingH = (on ? kDaikinSwingOn : kDaikinSwingOff);
}

bool Ir_DaikinESP::getSwingHorizontal() {
    return _.SwingV;
}

bool Ir_DaikinESP::getQuiet() {
    return _.Quiet;
}

void Ir_DaikinESP::setQuiet(const bool on) {
    _.Quiet = on;
    // Powerful & Quiet mode being on are mutually exclusive.
    if (on) setPowerful(false);
}

bool Ir_DaikinESP::getPowerful() {
    return _.Powerful;
}

void Ir_DaikinESP::setPowerful(const bool on) {
    _.Powerful = on;
    if (on) {
        // Powerful, Quiet, & Econo mode being on are mutually exclusive.
        setQuiet(false);
        setEcono(false);
    }
}

void Ir_DaikinESP::setSensor(const bool on) {
    _.Sensor = on;
}

bool Ir_DaikinESP::getSensor() {
    return _.Sensor;
}

bool Ir_DaikinESP::getEcono() {
    return _.Econo;
}

void Ir_DaikinESP::setEcono(const bool on) {
    _.Econo = on;
    // Powerful & Econo mode being on are mutually exclusive.
    if (on) setPowerful(false);
}

void Ir_DaikinESP::setMold(const bool on) {
    _.Mold = on;
}

bool Ir_DaikinESP::getMold() {
    return _.Mold;
}

bool Ir_DaikinESP::getComfort() {
    return _.Comfort;
}

void Ir_DaikinESP::setComfort(const bool on) {
    _.Comfort = on;
}

void Ir_DaikinESP::enableOnTimer(const uint16_t starttime) {
    _.OnTimer = true;
    _.OnTime = starttime;
}

void Ir_DaikinESP::disableOnTimer() {
    _.OnTimer = false;
    _.OnTime = kDaikinUnusedTime;
}

uint16_t Ir_DaikinESP::getOnTime() {
    return _.OnTime;
}

bool Ir_DaikinESP::getOnTimerEnabled() {
    return _.OnTimer;
}

void Ir_DaikinESP::enableOffTimer(const uint16_t endtime) {
    _.OffTimer = true;
    _.OffTime = endtime;
}

void Ir_DaikinESP::disableOffTimer() {
    _.OffTimer = false;
    _.OffTime = kDaikinUnusedTime;
}

uint16_t Ir_DaikinESP::getOffTime() {
    return _.OffTime;
}

bool Ir_DaikinESP::getOffTimerEnabled() {
    return _.OffTimer;
}

void Ir_DaikinESP::setCurrentTime(const uint16_t mins_since_midnight) {
    uint16_t mins = mins_since_midnight;
    if (mins > 24 * 60) mins = 0;  // If > 23:59, set to 00:00
    _.CurrentTime = mins;
}

uint16_t Ir_DaikinESP::getCurrentTime() {
    return _.CurrentTime;
}

uint8_t Ir_DaikinESP::getCurrentDay() {
    return _.CurrentDay;
}
void Ir_DaikinESP::setCurrentDay(const uint8_t day_of_week) {
    _.CurrentDay = day_of_week;
}

void Ir_DaikinESP::setWeeklyTimerEnable(const bool on) {
    _.WeeklyTimer = !on;
}

bool Ir_DaikinESP::getWeeklyTimerEnable() {
    return !_.WeeklyTimer;
}

uint8_t * Ir_DaikinESP::getRaw() {
    checksum();  // Ensure correct settings before sending.
    return _.raw;
}

void Ir_DaikinESP::setRaw(const uint8_t *new_code, const uint16_t length) {
    uint8_t offset = 0;
    if (length == kDaikinStateLengthShort) {  // Handle the "short" length case.
        offset = kDaikinStateLength - kDaikinStateLengthShort;
        stateReset();
    }
    for (uint8_t i = 0; i < length && i < kDaikinStateLength; i++)
        _.raw[i + offset] = new_code[i];
}

bool Ir_DaikinESP::validChecksum(uint8_t *state, const uint16_t length) {
    // Data #1
    if (length < kDaikinSection1Length ||
        state[kDaikinByteChecksum1] != sumBytes(state, kDaikinSection1Length - 1))
        return false;
    // Data #2
    if (length < kDaikinSection1Length + kDaikinSection2Length ||
        state[kDaikinByteChecksum2] != sumBytes(state + kDaikinSection1Length,
                                                kDaikinSection2Length - 1))
        return false;
    // Data #3
    if (length < kDaikinSection1Length + kDaikinSection2Length + 2 ||
        state[length - 1] != sumBytes(state + kDaikinSection1Length +
                                      kDaikinSection2Length,
                                      length - (kDaikinSection1Length +
                                                kDaikinSection2Length) - 1))
        return false;
    return true;
}

void Ir_DaikinESP::stateReset() {
    for (uint8_t i = 0; i < kDaikinStateLength; i++) _.raw[i] = 0x0;

    _.raw[0] = 0x11;
    _.raw[1] = 0xDA;
    _.raw[2] = 0x27;
    _.raw[4] = 0xC5;
    // _.raw[7] is a checksum byte, it will be set by checksum().
    _.raw[8] = 0x11;
    _.raw[9] = 0xDA;
    _.raw[10] = 0x27;
    _.raw[12] = 0x42;
    // _.raw[15] is a checksum byte, it will be set by checksum().
    _.raw[16] = 0x11;
    _.raw[17] = 0xDA;
    _.raw[18] = 0x27;
    _.raw[21] = 0x49;
    _.raw[22] = 0x1E;
    _.raw[24] = 0xB0;
    _.raw[27] = 0x06;
    _.raw[28] = 0x60;
    _.raw[31] = 0xC0;
    // _.raw[34] is a checksum byte, it will be set by checksum().
    checksum();
}

void Ir_DaikinESP::checksum() {
    _.Sum1 = sumBytes(_.raw, kDaikinSection1Length - 1);
    _.Sum2 = sumBytes(_.raw + kDaikinSection1Length, kDaikinSection2Length - 1);
    _.Sum3 = sumBytes(_.raw + kDaikinSection1Length + kDaikinSection2Length,
                      kDaikinSection3Length - 1);
}
/***********************************************************************************************************************
 * End of class DaikinESP
 ***********************************************************************************************************************/

Ir_Daikin152::Ir_Daikin152(uint32_t frequency, uint16_t dutycycle):_ir_daikin152_send(frequency, dutycycle) {
	// TODO Auto-generated constructor stub
	stateReset();
}

Ir_Daikin152::~Ir_Daikin152() {
	// TODO Auto-generated destructor stub
}

void Ir_Daikin152::sendDaikin152(const uint16_t repeat) {
	uint8_t state[19] = {0x11, 0xDA, 0x27, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC5, 0x00, 0x00, 0x07};
    send(getRaw(), kDaikin152StateLength, repeat);
}

void Ir_Daikin152::setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp) {
    setPower(power);
    setMode(convertMode(stdAc::opmode_t(mode)));
    setFan(convertFan(stdAc::fanspeed_t(speed)));
    setTemp(temp);
}
/***********************************************************************************************************************
 * Static Functions
 ***********************************************************************************************************************/
void Ir_Daikin152::send(const unsigned char *data, const uint16_t nbytes, const uint16_t repeat) {
    for (uint16_t r = 0; r <= repeat; r++) {
        // Leader
        _ir_daikin152_send.sendGeneric(0, 0, kDaikin152BitMark, kDaikin152OneSpace,
                    kDaikin152BitMark, kDaikin152ZeroSpace,
                    kDaikin152BitMark, kDaikin152Gap,
                    (uint64_t)0, kDaikin152LeaderBits,
                    kDaikin152Freq, false, 0, 50);
        // Header + Data + Footer
        _ir_daikin152_send.sendGeneric(kDaikin152HdrMark, kDaikin152HdrSpace, kDaikin152BitMark,
                    kDaikin152OneSpace, kDaikin152BitMark, kDaikin152ZeroSpace,
                    kDaikin152BitMark, kDaikin152Gap, data,
                    nbytes, kDaikin152Freq, false, 0, 50);
    }
}
/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t Ir_Daikin152::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
	case stdAc::opmode_t::kCool: return kDaikinCool;
	case stdAc::opmode_t::kHeat: return kDaikinHeat;
	case stdAc::opmode_t::kDry: return kDaikinDry;
	case stdAc::opmode_t::kFan: return kDaikinFan;
	default: return kDaikinAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t Ir_Daikin152::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
	case stdAc::fanspeed_t::kMin: return kDaikinFanQuiet;
	case stdAc::fanspeed_t::kLow: return kDaikinFanMin;
	case stdAc::fanspeed_t::kMedium: return kDaikinFanMed;
	case stdAc::fanspeed_t::kHigh: return kDaikinFanMax - 1;
	case stdAc::fanspeed_t::kMax: return kDaikinFanMax;
	default: return kDaikinFanAuto;
  };
}

uint8_t * Ir_Daikin152::getRaw() {
    checksum();  // Ensure correct settings before sending.
    return _.raw;
}

void Ir_Daikin152::setRaw(const uint8_t *new_code) {
    memcpy(_.raw, new_code, kDaikin152StateLength);
}

bool Ir_Daikin152::validChecksum(uint8_t *state, const uint16_t length) {
    // Validate the checksum of the given state.
    if (length <= 1 || state[length - 1] != sumBytes(state, length - 1))
        return false;
    else
        return true;
}

void Ir_Daikin152::on() {
    setPower(true);
}

void Ir_Daikin152::off() {
    setPower(false);
}

void Ir_Daikin152::setPower(const bool on) {
    _.Power = on;
}

bool Ir_Daikin152::getPower() const {
    return _.Power;
}

uint8_t Ir_Daikin152::getTemp() const {
    return _.Temp;
}

void Ir_Daikin152::setTemp(const uint8_t temp) {
    uint8_t degrees = std::max(
            temp, (_.Mode == kDaikinHeat) ? kDaikinMinTemp : kDaikin2MinCoolTemp);
    degrees = std::min(degrees, kDaikinMaxTemp);
    if (temp == kDaikin152FanTemp) degrees = temp;  // Handle fan only temp.
    _.Temp = degrees;
}

void Ir_Daikin152::setFan(const uint8_t fan) {
    // Set the fan speed bits, leave low 4 bits alone
    uint8_t fanset;
    if (fan == kDaikinFanQuiet || fan == kDaikinFanAuto)
        fanset = fan;
    else if (fan < kDaikinFanMin || fan > kDaikinFanMax)
        fanset = kDaikinFanAuto;
    else
        fanset = 2 + fan;
    _.Fan = fanset;
}

uint8_t Ir_Daikin152::getFan() const {
    const uint8_t fan = _.Fan;
    switch (fan) {
        case kDaikinFanAuto:
        case kDaikinFanQuiet: return fan;
        default: return fan - 2;
    }
}

void Ir_Daikin152::setMode(const uint8_t mode) {
    switch (mode) {
        case kDaikinFan:
            setTemp(kDaikin152FanTemp);  // Handle special temp for fan mode.
            break;
        case kDaikinDry:
            setTemp(kDaikin152DryTemp);  // Handle special temp for dry mode.
            break;
        case kDaikinAuto:
        case kDaikinCool:
        case kDaikinHeat:
            break;
        default:
            _.Mode = kDaikinAuto;
            return;
    }
    _.Mode = mode;
}

uint8_t Ir_Daikin152::getMode() const {
    return _.Mode;
}

void Ir_Daikin152::setSwingV(const bool on) {
    _.SwingV = (on ? kDaikinSwingOn : kDaikinSwingOff);
}

bool Ir_Daikin152::getSwingV() const {
    return _.SwingV;
}

bool Ir_Daikin152::getQuiet() const {
    return _.Quiet;
}

void Ir_Daikin152::setQuiet(const bool on) {
    _.Quiet = on;
    // Powerful & Quiet mode being on are mutually exclusive.
    if (on) setPowerful(false);
}

bool Ir_Daikin152::getPowerful() const {
    return _.Powerful;
}

void Ir_Daikin152::setPowerful(const bool on) {
    _.Powerful = on;
    if (on) {
        // Powerful, Quiet, Comfort & Econo mode being on are mutually exclusive.
        setQuiet(false);
        setComfort(false);
        setEcono(false);
    }
}
void Ir_Daikin152::setSensor(const bool on) {
    _.Sensor = on;
}

bool Ir_Daikin152::getSensor() const {
    return _.Sensor;
}

void Ir_Daikin152::setEcono(const bool on) {
    _.Econo = on;
    // Powerful & Econo mode being on are mutually exclusive.
    if (on) setPowerful(false);
}

bool Ir_Daikin152::getEcono() const {
    return _.Econo;
}

void Ir_Daikin152::setComfort(const bool on) {
    _.Comfort = on;
    if (on) {
        // Comfort mode is incompatible with Powerful mode.
        setPowerful(false);
        // It also sets the fan to auto and turns off swingv.
        setFan(kDaikinFanAuto);
        setSwingV(false);
    }
}

bool Ir_Daikin152::getComfort() const {
    return _.Comfort;
}

void Ir_Daikin152::stateReset() {
    for (uint8_t i = 3; i < kDaikin152StateLength; i++) _.raw[i] = 0x00;
    _.raw[0] =  0x11;
    _.raw[1] =  0xDA;
    _.raw[2] =  0x27;
    _.raw[15] = 0xC5;
    // _.raw[19] is a checksum byte, it will be set by checksum().
}

void Ir_Daikin152::checksum() {
    _.Sum = sumBytes(_.raw, kDaikin152StateLength - 1);
}

/***********************************************************************************************************************
 * End of class Daikin152
 ***********************************************************************************************************************/
Ir_Daikin64::Ir_Daikin64(uint32_t frequency, uint16_t dutycycle):_ir_daikin64_send(frequency, dutycycle) {
    // TODO Auto-generated constructor stub
    stateReset();
}

Ir_Daikin64::~Ir_Daikin64() {
    // TODO Auto-generated destructor stub
}

void Ir_Daikin64::sendDaikin64(const uint16_t repeat) {
    send_64(getRaw(), kDaikin64Bits, repeat);
}

void Ir_Daikin64::setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp) {
    setPowerToggle(power);
    setMode(convertMode(stdAc::opmode_t(mode)));
    setFan(convertFan(stdAc::fanspeed_t(speed)));
    setTemp(temp);
}
/***********************************************************************************************************************
 * Static Functions
 ***********************************************************************************************************************/
void Ir_Daikin64::send_64(const uint64_t data, const uint16_t nbits, const uint16_t repeat) {
    for (uint16_t r = 0; r <= repeat; r++) {
        for (uint8_t i = 0; i < 2; i++) {
            // Leader
            _ir_daikin64_send.mark_space(kDaikin64LdrMark, kDaikin64LdrSpace);
        }
        // Header + Data + Footer #1
        _ir_daikin64_send.sendGeneric(kDaikin64HdrMark, kDaikin64HdrSpace,
                    kDaikin64BitMark, kDaikin64OneSpace,
                    kDaikin64BitMark, kDaikin64ZeroSpace,
                    kDaikin64BitMark, kDaikin64Gap,
                    data, nbits, kDaikin64Freq, false, 0, 50);
        // Footer #2
        _ir_daikin64_send.mark_space(kDaikin64HdrMark, 0);
    }
}
/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t Ir_Daikin64::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kDry: return kDaikin64Dry;
    case stdAc::opmode_t::kFan: return kDaikin64Fan;
    default: return kDaikinCool;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t Ir_Daikin64::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:    return kDaikin64FanQuiet;
    case stdAc::fanspeed_t::kLow:    return kDaikin64FanLow;
    case stdAc::fanspeed_t::kMedium: return kDaikin64FanMed;
    case stdAc::fanspeed_t::kHigh:   return kDaikin64FanHigh;
    case stdAc::fanspeed_t::kMax:    return kDaikin64FanTurbo;
    default:                         return kDaikin64FanAuto;
  }
}
uint64_t Ir_Daikin64::getRaw() {
    checksum();  // Ensure correct settings before sending.
    return _.raw;
}

void Ir_Daikin64::setRaw(const uint64_t new_state) {
    _.raw = new_state;
}

uint8_t Ir_Daikin64::calcChecksum(const uint64_t state) {
    uint64_t data = GETBITS64(state, 0, kDaikin64ChecksumOffset);
    uint8_t result = 0;
    for (; data; data >>= 4)  // Add each nibble together.
        result += GETBITS64(data, 0, 4);
    return result & 0xF;
}

bool Ir_Daikin64::validChecksum(const uint64_t state) {
    // Validate the checksum of the given state.
    return (GETBITS64(state, kDaikin64ChecksumOffset,
                      kDaikin64ChecksumSize) == calcChecksum(state));
}

void Ir_Daikin64::setPowerToggle(const bool on) {
    _.Power = on;
}

bool Ir_Daikin64::getPowerToggle() const {
    return _.Power;
}

void Ir_Daikin64::setTemp(const uint8_t temp) {
    uint8_t degrees = std::max(temp, kDaikin64MinTemp);
    degrees = std::min(degrees, kDaikin64MaxTemp);
    _.Temp = uint8ToBcd(degrees);
}

uint8_t Ir_Daikin64::getTemp() const {
    return bcdToUint8(_.Temp);
}

void Ir_Daikin64::setFan(const uint8_t speed) {
    switch (speed) {
        case kDaikin64FanQuiet:
        case kDaikin64FanTurbo:
        case kDaikin64FanAuto:
        case kDaikin64FanHigh:
        case kDaikin64FanMed:
        case kDaikin64FanLow:
            _.Fan = speed;
            break;
        default:
            _.Fan = kDaikin64FanAuto;
    }
}

uint8_t Ir_Daikin64::getFan() const {
    return _.Fan;
}

void Ir_Daikin64::setMode(const uint8_t mode) {
    switch (mode) {
        case kDaikin64Fan:
        case kDaikin64Dry:
        case kDaikin64Cool:
            _.Mode = mode;
            break;
        default:
            _.Mode = kDaikin64Cool;
    }
}

uint8_t Ir_Daikin64::getMode() const {
    return _.Mode;
}

void Ir_Daikin64::setSwingVertical(const bool on) {
    _.SwingV = on;
}

bool Ir_Daikin64::getSwingVertical() const {
    return _.SwingV;
}

void Ir_Daikin64::setSleep(const bool on) {
    _.Sleep = on;
}

bool Ir_Daikin64::getSleep() const {
    return _.Sleep;
}

bool Ir_Daikin64::getQuiet() const {
    return _.Fan == kDaikin64FanQuiet;
}
void Ir_Daikin64::setQuiet(const bool on) {
    if (on) {
        setFan(kDaikin64FanQuiet);
    } else if (_.Fan == kDaikin64FanQuiet) {
        setFan(kDaikin64FanAuto);
    }
}
bool Ir_Daikin64::getTurbo() const {
    return _.Fan == kDaikin64FanTurbo;
}
void Ir_Daikin64::setTurbo(const bool on) {
    if (on) {
        setFan(kDaikin64FanTurbo);
    } else if (_.Fan == kDaikin64FanTurbo) {
        setFan(kDaikin64FanAuto);
    }
}
void Ir_Daikin64::setClock(const uint16_t mins_since_midnight) {
    uint16_t mins = mins_since_midnight;
    if (mins_since_midnight >= 24 * 60) mins = 0;  // Bounds check.
    _.ClockMins = uint8ToBcd(mins % 60);
    _.ClockHours = uint8ToBcd(mins / 60);
}
uint16_t Ir_Daikin64::getClock() const {
    return bcdToUint8(_.ClockHours) * 60 + bcdToUint8(_.ClockMins);
}
void Ir_Daikin64::setOnTimeEnabled(const bool on) {
    _.OnTimer = on;
}
bool Ir_Daikin64::getOnTimeEnabled() const {
    return _.OnTimer;
}
void Ir_Daikin64::setOnTime(const uint16_t mins_since_midnight) {
    SETTIME(On, mins_since_midnight);
}

uint16_t Ir_Daikin64::getOnTime() const {
    return GETTIME(On);
}
void Ir_Daikin64::setOffTimeEnabled(const bool on) {
    _.OffTimer = on;
}

bool Ir_Daikin64::getOffTimeEnabled() const {
    return _.OffTimer;
}

void Ir_Daikin64::setOffTime(const uint16_t mins_since_midnight) {
    SETTIME(Off, mins_since_midnight);
}

uint16_t Ir_Daikin64::getOffTime() const {
    return GETTIME(Off);
}

void Ir_Daikin64::stateReset() {
    _.raw = kDaikin64KnownGoodState;
}

void Ir_Daikin64::checksum() {
    _.Sum = calcChecksum(_.raw);
}
/***********************************************************************************************************************
 * End of class Daikin64
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * Start of class Daikin176
 ***********************************************************************************************************************/
Ir_Daikin176::Ir_Daikin176(uint32_t frequency, uint16_t dutycycle):_ir_daikin176_send(frequency, dutycycle){
    stateReset();
}


Ir_Daikin176::~Ir_Daikin176() {

}

void Ir_Daikin176::sendDaikin176(const uint16_t repeat) {
	send(getRaw(), kDaikin176StateLength, repeat);
}

void Ir_Daikin176::send(const unsigned char *data, const uint16_t nbytes, const uint16_t repeat) {
    if (nbytes < kDaikin176Section1Length)
        return;  // Not enough bytes to send a partial message.

    for (uint16_t r = 0; r <= repeat; r++) {
        // Section #1
        _ir_daikin176_send.sendGeneric(kDaikin176HdrMark, kDaikin176HdrSpace, kDaikin176BitMark,
                                       kDaikin176OneSpace, kDaikin176BitMark, kDaikin176ZeroSpace,
                                       kDaikin176BitMark, kDaikin176Gap, data,
                                       kDaikin176Section1Length,
                                       kDaikin176Freq, false, 0, 50);
        // Section #2
        _ir_daikin176_send.sendGeneric(kDaikin176HdrMark, kDaikin176HdrSpace, kDaikin176BitMark,
                                       kDaikin176OneSpace, kDaikin176BitMark, kDaikin176ZeroSpace,
                                       kDaikin176BitMark, kDaikin176Gap,
                                       data + kDaikin176Section1Length,
                                       nbytes - kDaikin176Section1Length,
                                       kDaikin176Freq, false, 0, 50);
    }
}
void Ir_Daikin176::setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp) {
    setPower(power);
    setMode(convertMode(stdAc::opmode_t(mode)));
    setFan(convertFan(stdAc::fanspeed_t(speed)));
    setTemp(temp);
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t Ir_Daikin176::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kDry:   return kDaikin176Dry;
    case stdAc::opmode_t::kHeat:  return kDaikin176Heat;
    case stdAc::opmode_t::kFan:   return kDaikin176Fan;
    case stdAc::opmode_t::kAuto:  return kDaikin176Auto;
    default:                      return kDaikin176Cool;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t Ir_Daikin176::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow: return kDaikinFanMin;
    default: return kDaikin176FanMax;
  }
}

uint8_t * Ir_Daikin176::getRaw() {
    checksum();  // Ensure correct settings before sending.
    return _.raw;
}

void Ir_Daikin176::setRaw(const uint8_t *new_code) {
    memcpy(_.raw, new_code, kDaikin176StateLength);
    _saved_temp = getTemp();
}

bool Ir_Daikin176::validChecksum(uint8_t *state, const uint16_t length) {
    // Validate the checksum of section #1.
    if (length <= kDaikin176Section1Length - 1 ||
        state[kDaikin176Section1Length - 1] != sumBytes(
                state, kDaikin176Section1Length - 1))
        return false;
    // Validate the checksum of section #2 (a.k.a. the rest)
    if (length <= kDaikin176Section1Length + 1 ||
        state[length - 1] != sumBytes(state + kDaikin176Section1Length,
                                      length - kDaikin176Section1Length - 1))
        return false;
    return true;
}

void Ir_Daikin176::on() {
    setPower(true);
}

void Ir_Daikin176::off() {
    setPower(false);
}

void Ir_Daikin176::setPower(const bool on) {
    _.ModeButton = 0;
    _.Power = on;
}

bool Ir_Daikin176::getPower() const {
    return _.Power;
}

void Ir_Daikin176::setTemp(const uint8_t temp) {
    uint8_t degrees = std::min(kDaikinMaxTemp, std::max(temp, kDaikinMinTemp));
    _saved_temp = degrees;
    switch (_.Mode) {
        case kDaikin176Dry:
        case kDaikin176Fan:
            degrees = kDaikin176DryFanTemp; break;
    }
    _.Temp = degrees - 9;
    _.ModeButton = 0;
}

uint8_t Ir_Daikin176::getTemp() const {
    return _.Temp + 9;
}

void Ir_Daikin176::setMode(const uint8_t mode) {
    uint8_t altmode = 0;
    // Set the mode bits.
    _.Mode = mode;
    // Daikin172 has some alternate/additional mode bits that need to be changed
    // in line with the operating mode. The following few lines match up these
    // bits with the corresponding operating bits.
    switch (mode) {
        case kDaikin176Dry:  altmode = 2; break;
        case kDaikin176Fan:  altmode = 6; break;
        case kDaikin176Auto:
        case kDaikin176Cool:
        case kDaikin176Heat: altmode = 7; break;
        default: _.Mode = kDaikin176Cool; altmode = 7; break;
    }
    // Set the additional mode bits.
    _.AltMode = altmode;
    setTemp(_saved_temp);
    // Needs to happen after setTemp() as it will clear it.
    _.ModeButton = kDaikin176ModeButton;
}

uint8_t Ir_Daikin176::getMode() const {
    return _.Mode;
}

void Ir_Daikin176::setFan(const uint8_t fan) {
    switch (fan) {
        case kDaikinFanMin:
        case kDaikin176FanMax:
            _.Fan = fan;
            break;
        default:
            _.Fan = kDaikin176FanMax;
            break;
    }
    _.ModeButton = 0;
}

uint8_t Ir_Daikin176::getFan() const {
    return _.Fan;
}

void Ir_Daikin176::setSwingHorizontal(const uint8_t position) {
    switch (position) {
        case kDaikin176SwingHOff:
        case kDaikin176SwingHAuto:
            _.SwingH = position;
            break;
        default: _.SwingH = kDaikin176SwingHAuto;
    }
}

uint8_t Ir_Daikin176::getSwingHorizontal() const {
    return _.SwingH;
}

void Ir_Daikin176::stateReset() {
    for (uint8_t i = 0; i < kDaikin176StateLength; i++) _.raw[i] = 0x00;
    _.raw[0] =  0x11;
    _.raw[1] =  0xDA;
    _.raw[2] =  0x17;
    _.raw[3] =  0x18;
    _.raw[4] =  0x04;
    // _.raw[6] is a checksum byte, it will be set by checksum().
    _.raw[7] =  0x11;
    _.raw[8] =  0xDA;
    _.raw[9] =  0x17;
    _.raw[10] = 0x18;
    _.raw[12] = 0x73;
    _.raw[14] = 0x20;
    _.raw[18] = 0x16;  // Fan speed and swing
    _.raw[20] = 0x20;
    // _.raw[21] is a checksum byte, it will be set by checksum().
    _saved_temp = getTemp();
}

void Ir_Daikin176::checksum() {
    _.Sum1 = sumBytes(_.raw, kDaikin176Section1Length - 1);
    _.Sum2 = sumBytes(_.raw + kDaikin176Section1Length,
                      kDaikin176Section2Length - 1);
}
/***********************************************************************************************************************
 * End of class Daikin176
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * End of class Ir_Daikin128
 ***********************************************************************************************************************/
Ir_Daikin128::Ir_Daikin128(uint32_t frequency, uint16_t dutycycle) :_ir_daikin128_send(frequency, dutycycle){
    stateReset();
}

Ir_Daikin128::~Ir_Daikin128() {

}

void Ir_Daikin128::sendDaikin128(const uint16_t repeat) {
	send(getRaw(), kDaikin128StateLength, repeat);
}

void Ir_Daikin128::send(const unsigned char *data, const uint16_t nbytes, const uint16_t repeat) {
	  if (nbytes < kDaikin128SectionLength)
	    return;  // Not enough bytes to send a partial message.

	  for (uint16_t r = 0; r <= repeat; r++) {
	    // Leader
	    for (uint8_t i = 0; i < 2; i++) {
	    	_ir_daikin128_send.mark_space(kDaikin128LeaderMark, kDaikin128LeaderSpace);
//	      mark(kDaikin128LeaderMark);
//	      space(kDaikin128LeaderSpace);
	    }
	    // Section #1 (Header + Data)
	    _ir_daikin128_send.sendGeneric(kDaikin128HdrMark, kDaikin128HdrSpace, kDaikin128BitMark,
	                kDaikin128OneSpace, kDaikin128BitMark, kDaikin128ZeroSpace,
	                kDaikin128BitMark, kDaikin128Gap, data,
	                kDaikin128SectionLength,
	                kDaikin128Freq, false, 0, 50);
	    // Section #2 (Data + Footer)
	    _ir_daikin128_send.sendGeneric(0, 0, kDaikin128BitMark,
	                kDaikin128OneSpace, kDaikin128BitMark, kDaikin128ZeroSpace,
	                kDaikin128FooterMark, kDaikin128Gap,
	                data + kDaikin128SectionLength,
	                nbytes - kDaikin128SectionLength,
	                kDaikin128Freq, false, 0, 50);
	  }
}
void Ir_Daikin128::setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp) {
	setPowerToggle(power);
    setMode(convertMode(stdAc::opmode_t(mode)));
    setFan(convertFan(stdAc::fanspeed_t(speed)));
    setTemp(temp);
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t Ir_Daikin128::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kDaikin128Cool;
    case stdAc::opmode_t::kHeat: return kDaikin128Heat;
    case stdAc::opmode_t::kDry: return kDaikinDry;
    case stdAc::opmode_t::kFan: return kDaikin128Fan;
    default: return kDaikin128Auto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t Ir_Daikin128::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin: return kDaikinFanQuiet;
    case stdAc::fanspeed_t::kLow: return kDaikin128FanLow;
    case stdAc::fanspeed_t::kMedium: return kDaikin128FanMed;
    case stdAc::fanspeed_t::kHigh: return kDaikin128FanHigh;
    case stdAc::fanspeed_t::kMax: return kDaikin128FanPowerful;
    default: return kDaikin128FanAuto;
  }
}

void Ir_Daikin128::setPowerToggle(const bool toggle) {
    _.Power = toggle;
}

bool Ir_Daikin128::getPowerToggle() const {
    return _.Power;
}

void Ir_Daikin128::setTemp(const uint8_t temp) {
    _.Temp = uint8ToBcd(std::min(kDaikin128MaxTemp,
                                 std::max(temp, kDaikin128MinTemp)));
}
uint8_t Ir_Daikin128::getTemp() const {
    return bcdToUint8(_.Temp);
}

void Ir_Daikin128::setFan(const uint8_t speed) {
    uint8_t new_speed = speed;
    uint8_t mode = _.Mode;
    switch (speed) {
        case kDaikin128FanQuiet:
        case kDaikin128FanPowerful:
            if (mode == kDaikin128Auto) new_speed = kDaikin128FanAuto;
            // FALL-THRU
            break;
        case kDaikin128FanAuto:
        case kDaikin128FanHigh:
        case kDaikin128FanMed:
        case kDaikin128FanLow:
            _.Fan = new_speed;
            break;
        default:
            _.Fan = kDaikin128FanAuto;
            return;
    }
}

uint8_t Ir_Daikin128::getFan() const {
    return _.Fan;
}

uint8_t Ir_Daikin128::getMode() const {
    return _.Mode;
}

void Ir_Daikin128::setMode(const uint8_t mode) {
    switch (mode) {
        case kDaikin128Auto:
        case kDaikin128Cool:
        case kDaikin128Heat:
        case kDaikin128Fan:
        case kDaikin128Dry:
            _.Mode = mode;
            break;
        default:
            _.Mode = kDaikin128Auto;
            break;
    }
    // Force a reset of mode dependant things.
    setFan(getFan());  // Covers Quiet & Powerful too.
    setEcono(getEcono());
}

void Ir_Daikin128::setSwingVertical(const bool on) {
    _.SwingV = on;
}

bool Ir_Daikin128::getSwingVertical() const {
    return _.SwingV;
}

bool Ir_Daikin128::getSleep() const {
    return _.Sleep;
}

void Ir_Daikin128::setSleep(const bool on) {
    _.Sleep = on;
}

bool Ir_Daikin128::getQuiet() const {
    return _.Fan == kDaikin128FanQuiet;
}

void Ir_Daikin128::setQuiet(const bool on) {
    uint8_t mode = _.Mode;
    if (on && (mode == kDaikin128Cool || mode == kDaikin128Heat))
        setFan(kDaikin128FanQuiet);
    else if (_.Fan == kDaikin128FanQuiet)
        setFan(kDaikin128FanAuto);
}

bool Ir_Daikin128::getPowerful() const {
    return _.Fan == kDaikin128FanPowerful;
}

void Ir_Daikin128::setPowerful(const bool on) {
    uint8_t mode = _.Mode;
    if (on && (mode == kDaikin128Cool || mode == kDaikin128Heat))
        setFan(kDaikin128FanPowerful);
    else if (_.Fan == kDaikin128FanPowerful)
        setFan(kDaikin128FanAuto);
}

void Ir_Daikin128::setEcono(const bool on) {
    uint8_t mode = _.Mode;
    _.Econo = (on && (mode == kDaikin128Cool || mode == kDaikin128Heat));
}

bool Ir_Daikin128::getEcono() const {
    return _.Econo;
}

void Ir_Daikin128::setOnTimer(const uint16_t on) {
    _.OnTimer = on;
}

uint16_t Ir_Daikin128::getOnTimer() const {
    return _.OnTimer;
}

bool Ir_Daikin128::getOnTimerEnabled() const {
    return _.OnTimer;
}

void Ir_Daikin128::setOnTimerEnabled(const bool on) {
    _.OnTimer = on;
}

void Ir_Daikin128::setOffTimer(const uint16_t mins_since_midnight) {
    SETTIME(Off, mins_since_midnight);
}

uint16_t Ir_Daikin128::getOffTimer() const {
    return GETTIME(Off);
}
bool Ir_Daikin128::getOffTimerEnabled() const {
    return _.OffTimer;
}

void Ir_Daikin128::setOffTimerEnabled(const bool on) {
    _.OffTimer = on;
}

void Ir_Daikin128::setClock(const uint16_t mins_since_midnight) {
    uint16_t mins = mins_since_midnight;
    if (mins_since_midnight >= 24 * 60) mins = 0;  // Bounds check.
    // Hours.
    _.ClockHours = uint8ToBcd(mins / 60);
    // Minutes.
    _.ClockMins = uint8ToBcd(mins % 60);
}

uint16_t Ir_Daikin128::getClock() const {
    return bcdToUint8(_.ClockHours) * 60 + bcdToUint8(_.ClockMins);
}

void Ir_Daikin128::setLightToggle(const uint8_t unit) {
    _.Ceiling = 0;
    _.Wall = 0;
    switch (unit) {
        case kDaikin128BitCeiling:
            _.Ceiling = 1;
            break;
        case kDaikin128BitWall:
            _.Wall = 1;
            break;
    }
}

uint8_t Ir_Daikin128::getLightToggle() const {
    uint8_t code = 0;
    if (_.Ceiling) {
        code = kDaikin128BitCeiling;
    } else if (_.Wall) {
        code = kDaikin128BitWall;
    }

    return code;
}

uint8_t * Ir_Daikin128::getRaw() {
    checksum();  // Ensure correct settings before sending.
    return _.raw;
}

void Ir_Daikin128::setRaw(const uint8_t *new_code) {
    memcpy(_.raw, new_code, kDaikin128StateLength);
}

bool Ir_Daikin128::validChecksum(uint8_t *state) {
    // Validate the checksum of section #1.
    if (state[kDaikin128SectionLength - 1] >> 4 != calcFirstChecksum(state))
        return false;
    // Validate the checksum of section #2
    if (state[kDaikin128StateLength - 1] != calcSecondChecksum(state))
        return false;
    return true;
}

void Ir_Daikin128::stateReset() {
    for (uint8_t i = 0; i < kDaikin128StateLength; i++) _.raw[i] = 0x00;
    _.raw[0] = 0x16;
    _.raw[7] = 0x04;  // Most significant nibble is a checksum.
    _.raw[8] = 0xA1;
    // _.raw[15] is a checksum byte, it will be set by checksum().
}

uint8_t Ir_Daikin128::calcFirstChecksum(const uint8_t *state) {
    return sumNibbles(state, kDaikin128SectionLength - 1,
                      state[kDaikin128SectionLength - 1] & 0x0F) & 0x0F;
}

uint8_t Ir_Daikin128::calcSecondChecksum(const uint8_t *state) {
    return sumNibbles(state + kDaikin128SectionLength,
                      kDaikin128SectionLength - 1);
}

void Ir_Daikin128::checksum() {
    _.Sum1 = calcFirstChecksum(_.raw);
    _.Sum2 = calcSecondChecksum(_.raw);
}
/***********************************************************************************************************************
 * End of class Ir_Daikin128
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * Start of class Ir_Daikin160
 ***********************************************************************************************************************/
Ir_Daikin160::Ir_Daikin160(uint32_t frequency, uint16_t dutycycle): _ir_daikin160_send(frequency, dutycycle){
    stateReset();
}

Ir_Daikin160::~Ir_Daikin160() {

}

void Ir_Daikin160::sendDaikin160(const uint16_t repeat) {
    send(getRaw(), kDaikin160StateLength, repeat);
}

void Ir_Daikin160::send(const unsigned char *data, const uint16_t nbytes, const uint16_t repeat) {
    if (nbytes < kDaikin160Section1Length)
        return;  // Not enough bytes to send a partial message.

    for (uint16_t r = 0; r <= repeat; r++) {
        // Section #1
        _ir_daikin160_send.sendGeneric(kDaikin160HdrMark, kDaikin160HdrSpace, kDaikin160BitMark,
                    kDaikin160OneSpace, kDaikin160BitMark, kDaikin160ZeroSpace,
                    kDaikin160BitMark, kDaikin160Gap, data,
                    kDaikin160Section1Length,
                    kDaikin160Freq, false, 0, 50);
        // Section #2
        _ir_daikin160_send.sendGeneric(kDaikin160HdrMark, kDaikin160HdrSpace, kDaikin160BitMark,
                    kDaikin160OneSpace, kDaikin160BitMark, kDaikin160ZeroSpace,
                    kDaikin160BitMark, kDaikin160Gap,
                    data + kDaikin160Section1Length,
                    nbytes - kDaikin160Section1Length,
                    kDaikin160Freq, false, 0, 50);
    }
}
void Ir_Daikin160::setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp) {
    setPower(power);
    setMode(convertMode(stdAc::opmode_t(mode)));
    setFan(convertFan(stdAc::fanspeed_t(speed)));
    setTemp(temp);
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t Ir_Daikin160::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
	case stdAc::opmode_t::kCool: return kDaikinCool;
	case stdAc::opmode_t::kHeat: return kDaikinHeat;
	case stdAc::opmode_t::kDry: return kDaikinDry;
	case stdAc::opmode_t::kFan: return kDaikinFan;
	default: return kDaikinAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t Ir_Daikin160::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin: return kDaikinFanMin;
    case stdAc::fanspeed_t::kLow: return kDaikinFanMin + 1;
    case stdAc::fanspeed_t::kMedium: return kDaikinFanMin + 2;
    case stdAc::fanspeed_t::kHigh: return kDaikinFanMax - 1;
    case stdAc::fanspeed_t::kMax: return kDaikinFanMax;
    default:
      return kDaikinFanAuto;
  }
}

uint8_t * Ir_Daikin160::getRaw() {
    checksum();  // Ensure correct settings before sending.
    return _.raw;
}

void Ir_Daikin160::setRaw(const uint8_t *new_code) {
    memcpy(_.raw, new_code, kDaikin160StateLength);
}

bool Ir_Daikin160::validChecksum(uint8_t *state, const uint16_t length) {
    // Validate the checksum of section #1.
    if (length <= kDaikin160Section1Length - 1 ||
        state[kDaikin160Section1Length - 1] != sumBytes(
                state, kDaikin160Section1Length - 1))
        return false;
    // Validate the checksum of section #2 (a.k.a. the rest)
    if (length <= kDaikin160Section1Length + 1 ||
        state[length - 1] != sumBytes(state + kDaikin160Section1Length,
                                      length - kDaikin160Section1Length - 1))
        return false;
    return true;
}

void Ir_Daikin160::on() {
    setPower(true);
}

void Ir_Daikin160::off() {
    setPower(false);
}

void Ir_Daikin160::setPower(const bool on) {
    _.Power = on;
}

bool Ir_Daikin160::getPower() const {
    return _.Power;
}

void Ir_Daikin160::setTemp(const uint8_t temp) {
    uint8_t degrees = std::max(temp, kDaikinMinTemp);
    degrees = std::min(degrees, kDaikinMaxTemp) - 10;
    _.Temp = degrees;
}

uint8_t Ir_Daikin160::getTemp() const {
    return _.Temp + 10;
}

void Ir_Daikin160::setMode(const uint8_t mode) {
    switch (mode) {
        case kDaikinAuto:
        case kDaikinCool:
        case kDaikinHeat:
        case kDaikinFan:
        case kDaikinDry:
            _.Mode = mode;
            break;
        default: _.Mode = kDaikinAuto;
    }
}

uint8_t Ir_Daikin160::getMode() const {
    return _.Mode;
}

void Ir_Daikin160::setFan(const uint8_t fan) {
    uint8_t fanset;
    if (fan == kDaikinFanQuiet || fan == kDaikinFanAuto)
        fanset = fan;
    else if (fan < kDaikinFanMin || fan > kDaikinFanMax)
        fanset = kDaikinFanAuto;
    else
        fanset = 2 + fan;
    _.Fan = fanset;
}

uint8_t Ir_Daikin160::getFan() const {
    uint8_t fan = _.Fan;
    if (fan != kDaikinFanQuiet && fan != kDaikinFanAuto) fan -= 2;
    return fan;
}

void Ir_Daikin160::setSwingVertical(const uint8_t position) {
    switch (position) {
        case kDaikin160SwingVLowest:
        case kDaikin160SwingVLow:
        case kDaikin160SwingVMiddle:
        case kDaikin160SwingVHigh:
        case kDaikin160SwingVHighest:
        case kDaikin160SwingVAuto:
            _.SwingV = position;
            break;
        default: _.SwingV = kDaikin160SwingVAuto;
    }
}

uint8_t Ir_Daikin160::getSwingVertical() const {
    return _.SwingV;
}

void Ir_Daikin160::stateReset() {
    for (uint8_t i = 0; i < kDaikin160StateLength; i++) _.raw[i] = 0x00;
    _.raw[0] =  0x11;
    _.raw[1] =  0xDA;
    _.raw[2] =  0x27;
    _.raw[3] =  0xF0;
    _.raw[4] =  0x0D;
    // _.raw[6] is a checksum byte, it will be set by checksum().
    _.raw[7] =  0x11;
    _.raw[8] =  0xDA;
    _.raw[9] =  0x27;
    _.raw[11] = 0xD3;
    _.raw[12] = 0x30;
    _.raw[13] = 0x11;
    _.raw[16] = 0x1E;
    _.raw[17] = 0x0A;
    _.raw[18] = 0x08;
    // _.raw[19] is a checksum byte, it will be set by checksum().
}

void Ir_Daikin160::checksum() {
    _.Sum1 = sumBytes(_.raw, kDaikin160Section1Length - 1);
    _.Sum2 = sumBytes(_.raw + kDaikin160Section1Length,
                      kDaikin160Section2Length - 1);
}
/***********************************************************************************************************************
 * End of class Ir_Daikin160
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Start of class Ir_Daikin216
 ***********************************************************************************************************************/

Ir_Daikin216::Ir_Daikin216(uint32_t frequency, uint16_t dutycycle) : _ir_daikin216(frequency, dutycycle){
    stateReset();
}

Ir_Daikin216::~Ir_Daikin216() {

}

void Ir_Daikin216::sendDaikin216(const uint16_t repeat) {
    send(getRaw(), kDaikin216StateLength, repeat);
}

void Ir_Daikin216::send(const unsigned char *data, const uint16_t nbytes, const uint16_t repeat) {
    if (nbytes < kDaikin216Section1Length)
        return;  // Not enough bytes to send a partial message.
    for (uint16_t r = 0; r <= repeat; r++) {
        // Section #1
        _ir_daikin216.sendGeneric(kDaikin216HdrMark, kDaikin216HdrSpace, kDaikin216BitMark,
                    kDaikin216OneSpace, kDaikin216BitMark, kDaikin216ZeroSpace,
                    kDaikin216BitMark, kDaikin216Gap, data,
                    kDaikin216Section1Length,
                    kDaikin216Freq, false, 0, 50);
        // Section #2
        _ir_daikin216.sendGeneric(kDaikin216HdrMark, kDaikin216HdrSpace, kDaikin216BitMark,
                    kDaikin216OneSpace, kDaikin216BitMark, kDaikin216ZeroSpace,
                    kDaikin216BitMark, kDaikin216Gap,
                    data + kDaikin216Section1Length,
                    nbytes - kDaikin216Section1Length,
                    kDaikin216Freq, false, 0, 50);
    }
}

void Ir_Daikin216::setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp) {
    setPower(power);
    setMode(convertMode(stdAc::opmode_t(mode)));
    setFan(convertFan(stdAc::fanspeed_t(speed)));
    setTemp(temp);
}
/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t Ir_Daikin216::convertMode(const stdAc::opmode_t mode) {
	switch (mode) {
		case stdAc::opmode_t::kCool: return kDaikinCool;
		case stdAc::opmode_t::kHeat: return kDaikinHeat;
		case stdAc::opmode_t::kDry: return kDaikinDry;
		case stdAc::opmode_t::kFan: return kDaikinFan;
		default: return kDaikinAuto;
	}
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t Ir_Daikin216::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
	case stdAc::fanspeed_t::kMin: return kDaikinFanQuiet;
	case stdAc::fanspeed_t::kLow: return kDaikinFanMin;
	case stdAc::fanspeed_t::kMedium: return kDaikinFanMed;
	case stdAc::fanspeed_t::kHigh: return kDaikinFanMax - 1;
	case stdAc::fanspeed_t::kMax: return kDaikinFanMax;
	default: return kDaikinFanAuto;
  }
}

uint8_t * Ir_Daikin216::getRaw() {
    checksum();  // Ensure correct settings before sending.
    return _.raw;
}

void Ir_Daikin216::setRaw(const uint8_t *new_code) {
    memcpy(_.raw, new_code, kDaikin216StateLength);
}

bool Ir_Daikin216::validChecksum(uint8_t *state, const uint16_t length) {
    // Validate the checksum of section #1.
    if (length <= kDaikin216Section1Length - 1 ||
        state[kDaikin216Section1Length - 1] != sumBytes(
                state, kDaikin216Section1Length - 1))
        return false;
    // Validate the checksum of section #2 (a.k.a. the rest)
    if (length <= kDaikin216Section1Length + 1 ||
        state[length - 1] != sumBytes(state + kDaikin216Section1Length,
                                      length - kDaikin216Section1Length - 1))
        return false;
    return true;
}

void Ir_Daikin216::on() {
    setPower(true);
}

void Ir_Daikin216::off() {
    setPower(false);
}

void Ir_Daikin216::setPower(const bool on) {
    _.Power = on;
}

bool Ir_Daikin216::getPower() const {
    return _.Power;
}

void Ir_Daikin216::setTemp(const uint8_t temp) {
    uint8_t degrees = std::max(temp, kDaikinMinTemp);
    degrees = std::min(degrees, kDaikinMaxTemp);
    _.Temp = degrees;
}

uint8_t Ir_Daikin216::getTemp() const {
    return _.Temp;
}

void Ir_Daikin216::setMode(const uint8_t mode) {
    switch (mode) {
        case kDaikinAuto:
        case kDaikinCool:
        case kDaikinHeat:
        case kDaikinFan:
        case kDaikinDry:
            _.Mode = mode;
            break;
        default:
            _.Mode = kDaikinAuto;
    }
}

uint8_t Ir_Daikin216::getMode() const {
    return _.Mode;
}

void Ir_Daikin216::setFan(const uint8_t fan) {
    // Set the fan speed bits, leave low 4 bits alone
    uint8_t fanset;
    if (fan == kDaikinFanQuiet || fan == kDaikinFanAuto)
        fanset = fan;
    else if (fan < kDaikinFanMin || fan > kDaikinFanMax)
        fanset = kDaikinFanAuto;
    else
        fanset = 2 + fan;
    _.Fan = fanset;
}

uint8_t Ir_Daikin216::getFan() const {
    uint8_t fan = _.Fan;
    if (fan != kDaikinFanQuiet && fan != kDaikinFanAuto) fan -= 2;
    return fan;
}
void Ir_Daikin216::setSwingVertical(const bool on) {
    _.SwingV = (on ? kDaikin216SwingOn : kDaikin216SwingOff);
}

bool Ir_Daikin216::getSwingVertical() const {
    return _.SwingV;
}

void Ir_Daikin216::setSwingHorizontal(const bool on) {
    _.SwingH = (on ? kDaikin216SwingOn : kDaikin216SwingOff);
}

bool Ir_Daikin216::getSwingHorizontal() const {
    return _.SwingH;
}
void Ir_Daikin216::setQuiet(const bool on) {
    if (on) {
        setFan(kDaikinFanQuiet);
        // Powerful & Quiet mode being on are mutually exclusive.
        setPowerful(false);
    } else if (getFan() == kDaikinFanQuiet) {
        setFan(kDaikinFanAuto);
    }
}

bool Ir_Daikin216::getQuiet() const {
    return getFan() == kDaikinFanQuiet;
}

void Ir_Daikin216::setPowerful(const bool on) {
    _.Powerful = on;
    // Powerful & Quiet mode being on are mutually exclusive.
    if (on) setQuiet(false);
}

bool Ir_Daikin216::getPowerful() const {
    return _.Powerful;
}

void Ir_Daikin216::stateReset() {
    for (uint8_t i = 0; i < kDaikin216StateLength; i++) _.raw[i] = 0x00;
    _.raw[0] =  0x11;
    _.raw[1] =  0xDA;
    _.raw[2] =  0x27;
    _.raw[3] =  0xF0;
    // _.raw[7] is a checksum byte, it will be set by checksum().
    _.raw[8] =  0x11;
    _.raw[9] =  0xDA;
    _.raw[10] = 0x27;
    _.raw[23] = 0xC0;
    // _.raw[26] is a checksum byte, it will be set by checksum().
}

void Ir_Daikin216::checksum() {
    _.Sum1 = sumBytes(_.raw, kDaikin216Section1Length - 1);
    _.Sum2 = sumBytes(_.raw + kDaikin216Section1Length,
                      kDaikin216Section2Length - 1);
}
/***********************************************************************************************************************
 * End of class Ir_Daikin160
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * Start of class Ir_Daikin2
 ***********************************************************************************************************************/
Ir_Daikin2::Ir_Daikin2(uint32_t frequency, uint16_t dutycycle):_ir_daikin2(frequency, dutycycle) {
    stateReset();
}

Ir_Daikin2::~Ir_Daikin2() {

}

void Ir_Daikin2::sendDaikin2(const uint16_t repeat) {
    send(getRaw(), kDaikin2StateLength, repeat);
}

void Ir_Daikin2::setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp) {
    setPower(power);
    setMode(convertMode(stdAc::opmode_t(mode)));
    setFan(convertFan(stdAc::fanspeed_t(speed)));
    setTemp(temp);
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t Ir_Daikin2::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
	case stdAc::opmode_t::kCool: return kDaikinCool;
	case stdAc::opmode_t::kHeat: return kDaikinHeat;
	case stdAc::opmode_t::kDry: return kDaikinDry;
	case stdAc::opmode_t::kFan: return kDaikinFan;
	default: return kDaikinAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t Ir_Daikin2::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
	case stdAc::fanspeed_t::kMin: return kDaikinFanQuiet;
	case stdAc::fanspeed_t::kLow: return kDaikinFanMin;
	case stdAc::fanspeed_t::kMedium: return kDaikinFanMed;
	case stdAc::fanspeed_t::kHigh: return kDaikinFanMax - 1;
	case stdAc::fanspeed_t::kMax: return kDaikinFanMax;
	default: return kDaikinFanAuto;
  }
}

void Ir_Daikin2::send(const unsigned char *data, const uint16_t nbytes, const uint16_t repeat) {
    if (nbytes < kDaikin2Section1Length)
        return;  // Not enough bytes to send a partial message.

    for (uint16_t r = 0; r <= repeat; r++) {
        // Leader
        _ir_daikin2.sendGeneric(kDaikin2LeaderMark, kDaikin2LeaderSpace,
                    0, 0, 0, 0, 0, 0, (uint64_t) 0,  // No data payload.
                    0, kDaikin2Freq, false, 0, 50);
        // Section #1
        _ir_daikin2.sendGeneric(kDaikin2HdrMark, kDaikin2HdrSpace, kDaikin2BitMark,
                    kDaikin2OneSpace, kDaikin2BitMark, kDaikin2ZeroSpace,
                    kDaikin2BitMark, kDaikin2Gap, data, kDaikin2Section1Length,
                    kDaikin2Freq, false, 0, 50);
        // Section #2
        _ir_daikin2.sendGeneric(kDaikin2HdrMark, kDaikin2HdrSpace, kDaikin2BitMark,
                    kDaikin2OneSpace, kDaikin2BitMark, kDaikin2ZeroSpace,
                    kDaikin2BitMark, kDaikin2Gap, data + kDaikin2Section1Length,
                    nbytes - kDaikin2Section1Length,
                    kDaikin2Freq, false, 0, 50);
    }
}

void Ir_Daikin2::on() {
    setPower(true);
}

void Ir_Daikin2::off() {
    setPower(false);
}

void Ir_Daikin2::setPower(const bool on) {
    _.Power = on;
    _.Power2 = !on;
}

bool Ir_Daikin2::getPower() const {
    return _.Power && !_.Power2;
}

void Ir_Daikin2::setTemp(const uint8_t desired) {
    // The A/C has a different min temp if in cool mode.
    uint8_t temp = std::max(
            (_.Mode == kDaikinCool) ? kDaikin2MinCoolTemp : kDaikinMinTemp,
            desired);
    _.Temp = std::min(kDaikinMaxTemp, temp);
}

uint8_t Ir_Daikin2::getTemp() const {
    return _.Temp;
}

void Ir_Daikin2::setFan(const uint8_t fan) {
    uint8_t fanset;
    if (fan == kDaikinFanQuiet || fan == kDaikinFanAuto)
        fanset = fan;
    else if (fan < kDaikinFanMin || fan > kDaikinFanMax)
        fanset = kDaikinFanAuto;
    else
        fanset = 2 + fan;
    _.Fan = fanset;
}

uint8_t Ir_Daikin2::getFan() const {
    const uint8_t fan = _.Fan;
    switch (fan) {
        case kDaikinFanAuto:
        case kDaikinFanQuiet: return fan;
        default: return fan - 2;
    }
}

uint8_t Ir_Daikin2::getMode() const {
    return _.Mode;
}

void Ir_Daikin2::setMode(const uint8_t desired_mode) {
    uint8_t mode = desired_mode;
    switch (mode) {
        case kDaikinCool:
        case kDaikinHeat:
        case kDaikinFan:
        case kDaikinDry: break;
        default: mode = kDaikinAuto;
    }
    _.Mode = mode;
    // Redo the temp setting as Cool mode has a different min temp.
    if (mode == kDaikinCool) setTemp(getTemp());
}

void Ir_Daikin2::setSwingVertical(const uint8_t position) {
    switch (position) {
        case kDaikin2SwingVHigh:
        case 2:
        case 3:
        case 4:
        case 5:
        case kDaikin2SwingVLow:
        case kDaikin2SwingVSwing:
        case kDaikin2SwingVBreeze:
        case kDaikin2SwingVCirculate:
        case kDaikin2SwingVAuto:
            _.SwingV = position;
    }
}

uint8_t Ir_Daikin2::getSwingVertical() const {
    return _.SwingV;
}

void Ir_Daikin2::setSwingHorizontal(const uint8_t position) {
    _.SwingH = position;
}

uint8_t Ir_Daikin2::getSwingHorizontal() const {
    return _.SwingH;
}

bool Ir_Daikin2::getQuiet() const {
    return _.Quiet;
}

void Ir_Daikin2::setQuiet(const bool on) {
    _.Quiet = on;
    // Powerful & Quiet mode being on are mutually exclusive.
    if (on) setPowerful(false);
}

bool Ir_Daikin2::getPowerful() const {
    return _.Powerful;
}

void Ir_Daikin2::setPowerful(const bool on) {
    _.Powerful = on;
    // Powerful & Quiet mode being on are mutually exclusive.
    if (on) setQuiet(false);
}

void Ir_Daikin2::setEcono(const bool on) {
    _.Econo = on;
}

bool Ir_Daikin2::getEcono() const {
    return _.Econo;
}

void Ir_Daikin2::setEye(const bool on) {
    _.EyeAuto = on;
}

bool Ir_Daikin2::getEye() const {
    return _.Eye;
}

void Ir_Daikin2::setEyeAuto(const bool on) {
    _.EyeAuto = on;
}

bool Ir_Daikin2::getEyeAuto() const {
    return _.EyeAuto;
}

void Ir_Daikin2::setPurify(const bool on) {
    _.Purify = on;
}

bool Ir_Daikin2::getPurify() const {
    return _.Purify;
}

void Ir_Daikin2::setMold(const bool on) {
    _.Mold = on;
}

bool Ir_Daikin2::getMold() const {
    return _.Mold;
}

void Ir_Daikin2::enableOnTimer(const uint16_t starttime) {
    clearSleepTimerFlag();
    _.OnTimer = true;
    _.OnTime = starttime;
}

void Ir_Daikin2::disableOnTimer() {
    _.OnTime = kDaikinUnusedTime;
    clearOnTimerFlag();
    clearSleepTimerFlag();
}

uint16_t Ir_Daikin2::getOnTime() const {
    return _.OnTime;
}

bool Ir_Daikin2::getOnTimerEnabled() const {
    return _.OnTimer;
}

void Ir_Daikin2::enableSleepTimer(const uint16_t sleeptime) {
    enableOnTimer(sleeptime);
    clearOnTimerFlag();
    _.SleepTimer = true;
}
void Ir_Daikin2::disableSleepTimer() {
    disableOnTimer();
}
uint16_t Ir_Daikin2::getSleepTime() const {
    return getOnTime();
}
bool Ir_Daikin2::getSleepTimerEnabled() const {
    return _.SleepTimer;
}
void Ir_Daikin2::enableOffTimer(const uint16_t endtime) {
    // Set the Off Timer flag.
    _.OffTimer = true;
    _.OffTime = endtime;
}
void Ir_Daikin2::disableOffTimer() {
    _.OffTime = kDaikinUnusedTime;
    // Clear the Off Timer flag.
    _.OffTimer = false;
}

uint16_t Ir_Daikin2::getOffTime() const {
    return _.OffTime;
}

bool Ir_Daikin2::getOffTimerEnabled() const {
    return _.OffTimer;
}

void Ir_Daikin2::setCurrentTime(const uint16_t numMins) {
    uint16_t mins = numMins;
    if (numMins > 24 * 60) mins = 0;  // If > 23:59, set to 00:00
    _.CurrentTime = mins;
}

uint16_t Ir_Daikin2::getCurrentTime() const {
    return _.CurrentTime;
}

void Ir_Daikin2::setBeep(const uint8_t beep) {
    _.Beep = beep;
}

uint8_t Ir_Daikin2::getBeep() const {
    return _.Beep;
}

void Ir_Daikin2::setLight(const uint8_t light) {
    _.Light = light;
}

uint8_t Ir_Daikin2::getLight() const {
    return _.Light;
}

void Ir_Daikin2::setClean(const bool on) {
    _.Clean = on;
}

bool Ir_Daikin2::getClean() const {
    return _.Clean;
}

void Ir_Daikin2::setFreshAir(const bool on) {
    _.FreshAir = on;
}

bool Ir_Daikin2::getFreshAir() const {
    return _.FreshAir;
}

void Ir_Daikin2::setFreshAirHigh(const bool on) {
    _.FreshAirHigh = on;
}

bool Ir_Daikin2::getFreshAirHigh() const {
    return _.FreshAirHigh;
}

uint8_t * Ir_Daikin2::getRaw() {
    checksum();  // Ensure correct settings before sending.
    return _.raw;
}

void Ir_Daikin2::setRaw(const uint8_t *new_code) {
    memcpy(_.raw, new_code, kDaikin2StateLength);
}

bool Ir_Daikin2::validChecksum(uint8_t state[], const uint16_t length) {
    // Validate the checksum of section #1.
    if (length <= kDaikin2Section1Length - 1 ||
        state[kDaikin2Section1Length - 1] != sumBytes(state,
                                                      kDaikin2Section1Length - 1))
        return false;
    // Validate the checksum of section #2 (a.k.a. the rest)
    if (length <= kDaikin2Section1Length + 1 ||
        state[length - 1] != sumBytes(state + kDaikin2Section1Length,
                                      length - kDaikin2Section1Length - 1))
        return false;
    return true;
}

void Ir_Daikin2::stateReset() {
    for (uint8_t i = 0; i < kDaikin2StateLength; i++) _.raw[i] = 0x0;

    _.raw[0] = 0x11;
    _.raw[1] = 0xDA;
    _.raw[2] = 0x27;
    _.raw[4] = 0x01;
    _.raw[6] = 0xC0;
    _.raw[7] = 0x70;
    _.raw[8] = 0x08;
    _.raw[9] = 0x0C;
    _.raw[10] = 0x80;
    _.raw[11] = 0x04;
    _.raw[12] = 0xB0;
    _.raw[13] = 0x16;
    _.raw[14] = 0x24;
    _.raw[17] = 0xBE;
    _.raw[18] = 0xD0;
    // _.raw[19] is a checksum byte, it will be set by checksum().
    _.raw[20] = 0x11;
    _.raw[21] = 0xDA;
    _.raw[22] = 0x27;
    _.raw[25] = 0x08;
    _.raw[28] = 0xA0;
    _.raw[35] = 0xC1;
    _.raw[36] = 0x80;
    _.raw[37] = 0x60;
    // _.raw[38] is a checksum byte, it will be set by checksum().
    disableOnTimer();
    disableOffTimer();
    disableSleepTimer();
    checksum();
}

void Ir_Daikin2::checksum() {
    _.Sum1 = sumBytes(_.raw, kDaikin2Section1Length - 1);
    _.Sum2 = sumBytes(_.raw + kDaikin2Section1Length, kDaikin2Section2Length - 1);
}

void Ir_Daikin2::clearOnTimerFlag() {
    _.OnTimer = false;
}

void Ir_Daikin2::clearSleepTimerFlag() {
    _.SleepTimer = false;
}

/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/


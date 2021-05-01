/*
 * IrGoodweather.cpp
 *
 *  Created on: Jan 19, 2021
 *      Author: ductu
 */

#include "IrGoodweather.h"
#include "IRutils.h"
/***********************************************************************************************************************
 * Pragma directive
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes <System Includes>
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/
using std::max;
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
Ir_Goodweather::Ir_Goodweather(uint32_t frequency, uint16_t dutycycle): _ir_goodweather_send(frequency, dutycycle){
	// TODO Auto-generated constructor stub
	stateReset();
}

Ir_Goodweather::~Ir_Goodweather() {
	// TODO Auto-generated destructor stub
}

void Ir_Goodweather::sendGoodweather(const uint16_t repeat) {
    send(getRaw(), kGoodweatherBits, repeat);
}

void Ir_Goodweather::setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp) {
    setPower(power);
    setMode(convertMode(stdAc::opmode_t(mode)));
    setFan(convertFan(stdAc::fanspeed_t(speed)));
    setTemp(temp);
}
/***********************************************************************************************************************
 * Static Functions
 ***********************************************************************************************************************/
/// Reset the internal state to a fixed known good state.
void Ir_Goodweather::stateReset(void) { _.raw = kGoodweatherStateInit; }

void Ir_Goodweather::send(const uint64_t data, const uint16_t nbits, const uint16_t repeat) {
    if (nbits != kGoodweatherBits)
        return;  // Wrong nr. of bits to send a proper message.
    // Set IR carrier frequency
//    enableIROut(38);
    for (uint16_t r = 0; r <= repeat; r++) {
        // Header
        _ir_goodweather_send.mark_space(kGoodweatherHdrMark, kGoodweatherHdrSpace);
//        mark(kGoodweatherHdrMark);
//        space(kGoodweatherHdrSpace);
        // Data
        for (int16_t i = 0; i < nbits; i += 8) {
            uint16_t chunk = (data >> i) & 0xFF;  // Grab a byte at a time.
            chunk = (~chunk) << 8 | chunk;  // Prepend a inverted copy of the byte.
            _ir_goodweather_send.sendData(kGoodweatherBitMark, kGoodweatherOneSpace,
                     kGoodweatherBitMark, kGoodweatherZeroSpace,
                     chunk, 16, false);
        }
        // Footer
        _ir_goodweather_send.mark_space(kGoodweatherBitMark, kGoodweatherHdrSpace);
        _ir_goodweather_send.mark_space(kGoodweatherBitMark, kDefaultMessageGap);
//        mark(kGoodweatherBitMark);
//        space(kGoodweatherHdrSpace);
//        mark(kGoodweatherBitMark);
//        space(kDefaultMessageGap);
    }
}
/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t Ir_Goodweather::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kGoodweatherCool;
    case stdAc::opmode_t::kHeat: return kGoodweatherHeat;
    case stdAc::opmode_t::kDry:  return kGoodweatherDry;
    case stdAc::opmode_t::kFan:  return kGoodweatherFan;
    default:                     return kGoodweatherAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t Ir_Goodweather::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:    return kGoodweatherFanLow;
    case stdAc::fanspeed_t::kMedium: return kGoodweatherFanMed;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:    return kGoodweatherFanHigh;
    default:                         return kGoodweatherFanAuto;
  }
}

void Ir_Goodweather::on() {
    setPower(true);
}

void Ir_Goodweather::off() {
    setPower(false);
}

void Ir_Goodweather::setPower(const bool on) {
    _.Command = kGoodweatherCmdPower;
    _.Power = on;
}

bool Ir_Goodweather::getPower() const {
    return _.Power;
}

void Ir_Goodweather::setTemp(const uint8_t temp) {
    uint8_t new_temp = std::max(kGoodweatherTempMin, temp);
    new_temp = std::min(kGoodweatherTempMax, new_temp);
    if (new_temp > getTemp()) _.Command = kGoodweatherCmdUpTemp;
    if (new_temp < getTemp()) _.Command = kGoodweatherCmdDownTemp;
    _.Temp = new_temp - kGoodweatherTempMin;
}

uint8_t Ir_Goodweather::getTemp() const {
    return _.Temp + kGoodweatherTempMin;
}

void Ir_Goodweather::setFan(const uint8_t speed) {
    _.Command = kGoodweatherCmdFan;
    switch (speed) {
        case kGoodweatherFanAuto:
        case kGoodweatherFanLow:
        case kGoodweatherFanMed:
        case kGoodweatherFanHigh:
            _.Fan = speed;
            break;
        default:
            _.Fan = kGoodweatherFanAuto;
    }
}

uint8_t Ir_Goodweather::getFan() const {
    return _.Fan;
}

void Ir_Goodweather::setMode(const uint8_t mode) {
    _.Command = kGoodweatherCmdMode;
    switch (mode) {
        case kGoodweatherAuto:
        case kGoodweatherDry:
        case kGoodweatherCool:
        case kGoodweatherFan:
        case kGoodweatherHeat:
            _.Mode = mode;
            break;
        default:
            _.Mode = kGoodweatherAuto;
    }
}

uint8_t Ir_Goodweather::getMode() const {
    return _.Mode;
}

void Ir_Goodweather::setSwing(const uint8_t speed) {
    _.Command = kGoodweatherCmdSwing;
    switch (speed) {
        case kGoodweatherSwingOff:
        case kGoodweatherSwingSlow:
        case kGoodweatherSwingFast:
            _.Swing = speed;
            break;
        default:
            _.Swing = kGoodweatherSwingOff;
    }
}

uint8_t Ir_Goodweather::getSwing() const {
    return _.Swing;
}

void Ir_Goodweather::setSleep(const bool toggle) {
    _.Command = kGoodweatherCmdSleep;
    _.Sleep = toggle;
}

bool Ir_Goodweather::getSleep() const {
    return _.Sleep;
}

void Ir_Goodweather::setTurbo(const bool toggle) {
    _.Command = kGoodweatherCmdTurbo;
    _.Turbo = toggle;
}

bool Ir_Goodweather::getTurbo() const {
    return _.Turbo;
}

void Ir_Goodweather::setLight(const bool toggle) {
    _.Command = kGoodweatherCmdLight;
    _.Light = toggle;
}

bool Ir_Goodweather::getLight() const {
    return _.Light;
}

void Ir_Goodweather::setCommand(const uint8_t cmd) {
    if (cmd <= kGoodweatherCmdLight)
        _.Command = cmd;
}

uint8_t Ir_Goodweather::getCommand() const {
    return _.Command;
}

uint64_t Ir_Goodweather::getRaw() {
    return _.raw;
}

void Ir_Goodweather::setRaw(const uint64_t state) {
    _.raw  = state;
}

/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/


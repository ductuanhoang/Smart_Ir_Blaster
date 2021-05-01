/*
 * IrAmcor.cpp
 *
 *  Created on: Jan 14, 2021
 *      Author: ductu
 */



/***********************************************************************************************************************
 * Pragma directive
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes <System Includes>
 ***********************************************************************************************************************/
#include "IrAmcor.h"
#include "IRutils.h"

/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/
const uint16_t kAmcorHdrMark = 8200;
const uint16_t kAmcorHdrSpace = 4200;
const uint16_t kAmcorOneMark = 1500;
const uint16_t kAmcorZeroMark = 600;
const uint16_t kAmcorOneSpace = kAmcorZeroMark;
const uint16_t kAmcorZeroSpace = kAmcorOneMark;
const uint16_t kAmcorFooterMark = 1900;
const uint16_t kAmcorGap = 34300;
const uint8_t  kAmcorTolerance = 40;

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
Ir_Amcor::Ir_Amcor(uint32_t frequency, uint16_t dutycycle): _ir_amcor_send(frequency, dutycycle)
{
	// TODO Auto-generated constructor stub
	stateReset();
}

Ir_Amcor::~Ir_Amcor() {
	// TODO Auto-generated destructor stub
}

void Ir_Amcor::sendAmcor(const uint16_t repeat) {
    send(getRaw(), kAmcorStateLength, repeat);
}

void Ir_Amcor::setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp) {
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
uint8_t Ir_Amcor::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool:
      return kAmcorCool;
    case stdAc::opmode_t::kHeat:
      return kAmcorHeat;
    case stdAc::opmode_t::kDry:
      return kAmcorDry;
    case stdAc::opmode_t::kFan:
      return kAmcorFan;
    default:
      return kAmcorAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t Ir_Amcor::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:
      return kAmcorFanMin;
    case stdAc::fanspeed_t::kMedium:
      return kAmcorFanMed;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:
      return kAmcorFanMax;
    default:
      return kAmcorFanAuto;
  }
}

void Ir_Amcor::send(const unsigned char *data, const uint16_t nbytes, const uint16_t repeat) {
    // Check if we have enough bytes to send a proper message.
    if (nbytes < kAmcorStateLength) return;
    _ir_amcor_send.sendGeneric(kAmcorHdrMark, kAmcorHdrSpace, kAmcorOneMark, kAmcorOneSpace,
                kAmcorZeroMark, kAmcorZeroSpace, kAmcorFooterMark, kAmcorGap,
                data, nbytes, 38, false, repeat, 50);
}

void Ir_Amcor::stateReset() {
    for (uint8_t i = 1; i < kAmcorStateLength; i++) _.raw[i] = 0x0;
    _.raw[0] = 0x01;
    _.Fan = kAmcorFanAuto;
    _.Mode = kAmcorAuto;
    _.Temp = 25;  // 25C
}

uint8_t Ir_Amcor::calcChecksum(const uint8_t *state, const uint16_t length) {
    return irutils::sumNibbles(state, length - 1);
}

bool Ir_Amcor::validChecksum(const uint8_t *state, const uint16_t length) {
    return (state[length - 1] == calcChecksum(state, length));
}

void Ir_Amcor::setPower(const bool on) {
    _.Power = (on ? kAmcorPowerOn : kAmcorPowerOff);
}

bool Ir_Amcor::getPower() const {
    return _.Power == kAmcorPowerOn;
}

void Ir_Amcor::on() {
    setPower(true);
}

void Ir_Amcor::off() {
    setPower(false);
}
void Ir_Amcor::setTemp(const uint8_t degrees) {
    uint8_t temp = std::max(kAmcorMinTemp, degrees);
    temp = std::min(kAmcorMaxTemp, temp);
    _.Temp = temp;
}

uint8_t Ir_Amcor::getTemp() const {
    return _.Temp;
}

bool Ir_Amcor::getMax() const {
    return _.Max == kAmcorMax;
}

void Ir_Amcor::setMax(const bool on) {
    if (on) {
        switch (_.Mode) {
            case kAmcorCool: _.Temp = kAmcorMinTemp; break;
            case kAmcorHeat: _.Temp = kAmcorMaxTemp; break;
                // Not allowed in all other operating modes.
            default: return;
        }
    }
    _.Max = (on ? kAmcorMax : 0);
}
void Ir_Amcor::setFan(const uint8_t speed) {
    switch (speed) {
        case kAmcorFanAuto:
        case kAmcorFanMin:
        case kAmcorFanMed:
        case kAmcorFanMax:
            _.Fan = speed;
            break;
        default:
            _.Fan = kAmcorFanAuto;
    }
}

uint8_t Ir_Amcor::getFan() const {
    return _.Fan;
}

void Ir_Amcor::setMode(const uint8_t mode) {
    switch (mode) {
        case kAmcorFan:
        case kAmcorCool:
        case kAmcorHeat:
        case kAmcorDry:
        case kAmcorAuto:
            _.Vent = (mode == kAmcorFan) ? kAmcorVentOn : 0;
            _.Mode = mode;
            return;
        default:
            _.Vent = 0;
            _.Mode = kAmcorAuto;
            break;
    }
}

uint8_t Ir_Amcor::getMode() const {
    return _.Mode;
}

uint8_t * Ir_Amcor::getRaw() {
    checksum();  // Ensure correct bit array before returning
    return _.raw;
}

void Ir_Amcor::setRaw(const uint8_t *state) {
    memcpy(_.raw, state, kAmcorStateLength);
}

void Ir_Amcor::checksum() {
    _.Sum = calcChecksum(_.raw, kAmcorStateLength);
}
/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/


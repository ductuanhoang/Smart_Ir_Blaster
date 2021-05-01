/*
 * IrSanyo.cpp
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
#include "IrSanyo.h"
#include "IRutils.h"
#include "IrNEC.h"
/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/
using irutils::sumNibbles;
using irutils::setBit;
using irutils::setBits;
/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/
// Constants
// Sanyo SA 8650B
const uint16_t kSanyoSa8650bHdrMark = 3500;  // seen range 3500
const uint16_t kSanyoSa8650bHdrSpace = 950;  // seen 950
const uint16_t kSanyoSa8650bOneMark = 2400;  // seen 2400
const uint16_t kSanyoSa8650bZeroMark = 700;  // seen 700
// usually see 713 - not using ticks as get number wrapround
const uint16_t kSanyoSa8650bDoubleSpaceUsecs = 800;
const uint16_t kSanyoSa8650bRptLength = 45000;

// Sanyo LC7461
const uint16_t kSanyoLc7461AddressMask = (1 << kSanyoLC7461AddressBits) - 1;
const uint16_t kSanyoLc7461CommandMask = (1 << kSanyoLC7461CommandBits) - 1;
const uint16_t kSanyoLc7461HdrMark = 9000;
const uint16_t kSanyoLc7461HdrSpace = 4500;
const uint16_t kSanyoLc7461BitMark = 560;    // 1T
const uint16_t kSanyoLc7461OneSpace = 1690;  // 3T
const uint16_t kSanyoLc7461ZeroSpace = 560;  // 1T
const uint32_t kSanyoLc7461MinCommandLength = 108000;

const uint16_t kSanyoLc7461MinGap =
        kSanyoLc7461MinCommandLength -
        (kSanyoLc7461HdrMark + kSanyoLc7461HdrSpace +
         kSanyoLC7461Bits * (kSanyoLc7461BitMark +
                             (kSanyoLc7461OneSpace + kSanyoLc7461ZeroSpace) / 2) +
         kSanyoLc7461BitMark);

const uint16_t kSanyoAcHdrMark = 8500;   ///< uSeconds
const uint16_t kSanyoAcHdrSpace = 4200;  ///< uSeconds
const uint16_t kSanyoAcBitMark = 500;    ///< uSeconds
const uint16_t kSanyoAcOneSpace = 1600;  ///< uSeconds
const uint16_t kSanyoAcZeroSpace = 550;  ///< uSeconds
const uint32_t kSanyoAcGap = kDefaultMessageGap;  ///< uSeconds (Guess only)
const uint16_t kSanyoAcFreq = 38000;  ///< Hz. (Guess only)

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
IrSanyo::IrSanyo(uint32_t frequency, uint16_t dutycycle) : _ir_sanyo(frequency, dutycycle) {
	// TODO Auto-generated constructor stub
    stateReset();
}

IrSanyo::~IrSanyo() {
	// TODO Auto-generated destructor stub
}

void IrSanyo::sendSanYoAc(const uint16_t repeat) {
    send(getRaw(), kSanyoAcStateLength, repeat);
}

void IrSanyo::sendSanyoLC7461(const uint64_t data, const uint16_t nbits, const uint16_t repeat) {
    Ir_NEC _ir_NEC_remote(38000, 50);
    _ir_NEC_remote.sendNEC(data, nbits, repeat);
}

void IrSanyo::setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp) {
    setPower(power);
    setMode(convertMode(stdAc::opmode_t(mode)));
    setFan(convertFan(stdAc::fanspeed_t(speed)));
    setTemp(temp);
}
/***********************************************************************************************************************
 * Static Functions
 ***********************************************************************************************************************/
/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IrSanyo::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:    return kSanyoAcFanLow;
    case stdAc::fanspeed_t::kMedium: return kSanyoAcFanMedium;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:    return kSanyoAcFanHigh;
    default:                         return kSanyoAcFanAuto;
  }
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IrSanyo::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kSanyoAcCool;
    case stdAc::opmode_t::kHeat: return kSanyoAcHeat;
    case stdAc::opmode_t::kDry:  return kSanyoAcDry;
    default:                     return kSanyoAcAuto;
  }
}

void IrSanyo::send(const uint8_t *data, const uint16_t nbytes, const uint16_t repeat) {
    _ir_sanyo.sendGeneric(kSanyoAcHdrMark, kSanyoAcHdrSpace,
                kSanyoAcBitMark, kSanyoAcOneSpace,
                kSanyoAcBitMark, kSanyoAcZeroSpace,
                kSanyoAcBitMark, kSanyoAcGap,
                data, nbytes, kSanyoAcFreq, false, repeat, 50);
}

void IrSanyo::stateReset() {
    static const uint8_t kReset[kSanyoAcStateLength] = {
            0x6A, 0x6D, 0x51, 0x00, 0x10, 0x45, 0x00, 0x00, 0x33};
    memcpy(remote_state, kReset, kSanyoAcStateLength);
}

void IrSanyo::on() {
    setPower(true);
}

void IrSanyo::off() {
    setPower(false);
}

void IrSanyo::setPower(const bool on) {
    setBits(&remote_state[kSanyoAcPowerByte], kSanyoAcPowerOffset,
            kSanyoAcPowerSize, on ? kSanyoAcPowerOn : kSanyoAcPowerOff);
}

bool IrSanyo::getPower() {
    return GETBITS8(remote_state[kSanyoAcPowerByte], kSanyoAcPowerOffset,
                    kSanyoAcPowerSize) == kSanyoAcPowerOn;
}

void IrSanyo::setTemp(const uint8_t degrees) {
    _setTemp(&remote_state[kSanyoAcTempByte], degrees);
}

uint8_t IrSanyo::getTemp() {
    return _getTemp(&remote_state[kSanyoAcTempByte]);
}

void IrSanyo::setSensorTemp(const uint8_t degrees) {
    _setTemp(&remote_state[kSanyoAcSensorByte], degrees);
}

uint8_t IrSanyo::getSensorTemp() {
    return _getTemp(&remote_state[kSanyoAcSensorByte]);
}

void IrSanyo::setFan(const uint8_t speed) {
    setBits(&remote_state[kSanyoAcModeByte], kSanyoAcFanOffset, kSanyoAcFanSize,
            speed);
}

uint8_t IrSanyo::getFan() {
    return GETBITS8(remote_state[kSanyoAcModeByte], kSanyoAcFanOffset,
                    kSanyoAcFanSize);
}

void IrSanyo::setMode(const uint8_t mode) {
    switch (mode) {
        case kSanyoAcAuto:
        case kSanyoAcCool:
        case kSanyoAcDry:
        case kSanyoAcHeat:
            setBits(&remote_state[kSanyoAcModeByte], kSanyoAcModeOffset,
                    kSanyoAcModeSize, mode);
            break;
        default: setMode(kSanyoAcAuto);
    }
}

uint8_t IrSanyo::getMode() {
    return GETBITS8(remote_state[kSanyoAcModeByte], kSanyoAcModeOffset,
                    kSanyoAcModeSize);
}

void IrSanyo::setSleep(const bool on) {
    setBit(&remote_state[kSanyoAcSleepByte], kSanyoAcSleepBit, on);
}

bool IrSanyo::getSleep() {
    return GETBIT8(remote_state[kSanyoAcSleepByte], kSanyoAcSleepBit);
}

void IrSanyo::setSensor(const bool location) {
    setBit(&remote_state[kSanyoAcSensorByte], kSanyoAcSensorBit, location);
}

bool IrSanyo::getSensor() {
    return GETBIT8(remote_state[kSanyoAcSensorByte], kSanyoAcSensorBit);
}

void IrSanyo::setBeep(const bool on) {
    setBit(&remote_state[kSanyoAcSensorByte], kSanyoAcBeepBit, on);
}
bool IrSanyo::getBeep() {
    return GETBIT8(remote_state[kSanyoAcSensorByte], kSanyoAcBeepBit);
}

void IrSanyo::setSwingV(const uint8_t setting) {
    if (setting == kSanyoAcSwingVAuto ||
        (setting >= kSanyoAcSwingVLowest && setting <= kSanyoAcSwingVHighest))
        setBits(&remote_state[kSanyoAcPowerByte], kSanyoAcSwingVOffset,
                kSanyoAcSwingVSize, setting);

    else
        setSwingV(kSanyoAcSwingVAuto);
}

uint8_t IrSanyo::getSwingV() {
    return GETBITS8(remote_state[kSanyoAcPowerByte], kSanyoAcSwingVOffset,
                    kSanyoAcSwingVSize);
}

void IrSanyo::setRaw(const uint8_t *newState) {
    memcpy(remote_state, newState, kSanyoAcStateLength);
}

uint8_t * IrSanyo::getRaw() {
    checksum();
    return remote_state;
}

uint16_t IrSanyo::getOffTimer() {
    if (GETBIT8(remote_state[kSanyoAcModeByte], kSanyoAcOffTimerEnableBit))
        return GETBITS8(remote_state[kSanyoAcOffHourByte], kSanyoAcOffHourOffset,
                        kSanyoAcOffHourSize) * 60;
    else
        return 0;
}

void IrSanyo::setOffTimer(const uint16_t mins) {
    const uint8_t hours = std::min((uint8_t)(mins / 60), kSanyoAcHourMax);
    setBit(&remote_state[kSanyoAcModeByte], kSanyoAcOffTimerEnableBit, hours > 0);
    setBits(&remote_state[kSanyoAcOffHourByte], kSanyoAcOffHourOffset,
            kSanyoAcOffHourSize, hours);
}

bool IrSanyo::validChecksum(const uint8_t *state, const uint16_t length) {
    return length && state[length - 1] == calcChecksum(state, length);
}

void IrSanyo::checksum() {
    // Stored the checksum value in the last byte.
    remote_state[kSanyoAcStateLength - 1] = calcChecksum(remote_state);
}

uint8_t IrSanyo::calcChecksum(const uint8_t *state, const uint16_t length) {
    return length ? sumNibbles(state, length - 1) : 0;
}

void IrSanyo::_setTemp(uint8_t *ptr, const uint8_t degrees) {
    uint8_t temp = std::max((uint8_t)kSanyoAcTempMin, degrees);
    temp = std::min((uint8_t)kSanyoAcTempMax, temp);
    setBits(ptr, kSanyoAcTempOffset, kSanyoAcTempSize, temp - kSanyoAcTempDelta);
}

uint8_t IrSanyo::_getTemp(uint8_t *ptr) {
    return GETBITS8(*ptr, kSanyoAcTempOffset, kSanyoAcTempSize) +
           kSanyoAcTempDelta;
}
/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/


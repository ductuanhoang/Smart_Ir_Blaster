/*
 * IrArgo.cpp
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
#include "IrArgo.h"
#include "IRutils.h"
/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/
// Constants
// using SPACE modulation. MARK is always const 400u
const uint16_t kArgoHdrMark = 6400;
const uint16_t kArgoHdrSpace = 3300;
const uint16_t kArgoBitMark = 400;
const uint16_t kArgoOneSpace = 2200;
const uint16_t kArgoZeroSpace = 900;
const uint32_t kArgoGap = kDefaultMessageGap;  // Made up value. Complete guess.

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
Ir_Argo::Ir_Argo(uint32_t frequency, uint16_t dutycycle): _ir_argo_send(frequency, dutycycle) {
	// TODO Auto-generated constructor stub
	stateReset();
}

Ir_Argo::~Ir_Argo() {
	// TODO Auto-generated destructor stub
}

void Ir_Argo::sendArgo(const uint16_t repeat) {
    send(getRaw(), kArgoStateLength, repeat);
    APP_LOGD("IR Argo send data = ");
    for (int i = 0; i < kArgoStateLength; i++)
    {
    	printf("%x-", _.raw[i]);
	}
    printf("\r\n");
}

void Ir_Argo::setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp) {
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
uint8_t Ir_Argo::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool:
      return kArgoCool;
    case stdAc::opmode_t::kHeat:
      return kArgoHeat;
    case stdAc::opmode_t::kDry:
      return kArgoDry;
    case stdAc::opmode_t::kOff:
      return kArgoOff;
    // No fan mode.
    default:
      return kArgoAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t Ir_Argo::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:
      return kArgoFan1;
    case stdAc::fanspeed_t::kMedium:
      return kArgoFan2;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:
      return kArgoFan3;
    default:
      return kArgoFanAuto;
  }
}

/// Convert a stdAc::swingv_t enum into it's native setting.
/// @param[in] position The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t Ir_Argo::convertSwingV(const stdAc::swingv_t position) {
  switch (position) {
    case stdAc::swingv_t::kHighest:
      return kArgoFlapFull;
    case stdAc::swingv_t::kHigh:
      return kArgoFlap5;
    case stdAc::swingv_t::kMiddle:
      return kArgoFlap4;
    case stdAc::swingv_t::kLow:
      return kArgoFlap3;
    case stdAc::swingv_t::kLowest:
      return kArgoFlap1;
    default:
      return kArgoFlapAuto;
  }
}

void Ir_Argo::send(const unsigned char *data, const uint16_t nbytes, const uint16_t repeat) {
// Check if we have enough bytes to send a proper message.
    if (nbytes < kArgoStateLength) return;
    // TODO(kaschmo): validate
    _ir_argo_send.sendGeneric(kArgoHdrMark, kArgoHdrSpace, kArgoBitMark, kArgoOneSpace,
                kArgoBitMark, kArgoZeroSpace, 0, 0,  // No Footer.
                data, nbytes, 38, false, repeat, 50);
}

void Ir_Argo::on() {
    setPower(true);
}

void Ir_Argo::off() {
    setPower(false);
}

void Ir_Argo::setPower(const bool on) {
    _.Power = on;
}

bool Ir_Argo::getPower() const {
    return _.Power;
}

void Ir_Argo::setTemp(const uint8_t degrees) {
    uint8_t temp = std::max(kArgoMinTemp, degrees);
    // delta 4 degrees. "If I want 12 degrees, I need to send 8"
    temp = std::min(kArgoMaxTemp, temp) - kArgoTempDelta;
    // mask out bits
    // argo[13] & 0x00000100;  // mask out ON/OFF Bit
    _.Temp = temp;
}

uint8_t Ir_Argo::getTemp() const {
    return _.Temp + kArgoTempDelta;
}

void Ir_Argo::setFan(const uint8_t fan) {
    _.Fan = std::min(fan, kArgoFan3);
}

uint8_t Ir_Argo::getFan() const {
    return _.Fan;
}

void Ir_Argo::setFlap(const uint8_t flap) {
    flap_mode = flap;
    // TODO(kaschmo): set correct bits for flap mode
}

uint8_t Ir_Argo::getFlap() const {
    return flap_mode;
}

void Ir_Argo::setMode(const uint8_t mode) {
    switch (mode) {
        case kArgoCool:
        case kArgoDry:
        case kArgoAuto:
        case kArgoOff:
        case kArgoHeat:
        case kArgoHeatAuto:
            _.Mode = mode;
            return;
        default:
            _.Mode = kArgoAuto;
    }
}

uint8_t Ir_Argo::getMode() const {
    return _.Mode;
}

void Ir_Argo::setMax(const bool on) {
    _.Max = on;
}

bool Ir_Argo::getMax() const {
    return _.Max;
}

void Ir_Argo::setNight(const bool on) {
    _.Night = on;
}

bool Ir_Argo::getNight() const {
    return _.Night;
}

void Ir_Argo::setiFeel(const bool on) {
    _.iFeel = on;
}

bool Ir_Argo::getiFeel() const {
    return _.iFeel;
}

void Ir_Argo::setTime() {
    // TODO(kaschmo): use function call from checksum to set time first
}

void Ir_Argo::setRoomTemp(const uint8_t degrees) {
    uint8_t temp = std::min(degrees, kArgoMaxRoomTemp);
    temp = std::max(temp, kArgoTempDelta) - kArgoTempDelta;
    _.RoomTemp = temp;
}

uint8_t Ir_Argo::getRoomTemp() const {
    return _.RoomTemp + kArgoTempDelta;
}

uint8_t * Ir_Argo::getRaw() {
    checksum();  // Ensure correct bit array before returning
    return _.raw;
}

void Ir_Argo::setRaw(const uint8_t *state) {
    memcpy(_.raw, state, kArgoStateLength);
}

uint8_t Ir_Argo::calcChecksum(const uint8_t *state, const uint16_t length) {
    // Corresponds to byte 11 being constant 0b01
    // Only add up bytes to 9. byte 10 is 0b01 constant anyway.
    // Assume that argo array is MSB first (left)
    return sumBytes(state, length - 2, 2);
}

bool Ir_Argo::validChecksum(const uint8_t *state, const uint16_t length) {
    return ((state[length - 2] >> 2) + (state[length - 1] << 6)) ==
           calcChecksum(state, length);
}

void Ir_Argo::stateReset() {
    for (uint8_t i = 0; i < kArgoStateLength; i++) _.raw[i] = 0x0;

    // Argo Message. Store MSB left.
    // Default message:
    _.raw[0] = 0b10101100;  // LSB first (as sent) 0b00110101; //const preamble
    _.raw[1] = 0b11110101;  // LSB first: 0b10101111; //const preamble
    // Keep payload 2-9 at zero
    _.raw[10] = 0b00000010;  // Const 01
    _.Sum = 0;

    off();
    setTemp(20);
    setRoomTemp(25);
    setMode(kArgoAuto);
    setFan(kArgoFanAuto);
}

void Ir_Argo::checksum() {
    uint8_t sum = calcChecksum(_.raw, kArgoStateLength);
    // Append sum to end of array
    // Set const part of checksum bit 10
    _.raw[10] = 0b00000010;
    _.Sum = sum;
}
/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/


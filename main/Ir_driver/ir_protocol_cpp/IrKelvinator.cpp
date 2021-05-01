/*
 * IrKelvinator.cpp
 *
 *  Created on: Jan 19, 2021
 *      Author: ductu
 */
/***********************************************************************************************************************
 * Pragma directive
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes <System Includes>
 ***********************************************************************************************************************/
#include "IrKelvinator.h"
#include "IRutils.h"
/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/
// Constants
const uint16_t kKelvinatorTick = 85;
const uint16_t kKelvinatorHdrMarkTicks = 106;
const uint16_t kKelvinatorHdrMark = kKelvinatorHdrMarkTicks * kKelvinatorTick;
const uint16_t kKelvinatorHdrSpaceTicks = 53;
const uint16_t kKelvinatorHdrSpace = kKelvinatorHdrSpaceTicks * kKelvinatorTick;
const uint16_t kKelvinatorBitMarkTicks = 8;
const uint16_t kKelvinatorBitMark = kKelvinatorBitMarkTicks * kKelvinatorTick;
const uint16_t kKelvinatorOneSpaceTicks = 18;
const uint16_t kKelvinatorOneSpace = kKelvinatorOneSpaceTicks * kKelvinatorTick;
const uint16_t kKelvinatorZeroSpaceTicks = 6;
const uint16_t kKelvinatorZeroSpace =
        kKelvinatorZeroSpaceTicks * kKelvinatorTick;
const uint16_t kKelvinatorGapSpaceTicks = 235;
const uint16_t kKelvinatorGapSpace = kKelvinatorGapSpaceTicks * kKelvinatorTick;

const uint8_t kKelvinatorCmdFooter = 2;
const uint8_t kKelvinatorCmdFooterBits = 3;

const uint8_t kKelvinatorChecksumStart = 10;
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
Ir_Kelvinator::Ir_Kelvinator(uint32_t frequency, uint16_t dutycycle): _ir_kelvinator_send(frequency, dutycycle){
	// TODO Auto-generated constructor stub

}

Ir_Kelvinator::~Ir_Kelvinator() {
	// TODO Auto-generated destructor stub
}

void Ir_Kelvinator::sendKelvinator(const uint16_t repeat) {
    send(getRaw(), kKelvinatorStateLength, repeat);
}
/***********************************************************************************************************************
 * Static Functions
 ***********************************************************************************************************************/
void Ir_Kelvinator::send(const unsigned char *data, const uint16_t nbytes, const uint16_t repeat) {
    if (nbytes < kKelvinatorStateLength)
        return;  // Not enough bytes to send a proper message.

    for (uint16_t r = 0; r <= repeat; r++) {
        // Command Block #1 (4 bytes)
        _ir_kelvinator_send.sendGeneric(kKelvinatorHdrMark, kKelvinatorHdrSpace, kKelvinatorBitMark,
                    kKelvinatorOneSpace, kKelvinatorBitMark, kKelvinatorZeroSpace,
                    0, 0,  // No Footer yet.
                    data, 4, 38, false, 0, 50);
        // Send Footer for the command block (3 bits (b010))
        _ir_kelvinator_send.sendGeneric(0, 0,  // No Header
                    kKelvinatorBitMark, kKelvinatorOneSpace, kKelvinatorBitMark,
                    kKelvinatorZeroSpace, kKelvinatorBitMark, kKelvinatorGapSpace,
                    kKelvinatorCmdFooter, kKelvinatorCmdFooterBits, 38, false, 0,
                    50);
        // Data Block #1 (4 bytes)
        _ir_kelvinator_send.sendGeneric(0, 0,  // No header
                    kKelvinatorBitMark, kKelvinatorOneSpace, kKelvinatorBitMark,
                    kKelvinatorZeroSpace, kKelvinatorBitMark,
                    kKelvinatorGapSpace * 2, data + 4, 4, 38, false, 0, 50);
        // Command Block #2 (4 bytes)
        _ir_kelvinator_send.sendGeneric(kKelvinatorHdrMark, kKelvinatorHdrSpace, kKelvinatorBitMark,
                    kKelvinatorOneSpace, kKelvinatorBitMark, kKelvinatorZeroSpace,
                    0, 0,  // No Footer yet.
                    data + 8, 4, 38, false, 0, 50);
        // Send Footer for the command block (3 bits (B010))
        _ir_kelvinator_send.sendGeneric(0, 0,  // No Header
                    kKelvinatorBitMark, kKelvinatorOneSpace, kKelvinatorBitMark,
                    kKelvinatorZeroSpace, kKelvinatorBitMark, kKelvinatorGapSpace,
                    kKelvinatorCmdFooter, kKelvinatorCmdFooterBits, 38, false, 0,
                    50);
        // Data Block #2 (4 bytes)
        _ir_kelvinator_send.sendGeneric(0, 0,  // No header
                    kKelvinatorBitMark, kKelvinatorOneSpace, kKelvinatorBitMark,
                    kKelvinatorZeroSpace, kKelvinatorBitMark,
                    kKelvinatorGapSpace * 2, data + 12, 4, 38, false, 0, 50);
    }
}
void Ir_Kelvinator::setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp) {
    setPower(power);
    setMode(convertMode(stdAc::opmode_t(mode)));
    setFan(speed);
    setTemp(temp);
}

/// Convert a standard A/C mode (stdAc::opmode_t) into it a native mode.
/// @param[in] mode A stdAc::opmode_t operation mode.
/// @return The native mode equivalent.
uint8_t Ir_Kelvinator::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kKelvinatorCool;
    case stdAc::opmode_t::kHeat: return kKelvinatorHeat;
    case stdAc::opmode_t::kDry:  return kKelvinatorDry;
    case stdAc::opmode_t::kFan:  return kKelvinatorFan;
    default:                     return kKelvinatorAuto;
  }
}

void Ir_Kelvinator::on() {
    setPower(true);
}

void Ir_Kelvinator::off() {
    setPower(false);
}

void Ir_Kelvinator::setPower(const bool on) {
    _.Power = on;
}

bool Ir_Kelvinator::getPower() const {
    return _.Power;
}

void Ir_Kelvinator::setTemp(const uint8_t degrees) {
    uint8_t temp = std::max(kKelvinatorMinTemp, degrees);
    temp = std::min(kKelvinatorMaxTemp, temp);
    _.Temp = temp - kKelvinatorMinTemp;
}

uint8_t Ir_Kelvinator::getTemp() const {
    return _.Temp + kKelvinatorMinTemp;
}

void Ir_Kelvinator::setFan(const uint8_t speed) {
    uint8_t fan = std::min(kKelvinatorFanMax, speed);  // Bounds check

    // Only change things if we need to.
    if (fan != _.Fan) {
        // Set the basic fan values.
        _.BasicFan = std::min(kKelvinatorBasicFanMax, fan);
        // Set the advanced(?) fan value.
        _.Fan = fan;
        // Turbo mode is turned off if we change the fan settings.
        setTurbo(false);
    }
}

uint8_t Ir_Kelvinator::getFan() const {
    return _.Fan;
}

void Ir_Kelvinator::setMode(const uint8_t mode) {
    switch (mode) {
        case kKelvinatorAuto:
        case kKelvinatorDry:
            // When the remote is set to Auto or Dry, it defaults to 25C and doesn't
            // show it.
            setTemp(kKelvinatorAutoTemp);
            // FALL-THRU
            break;
        case kKelvinatorHeat:
        case kKelvinatorCool:
        case kKelvinatorFan:
            _.Mode = mode;
            break;
        default:
            setTemp(kKelvinatorAutoTemp);
            _.Mode = kKelvinatorAuto;
            break;
    }
}

uint8_t Ir_Kelvinator::getMode() const {
    return _.Mode;
}

void Ir_Kelvinator::setSwingVertical(const bool on) {
    _.SwingV = on;
    _.VentSwing = (on || _.SwingH);
}

bool Ir_Kelvinator::getSwingVertical() const {
    return _.SwingV;
}

void Ir_Kelvinator::setSwingHorizontal(const bool on) {
    _.SwingH = on;
    _.VentSwing = (on || _.SwingV);
}

bool Ir_Kelvinator::getSwingHorizontal() const {
    return _.SwingH;
}

void Ir_Kelvinator::setQuiet(const bool on) {
    _.Quiet = on;
}

bool Ir_Kelvinator::getQuiet() const {
    return _.Quiet;
}

void Ir_Kelvinator::setIonFilter(const bool on) {
    _.IonFilter = on;
}

bool Ir_Kelvinator::getIonFilter() const {
    return _.IonFilter;
}

void Ir_Kelvinator::setLight(const bool on) {
    _.Light = on;
}

bool Ir_Kelvinator::getLight() const {
    return _.Light;
}

void Ir_Kelvinator::setXFan(const bool on) {
    _.XFan = on;
}

bool Ir_Kelvinator::getXFan() const {
    return _.XFan;
}

void Ir_Kelvinator::setTurbo(const bool on) {
    _.Turbo = on;
}

bool Ir_Kelvinator::getTurbo() const {
    return _.Turbo;
}

uint8_t * Ir_Kelvinator::getRaw() {
    fixup();  // Ensure correct settings before sending.
    return _.raw;
}

uint8_t Ir_Kelvinator::calcBlockChecksum(const uint8_t *block, const uint16_t length) {
    uint8_t sum = kKelvinatorChecksumStart;
    // Sum the lower half of the first 4 bytes of this block.
    for (uint8_t i = 0; i < 4 && i < length - 1; i++, block++)
        sum += (*block & 0b1111);
    // then sum the upper half of the next 3 bytes.
    for (uint8_t i = 4; i < length - 1; i++, block++) sum += (*block >> 4);
    // Trim it down to fit into the 4 bits allowed. i.e. Mod 16.
    return sum & 0b1111;
}

bool Ir_Kelvinator::validChecksum(const uint8_t *state, const uint16_t length) {
    for (uint16_t offset = 0; offset + 7 < length; offset += 8) {
        // Top 4 bits of the last byte in the block is the block's checksum.
        if (GETBITS8(state[offset + 7], kHighNibble, kNibbleSize) !=
            calcBlockChecksum(state + offset))
            return false;
    }
    return true;
}

void Ir_Kelvinator::stateReset() {
    for (uint8_t i = 0; i < kKelvinatorStateLength; i++) _.raw[i] = 0x0;
    _.raw[3] = 0x50;
    _.raw[11] = 0x70;
}

void Ir_Kelvinator::checksum() {
    _.Sum1 = calcBlockChecksum(_.raw);
    _.Sum2 = calcBlockChecksum(_.raw + 8);
}

void Ir_Kelvinator::fixup() {
    // X-Fan mode is only valid in COOL or DRY modes.
    if (_.Mode != kKelvinatorCool && _.Mode != kKelvinatorDry)
        setXFan(false);
    // Duplicate to the 2nd command chunk.
    _.raw[8] = _.raw[0];
    _.raw[9] = _.raw[1];
    _.raw[10] = _.raw[2];
    checksum();  // Calculate the checksums
}
/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/


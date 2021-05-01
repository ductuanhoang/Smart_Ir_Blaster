/*
 * IrPanasonic.cpp
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
#include "IrPanasonic.h"
#include "IRutils.h"
/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/
// Constants
/// @see http://www.remotecentral.com/cgi-bin/mboard/rc-pronto/thread.cgi?26152
const uint16_t kPanasonicHdrMark = 3456;            ///< uSeconds.
const uint16_t kPanasonicHdrSpace = 1728;           ///< uSeconds.
const uint16_t kPanasonicBitMark = 432;             ///< uSeconds.
const uint16_t kPanasonicOneSpace = 1296;           ///< uSeconds.
const uint16_t kPanasonicZeroSpace = 432;           ///< uSeconds.
const uint32_t kPanasonicMinCommandLength = 163296; ///< uSeconds.
const uint16_t kPanasonicEndGap = 5000;             ///< uSeconds. See #245
const uint32_t kPanasonicMinGap = 74736;            ///< uSeconds.

const uint16_t kPanasonicAcSectionGap = 10000; ///< uSeconds.
const uint16_t kPanasonicAcSection1Length = 8;
const uint32_t kPanasonicAcMessageGap = kDefaultMessageGap; // Just a guess.

const uint16_t kPanasonicAc32HdrMark = 3543;     ///< uSeconds.
const uint16_t kPanasonicAc32BitMark = 920;      ///< uSeconds.
const uint16_t kPanasonicAc32HdrSpace = 3450;    ///< uSeconds.
const uint16_t kPanasonicAc32OneSpace = 2575;    ///< uSeconds.
const uint16_t kPanasonicAc32ZeroSpace = 828;    ///< uSeconds.
const uint16_t kPanasonicAc32SectionGap = 13946; ///< uSeconds.
const uint8_t kPanasonicAc32Sections = 2;
const uint8_t kPanasonicAc32BlocksPerSection = 2;

using irutils::setBit;
using irutils::setBits;
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
Ir_Panasonic::Ir_Panasonic(uint32_t frequency, uint16_t dutycycle) : _ir_panasonic(frequency, dutycycle)
{
    // TODO Auto-generated constructor stub
    stateReset();
}

Ir_Panasonic::~Ir_Panasonic()
{
    // TODO Auto-generated destructor stub
}

void Ir_Panasonic::sendPanasonicAC(const uint16_t repeat)
{
    send(getRaw(), kPanasonicAcStateLength, repeat);
    // check raw send
    APP_LOGD("IR Pana send data = ");
    for (int i = 0; i < kPanasonicAcStateLength; i++)
    {
    	printf("%x-", remote_state[i]);
	}
    printf("\r\n");
}

void Ir_Panasonic::sendPanasonic(const uint16_t address, const uint32_t data, const uint16_t nbits,
                                 const uint16_t repeat)
{
    sendPanasonic64(((uint64_t)address << 32) | (uint64_t)data, nbits, repeat);
}

void Ir_Panasonic::sendPanasonic64(const uint64_t data, const uint16_t nbits, const uint16_t repeat)
{
    _ir_panasonic.sendGeneric(kPanasonicHdrMark, kPanasonicHdrSpace, kPanasonicBitMark,
                              kPanasonicOneSpace, kPanasonicBitMark, kPanasonicZeroSpace,
                              kPanasonicBitMark, kPanasonicMinGap, kPanasonicMinCommandLength,
                              data, nbits, kPanasonicFreq, true, repeat, 50);
}

void Ir_Panasonic::sendPanasonicAC32(const uint64_t data, const uint16_t nbits, const uint16_t repeat)
{
    uint16_t section_bits;
    uint16_t sections;
    uint16_t blocks;
    // Calculate the section, block, and bit sizes based on the requested bit size
    if (nbits > kPanasonicAc32Bits / 2)
    { // A long message
        section_bits = nbits / kPanasonicAc32Sections;
        sections = kPanasonicAc32Sections;
        blocks = kPanasonicAc32BlocksPerSection;
    }
    else
    { // A short message
        section_bits = nbits;
        sections = kPanasonicAc32Sections - 1;
        blocks = kPanasonicAc32BlocksPerSection + 1;
    }
    for (uint16_t r = 0; r <= repeat; r++)
    {
        for (uint8_t section = 0; section < sections; section++)
        {
            uint64_t section_data;
            section_data = GETBITS64(data, section_bits * (sections - section - 1),
                                     section_bits);

            // Duplicate bytes in the data.
            uint64_t expanded_data = 0;
            for (uint8_t i = 0; i < sizeof(expanded_data); i++)
            {
                const uint8_t first_byte = section_data >> 56;
                for (uint8_t i = 0; i < 2; i++)
                    expanded_data = (expanded_data << 8) | first_byte;
                section_data <<= 8;
            }
            // Two data blocks per section (i.e. 1 + a repeat)
            _ir_panasonic.sendGeneric(kPanasonicAc32HdrMark, kPanasonicAc32HdrSpace, // Header
                                      kPanasonicAc32BitMark, kPanasonicAc32OneSpace, // Data
                                      kPanasonicAc32BitMark, kPanasonicAc32ZeroSpace,
                                      0, 0, // No Footer
                                      expanded_data, section_bits * 2, kPanasonicFreq, false,
                                      blocks - 1, // Repeat
                                      50);
            // Section Footer
            _ir_panasonic.sendGeneric(kPanasonicAc32HdrMark, kPanasonicAc32HdrSpace,   // Header
                                      0, 0, 0, 0,                                      // No Data
                                      kPanasonicAc32BitMark, kPanasonicAc32SectionGap, // Footer
                                      data, 0,                                         // No data (bits)
                                      kPanasonicFreq, true, 0, 50);
        }
    }
}

void Ir_Panasonic::setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp) {
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
uint8_t Ir_Panasonic::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kPanasonicAcCool;
    case stdAc::opmode_t::kHeat: return kPanasonicAcHeat;
    case stdAc::opmode_t::kDry:  return kPanasonicAcDry;
    case stdAc::opmode_t::kFan:  return kPanasonicAcFan;
    default:                     return kPanasonicAcAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t Ir_Panasonic::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:    return kPanasonicAcFanMin;
    case stdAc::fanspeed_t::kLow:    return kPanasonicAcFanMin + 1;
    case stdAc::fanspeed_t::kMedium: return kPanasonicAcFanMin + 2;
    case stdAc::fanspeed_t::kHigh:   return kPanasonicAcFanMin + 3;
    case stdAc::fanspeed_t::kMax:    return kPanasonicAcFanMax;
    default:                         return kPanasonicAcFanAuto;
  }
}

void Ir_Panasonic::send(const unsigned char *data, const uint16_t nbytes, const uint16_t repeat)
{
    if (nbytes < kPanasonicAcSection1Length)
        return;
    for (uint16_t r = 0; r <= repeat; r++)
    {
        // First section. (8 bytes)
        _ir_panasonic.sendGeneric(kPanasonicHdrMark, kPanasonicHdrSpace, kPanasonicBitMark,
                                  kPanasonicOneSpace, kPanasonicBitMark, kPanasonicZeroSpace,
                                  kPanasonicBitMark, kPanasonicAcSectionGap, data,
                                  kPanasonicAcSection1Length, kPanasonicFreq, false, 0, 50);
        // First section. (The rest of the data bytes)
        _ir_panasonic.sendGeneric(kPanasonicHdrMark, kPanasonicHdrSpace, kPanasonicBitMark,
                                  kPanasonicOneSpace, kPanasonicBitMark, kPanasonicZeroSpace,
                                  kPanasonicBitMark, kPanasonicAcMessageGap,
                                  data + kPanasonicAcSection1Length,
                                  nbytes - kPanasonicAcSection1Length, kPanasonicFreq, false, 0,
                                  50);
    }
}

void Ir_Panasonic::stateReset()
{
    memcpy(remote_state, kPanasonicKnownGoodState, kPanasonicAcStateLength);
    _temp = 25;                         // An initial saved desired temp. Completely made up.
    _swingh = kPanasonicAcSwingHMiddle; // A similar made up value for H Swing.
}

void Ir_Panasonic::on()
{
    setPower(true);
}
void Ir_Panasonic::off()
{
    setPower(false);
}

void Ir_Panasonic::setPower(const bool on)
{
    setBit(&remote_state[13], kPanasonicAcPowerOffset, on);
}

bool Ir_Panasonic::getPower()
{
    return GETBIT8(remote_state[13], kPanasonicAcPowerOffset);
}

void Ir_Panasonic::setTemp(const uint8_t celsius, const bool remember)
{
    uint8_t temperature;
    temperature = max(celsius, kPanasonicAcMinTemp);
    temperature = min(temperature, kPanasonicAcMaxTemp);
    if (remember)
        _temp = temperature;
    setBits(&remote_state[14], kPanasonicAcTempOffset, kPanasonicAcTempSize,
            temperature);
}

uint8_t Ir_Panasonic::getTemp()
{
    return GETBITS8(remote_state[14], kPanasonicAcTempOffset,
                    kPanasonicAcTempSize);
}

void Ir_Panasonic::setFan(const uint8_t fan)
{
    switch (fan)
    {
    case kPanasonicAcFanMin:
    case kPanasonicAcFanMed:
    case kPanasonicAcFanMax:
    case kPanasonicAcFanAuto:
        setBits(&remote_state[16], kHighNibble, kNibbleSize,
                fan + kPanasonicAcFanDelta);
        break;
    default:
        setFan(kPanasonicAcFanAuto);
    }
}

uint8_t Ir_Panasonic::getFan()
{
    return GETBITS8(remote_state[16], kHighNibble, kNibbleSize) -
           kPanasonicAcFanDelta;
}
void Ir_Panasonic::setMode(const uint8_t desired)
{
    uint8_t mode = kPanasonicAcAuto; // Default to Auto mode.
    switch (desired)
    {
    case kPanasonicAcFan:
        // Allegedly Fan mode has a temperature of 27.
        this->setTemp(kPanasonicAcFanModeTemp, false);
        mode = desired;
        break;
    case kPanasonicAcAuto:
    case kPanasonicAcCool:
    case kPanasonicAcHeat:
    case kPanasonicAcDry:
        mode = desired;
        // Set the temp to the saved temp, just incase our previous mode was Fan.
        this->setTemp(_temp);
        break;
    }
    remote_state[13] &= 0x0F; // Clear the previous mode bits.
    setBits(&remote_state[13], kHighNibble, kModeBitsSize, mode);
}

uint8_t Ir_Panasonic::getMode()
{
    return GETBITS8(remote_state[13], kHighNibble, kModeBitsSize);
}

void Ir_Panasonic::setRaw(const uint8_t *state)
{
    memcpy(remote_state, state, kPanasonicAcStateLength);
}

uint8_t *Ir_Panasonic::getRaw()
{
    this->fixChecksum();
    return remote_state;
}

bool Ir_Panasonic::validChecksum(const uint8_t *state, const uint16_t length)
{
    if (length < 2)
        return false; // 1 byte of data can't have a checksum.
    return (state[length - 1] ==
            sumBytes(state, length - 1, kPanasonicAcChecksumInit));
}

uint8_t Ir_Panasonic::calcChecksum(const uint8_t *state, const uint16_t length)
{
    return sumBytes(state, length - 1, kPanasonicAcChecksumInit);
}

void Ir_Panasonic::setQuiet(const bool on)
{
    uint8_t offset;
    switch (this->getModel())
    {
    case kPanasonicRkr:
    case kPanasonicCkp:
        offset = kPanasonicAcQuietCkpOffset;
        break;
    default:
        offset = kPanasonicAcQuietOffset;
    }
    if (on)
        this->setPowerful(false); // Powerful is mutually exclusive.
    setBit(&remote_state[21], offset, on);
}

bool Ir_Panasonic::getQuiet()
{
    switch (this->getModel())
    {
    case kPanasonicRkr:
    case kPanasonicCkp:
        return GETBIT8(remote_state[21], kPanasonicAcQuietCkpOffset);
    default:
        return GETBIT8(remote_state[21], kPanasonicAcQuietOffset);
    }
}

void Ir_Panasonic::setPowerful(const bool on)
{
    uint8_t offset;
    switch (this->getModel())
    {
    case kPanasonicRkr:
    case kPanasonicCkp:
        offset = kPanasonicAcPowerfulCkpOffset;
        break;
    default:
        offset = kPanasonicAcPowerfulOffset;
    }

    if (on)
        this->setQuiet(false); // Quiet is mutually exclusive.
    setBit(&remote_state[21], offset, on);
}

bool Ir_Panasonic::getPowerful()
{
    switch (this->getModel())
    {
    case kPanasonicRkr:
    case kPanasonicCkp:
        return GETBIT8(remote_state[21], kPanasonicAcPowerfulCkpOffset);
    default:
        return GETBIT8(remote_state[21], kPanasonicAcPowerfulOffset);
    }
}

void Ir_Panasonic::setIon(const bool on)
{
    if (this->getModel() == kPanasonicDke)
        setBit(&remote_state[kPanasonicAcIonFilterByte],
               kPanasonicAcIonFilterOffset, on);
}

bool Ir_Panasonic::getIon()
{
    switch (this->getModel())
    {
    case kPanasonicDke:
        return GETBIT8(remote_state[kPanasonicAcIonFilterByte],
                       kPanasonicAcIonFilterOffset);
    default:
        return false;
    }
}

void Ir_Panasonic::setModel(const panasonic_ac_remote_model_t model)
{
    switch (model)
    {
    case panasonic_ac_remote_model_t::kPanasonicDke:
    case panasonic_ac_remote_model_t::kPanasonicJke:
    case panasonic_ac_remote_model_t::kPanasonicLke:
    case panasonic_ac_remote_model_t::kPanasonicNke:
    case panasonic_ac_remote_model_t::kPanasonicCkp:
    case panasonic_ac_remote_model_t::kPanasonicRkr:
        break;
        // Only proceed if we know what to do.
    default:
        return;
    }
    // clear & set the various bits and bytes.
    remote_state[13] &= 0xF0;
    remote_state[17] = 0x00;
    remote_state[21] &= 0b11101111;
    remote_state[23] = 0x81;
    remote_state[25] = 0x00;

    switch (model)
    {
    case kPanasonicLke:
        remote_state[13] |= 0x02;
        remote_state[17] = 0x06;
        break;
    case kPanasonicDke:
        remote_state[23] = 0x01;
        remote_state[25] = 0x06;
        // Has to be done last as setSwingHorizontal has model check built-in
        this->setSwingHorizontal(_swingh);
        break;
    case kPanasonicNke:
        remote_state[17] = 0x06;
        break;
    case kPanasonicJke:
        break;
    case kPanasonicCkp:
        remote_state[21] |= 0x10;
        remote_state[23] = 0x01;
        break;
    case kPanasonicRkr:
        remote_state[13] |= 0x08;
        remote_state[23] = 0x89;
        break;
    default:
        break;
    }
    // Reset the Ion filter.
    setIon(getIon());
}
panasonic_ac_remote_model_t Ir_Panasonic::getModel()
{
    if (remote_state[23] == 0x89)
        return kPanasonicRkr;
    if (remote_state[17] == 0x00)
    {
        if ((remote_state[21] & 0x10) && (remote_state[23] & 0x01))
            return panasonic_ac_remote_model_t::kPanasonicCkp;
        if (remote_state[23] & 0x80)
            return panasonic_ac_remote_model_t::kPanasonicJke;
    }
    if (remote_state[17] == 0x06 && (remote_state[13] & 0x0F) == 0x02)
        return panasonic_ac_remote_model_t::kPanasonicLke;
    if (remote_state[23] == 0x01)
        return panasonic_ac_remote_model_t::kPanasonicDke;
    if (remote_state[17] == 0x06)
        return panasonic_ac_remote_model_t::kPanasonicNke;
    return panasonic_ac_remote_model_t::kPanasonicUnknown; // Default
}

void Ir_Panasonic::setSwingVertical(const uint8_t desired_elevation)
{
    uint8_t elevation = desired_elevation;
    if (elevation != kPanasonicAcSwingVAuto)
    {
        elevation = std::max(elevation, kPanasonicAcSwingVHighest);
        elevation = std::min(elevation, kPanasonicAcSwingVLowest);
    }
    setBits(&remote_state[16], kLowNibble, kNibbleSize, elevation);
}

uint8_t Ir_Panasonic::getSwingVertical()
{
    return GETBITS8(remote_state[16], kLowNibble, kNibbleSize);
}

void Ir_Panasonic::setSwingHorizontal(const uint8_t desired_direction)
{
    switch (desired_direction)
    {
    case kPanasonicAcSwingHAuto:
    case kPanasonicAcSwingHMiddle:
    case kPanasonicAcSwingHFullLeft:
    case kPanasonicAcSwingHLeft:
    case kPanasonicAcSwingHRight:
    case kPanasonicAcSwingHFullRight:
        break;
        // Ignore anything that isn't valid.
    default:
        return;
    }
    _swingh = desired_direction; // Store the direction for later.
    uint8_t direction = desired_direction;
    switch (this->getModel())
    {
    case kPanasonicDke:
    case kPanasonicRkr:
        break;
    case kPanasonicNke:
    case kPanasonicLke:
        direction = kPanasonicAcSwingHMiddle;
        break;
    default: // Ignore everything else.
        return;
    }
    setBits(&remote_state[17], kLowNibble, kNibbleSize, direction);
}

uint8_t Ir_Panasonic::getSwingHorizontal()
{
    return GETBITS8(remote_state[17], kLowNibble, kNibbleSize);
}
uint16_t Ir_Panasonic::getClock()
{
    return _getTime(&remote_state[24]);
}
void Ir_Panasonic::setClock(const uint16_t mins_since_midnight)
{
    _setTime(&remote_state[24], mins_since_midnight, false);
}

uint16_t Ir_Panasonic::getOnTimer()
{
    return _getTime(&remote_state[18]);
}

void Ir_Panasonic::setOnTimer(const uint16_t mins_since_midnight, const bool enable)
{
    setBit(&remote_state[13], kPanasonicAcOnTimerOffset, enable);
    // Store the time.
    _setTime(&remote_state[18], mins_since_midnight, true);
}
void Ir_Panasonic::cancelOnTimer()
{
    this->setOnTimer(0, false);
}

bool Ir_Panasonic::isOnTimerEnabled()
{
    return GETBIT8(remote_state[13], kPanasonicAcOnTimerOffset);
}

uint16_t Ir_Panasonic::getOffTimer()
{
    uint16_t result = (GETBITS8(remote_state[20], 0, 7) << kNibbleSize) |
                      GETBITS8(remote_state[19], kHighNibble, kNibbleSize);
    if (result == kPanasonicAcTimeSpecial)
        return 0;
    return result;
}

void Ir_Panasonic::setOffTimer(const uint16_t mins_since_midnight, const bool enable)
{
    // Ensure its on a 10 minute boundary and no overflow.
    uint16_t corrected = std::min(mins_since_midnight, kPanasonicAcTimeMax);
    corrected -= corrected % 10;
    if (mins_since_midnight == kPanasonicAcTimeSpecial)
        corrected = kPanasonicAcTimeSpecial;
    // Set the timer flag.
    setBit(&remote_state[13], kPanasonicAcOffTimerOffset, enable);
    // Store the time.
    setBits(&remote_state[19], kHighNibble, kNibbleSize, corrected);
    setBits(&remote_state[20], 0, 7, corrected >> kNibbleSize);
}

void Ir_Panasonic::cancelOffTimer()
{
    this->setOffTimer(0, false);
}

bool Ir_Panasonic::isOffTimerEnabled()
{
    return GETBIT8(remote_state[13], kPanasonicAcOffTimerOffset);
}

void Ir_Panasonic::fixChecksum(const uint16_t length)
{
    remote_state[length - 1] = this->calcChecksum(remote_state, length);
}

uint16_t Ir_Panasonic::_getTime(const uint8_t *ptr)
{
    uint16_t result = (GETBITS8(
                           ptr[1], kLowNibble, kPanasonicAcTimeOverflowSize)
                       << (kPanasonicAcTimeSize - kPanasonicAcTimeOverflowSize)) +
                      ptr[0];
    if (result == kPanasonicAcTimeSpecial)
        return 0;
    return result;
}

void Ir_Panasonic::_setTime(uint8_t *const ptr, const uint16_t mins_since_midnight, const bool round_down)
{
    uint16_t corrected = std::min(mins_since_midnight, kPanasonicAcTimeMax);
    if (round_down)
        corrected -= corrected % 10;
    if (mins_since_midnight == kPanasonicAcTimeSpecial)
        corrected = kPanasonicAcTimeSpecial;
    ptr[0] = corrected;
    setBits(&ptr[1], kLowNibble, kPanasonicAcTimeOverflowSize,
            corrected >> (kPanasonicAcTimeSize - kPanasonicAcTimeOverflowSize));
}
/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/

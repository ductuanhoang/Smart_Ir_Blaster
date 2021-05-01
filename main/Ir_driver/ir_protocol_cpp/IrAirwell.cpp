/*
 * IrAirwell.cpp
 *
 *  Created on: Jan 13, 2021
 *      Author: ductu
 */

/***********************************************************************************************************************
 * Pragma directive
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes <System Includes>
 ***********************************************************************************************************************/
#include "IrAirwell.h"

/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/
const uint8_t kAirwellOverhead = 4;
const uint16_t kAirwellHalfClockPeriod = 950; // uSeconds
const uint16_t kAirwellHdrMark = 3 * kAirwellHalfClockPeriod;  // uSeconds
const uint16_t kAirwellHdrSpace = 3 * kAirwellHalfClockPeriod;  // uSeconds
const uint16_t kAirwellFooterMark = 5 * kAirwellHalfClockPeriod;  // uSeconds

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/

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
Ir_Airwell::Ir_Airwell(uint32_t frequency, uint16_t dutycycle)
    : _ir_airwell_send(frequency, dutycycle)
{
  // TODO Auto-generated constructor stub
  stateReset();
}

Ir_Airwell::~Ir_Airwell()
{
  // TODO Auto-generated destructor stub
}

/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void Ir_Airwell::sendAirwell(const uint16_t repeat)
{
  send(getRaw());
}

void Ir_Airwell::setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp) {
    setPowerToggle(power);
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
uint8_t Ir_Airwell::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kAirwellCool;
    case stdAc::opmode_t::kHeat: return kAirwellHeat;
    case stdAc::opmode_t::kDry:  return kAirwellDry;
    case stdAc::opmode_t::kFan:  return kAirwellFan;
    default:                     return kAirwellAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t Ir_Airwell::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:
      return kAirwellFanLow;
    case stdAc::fanspeed_t::kMedium:
      return kAirwellFanMedium;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:
      return kAirwellFanHigh;
    default:
      return kAirwellFanAuto;
  }
}


void Ir_Airwell::send(uint64_t data, uint16_t nbits, uint16_t repeat)
{
  // Header + Data
  _ir_airwell_send.sendManchester(kAirwellHdrMark, kAirwellHdrMark, kAirwellHalfClockPeriod,
                                  0, 0, data, nbits, 38000, true, repeat, 50, false);
  // Footer
  _ir_airwell_send.mark_space(kAirwellHdrMark + kAirwellHalfClockPeriod, 0x7FFF);
}

/// Reset the internals of the object to a known good state.
void Ir_Airwell::stateReset(void)
{
  _AirwellProtocol.raw = kAirwellKnownGoodState;
}

/// Turn on/off the Power Airwell setting.
/// @param[in] on The desired setting state.
void Ir_Airwell::setPowerToggle(const bool on)
{
  _AirwellProtocol.PowerToggle = on;
}

/// Get the power toggle setting from the internal state.
/// @return A boolean indicating the setting.
bool Ir_Airwell::getPowerToggle(void) const
{
  return _AirwellProtocol.PowerToggle;
}

/// Get the current operation mode setting.
/// @return The current operation mode.
uint8_t Ir_Airwell::getMode(void) const
{
  return _AirwellProtocol.Mode;
}

/// Set the desired operation mode.
/// @param[in] mode The desired operation mode.
void Ir_Airwell::setMode(const uint8_t mode)
{
  switch (mode)
  {
  case kAirwellFan:
  case kAirwellCool:
  case kAirwellHeat:
  case kAirwellDry:
  case kAirwellAuto:
    _AirwellProtocol.Mode = mode;
    break;
  default:
    _AirwellProtocol.Mode = kAirwellAuto;
  }
  setFan(getFan()); // Ensure the fan is at the correct speed for the new mode.
}
/// Set the temperature.
/// @param[in] degrees The temperature in degrees celsius.
void Ir_Airwell::setTemp(const uint8_t degrees)
{
  uint8_t temp = max(kAirwellMinTemp, degrees);
  temp = min(kAirwellMaxTemp, temp);
  _AirwellProtocol.Temp = (temp - kAirwellMinTemp + 1);
}

/// Get the current temperature setting.
/// @return Get current setting for temp. in degrees celsius.
uint8_t Ir_Airwell::getTemp(void) const
{
  return _AirwellProtocol.Temp + kAirwellMinTemp - 1;
}

/// Set the raw state of the object.
/// @param[in] state The raw state from the native IR message.
void Ir_Airwell::setRaw(const uint64_t state)
{
  _AirwellProtocol.raw = state;
}

/// Get the raw state of the object, suitable to be sent with the appropriate
/// IRsend object method.
/// @return A copy of the internal state.
uint64_t Ir_Airwell::getRaw(void) const
{
  return _AirwellProtocol.raw;
}
/// Set the speed of the fan.
/// @param[in] speed The desired setting.
/// @note The speed is locked to Low when in Dry mode.
void Ir_Airwell::setFan(const uint8_t speed)
{
  _AirwellProtocol.Fan = (_AirwellProtocol.Mode == kAirwellDry) ? kAirwellFanLow
                                                                : min(speed, kAirwellFanAuto);
}

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t Ir_Airwell::getFan(void) const
{
  return _AirwellProtocol.Fan;
}
/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/

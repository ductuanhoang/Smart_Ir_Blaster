/*
 * IrFujitsu.cpp
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
#include "IrFujitsu.h"
#include "IRutils.h"
/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/
// Ref:
// These values are based on averages of measurements
const uint16_t kFujitsuAcHdrMark = 3324;
const uint16_t kFujitsuAcHdrSpace = 1574;
const uint16_t kFujitsuAcBitMark = 448;
const uint16_t kFujitsuAcOneSpace = 1182;
const uint16_t kFujitsuAcZeroSpace = 390;
const uint16_t kFujitsuAcMinGap = 8100;
/***********************************************************************************************************************
 * Private global variables and functions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global variables and functions (to be accessed by other files)
 ***********************************************************************************************************************/
using irutils::setBit;
using irutils::setBits;
using std::min;
using std::max;

/***********************************************************************************************************************
 * Imported global variables and functions (from other files)
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: 
 * Description  :
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
Ir_Fujitsu::Ir_Fujitsu(const fujitsu_ac_remote_model_t model, uint32_t frequency, uint16_t dutycycle)
: _ir_fujitsu_ac(frequency, dutycycle)
{
	// TODO Auto-generated constructor stub
	setModel(model);
	stateReset();
}

Ir_Fujitsu::~Ir_Fujitsu() {
	// TODO Auto-generated destructor stub
}

void Ir_Fujitsu::sendFujitsu(const uint16_t repeat) {
    buildState();
    send(remote_state, getStateLength(), repeat);
    APP_LOGD("IR Fujit send data %d lenght = ", getStateLength());
    for (int i = 0; i < kFujitsuAcStateLength; i++)
    {
    	printf("%x-", remote_state[i]);
	}
    printf("\r\n");
}

void Ir_Fujitsu::setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp){
    setPower(power);
    setMode(convertMode(stdAc::opmode_t(mode)));
    setFanSpeed(convertFan(stdAc::fanspeed_t(speed)));
    setTemp(temp);
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t Ir_Fujitsu::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kFujitsuAcModeCool;
    case stdAc::opmode_t::kHeat: return kFujitsuAcModeHeat;
    case stdAc::opmode_t::kDry:  return kFujitsuAcModeDry;
    case stdAc::opmode_t::kFan:  return kFujitsuAcModeFan;
    default:                     return kFujitsuAcModeAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t Ir_Fujitsu::convertFan(stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:    return kFujitsuAcFanQuiet;
    case stdAc::fanspeed_t::kLow:    return kFujitsuAcFanLow;
    case stdAc::fanspeed_t::kMedium: return kFujitsuAcFanMed;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:    return kFujitsuAcFanHigh;
    default:                         return kFujitsuAcFanAuto;
  }
}

void Ir_Fujitsu::setModel(const fujitsu_ac_remote_model_t model) {
    _model = model;
    switch (model) {
        case fujitsu_ac_remote_model_t::ARDB1:
        case fujitsu_ac_remote_model_t::ARJW2:
            _state_length = kFujitsuAcStateLength - 1;
            _state_length_short = kFujitsuAcStateLengthShort - 1;
            break;
        case fujitsu_ac_remote_model_t::ARRY4:
        case fujitsu_ac_remote_model_t::ARRAH2E:
        case fujitsu_ac_remote_model_t::ARREB1E:
        default:
            _state_length = kFujitsuAcStateLength;
            _state_length_short = kFujitsuAcStateLengthShort;
    }
}

void Ir_Fujitsu::stateReset(void) {
    _temp = 24;
    _fanSpeed = kFujitsuAcFanHigh;
    _mode = kFujitsuAcModeCool;
    _swingMode = kFujitsuAcSwingBoth;
    _cmd = kFujitsuAcCmdTurnOn;
    _filter = false;
    _clean = false;
    _timertype = kFujitsuAcStopTimers;
    _ontimer = 0;
    _offtimer = 0;
    buildState();
}

/// Get the currently emulated/detected model of the A/C.
/// @return The enum representing the model of A/C.
fujitsu_ac_remote_model_t Ir_Fujitsu::getModel(void) { return _model;
}
/***********************************************************************************************************************
 * Static Functions
 ***********************************************************************************************************************/

void Ir_Fujitsu::send(const unsigned char data[], const uint16_t nbytes, const uint16_t repeat)
{
    _ir_fujitsu_ac.sendGeneric(kFujitsuAcHdrMark, kFujitsuAcHdrSpace, kFujitsuAcBitMark,
            kFujitsuAcOneSpace, kFujitsuAcBitMark, kFujitsuAcZeroSpace,
            kFujitsuAcBitMark, kFujitsuAcMinGap, data, nbytes, 38, false,
            repeat, 50);
}

void Ir_Fujitsu::stepHoriz() {
    setCmd(kFujitsuAcCmdStepHoriz);
}

void Ir_Fujitsu::toggleSwingHoriz(const bool update) {
    // Toggle the current setting.
    if (update) setSwing(getSwing() ^ kFujitsuAcSwingHoriz);
    // and set the appropriate special command.
    setCmd(kFujitsuAcCmdToggleSwingHoriz);
}

void Ir_Fujitsu::stepVert() {
    setCmd(kFujitsuAcCmdStepVert);
}

void Ir_Fujitsu::toggleSwingVert(const bool update) {
    // Toggle the current setting.
    if (update) setSwing(getSwing() ^ kFujitsuAcSwingVert);
    // and set the appropriate special command.
    setCmd(kFujitsuAcCmdToggleSwingVert);
}

void Ir_Fujitsu::setCmd(const uint8_t cmd) {
    switch (cmd) {
        case kFujitsuAcCmdTurnOff:
        case kFujitsuAcCmdTurnOn:
        case kFujitsuAcCmdStayOn:
        case kFujitsuAcCmdStepVert:
        case kFujitsuAcCmdToggleSwingVert:
            _cmd = cmd;
            break;
        case kFujitsuAcCmdStepHoriz:
        case kFujitsuAcCmdToggleSwingHoriz:
            switch (_model) {
                // Only these remotes have horizontal.
                case fujitsu_ac_remote_model_t::ARRAH2E:
                case fujitsu_ac_remote_model_t::ARJW2:
                    _cmd = cmd;
                    break;
                default:
                    _cmd = kFujitsuAcCmdStayOn;
            }
            break;
        case kFujitsuAcCmdEcono:
        case kFujitsuAcCmdPowerful:
            switch (_model) {
                // Only these remotes have these commands.
                case ARREB1E:
                    _cmd = cmd;
                    break;
                default:
                    _cmd = kFujitsuAcCmdStayOn;
            }
            break;
        default:
            _cmd = kFujitsuAcCmdStayOn;
    }
}

uint8_t Ir_Fujitsu::getCmd(const bool raw) {
    if (raw) return remote_state[5];
    return _cmd;
}

void Ir_Fujitsu::setTemp(const uint8_t temp) {
    _temp = std::max((uint8_t)kFujitsuAcMinTemp, temp);
    _temp = std::min((uint8_t)kFujitsuAcMaxTemp, _temp);
    setCmd(kFujitsuAcCmdStayOn);  // No special command involved.
}

uint8_t Ir_Fujitsu::getTemp() {
    return _temp;
}

void Ir_Fujitsu::setFanSpeed(const uint8_t fan) {
    if (fan > kFujitsuAcFanQuiet)
        _fanSpeed = kFujitsuAcFanHigh;  // Set the fan to maximum if out of range.
    else
        _fanSpeed = fan;
    setCmd(kFujitsuAcCmdStayOn);  // No special command involved.
}

uint8_t Ir_Fujitsu::getFanSpeed() {
    return _fanSpeed;
}

void Ir_Fujitsu::setMode(const uint8_t mode) {
    if (mode > kFujitsuAcModeHeat)
        _mode = kFujitsuAcModeHeat;  // Set the mode to maximum if out of range.
    else
        _mode = mode;
    setCmd(kFujitsuAcCmdStayOn);  // No special command involved.
}

uint8_t Ir_Fujitsu::getMode() {
    return _mode;
}

void Ir_Fujitsu::setSwing(const uint8_t swingMode) {
    _swingMode = swingMode;
    switch (_model) {
        // No Horizontal support.
        case fujitsu_ac_remote_model_t::ARDB1:
        case fujitsu_ac_remote_model_t::ARREB1E:
        case fujitsu_ac_remote_model_t::ARRY4:
            // Set the mode to max if out of range
            if (swingMode > kFujitsuAcSwingVert) _swingMode = kFujitsuAcSwingVert;
            break;
            // Has Horizontal support.
        case fujitsu_ac_remote_model_t::ARRAH2E:
        case fujitsu_ac_remote_model_t::ARJW2:
        default:
            // Set the mode to max if out of range
            if (swingMode > kFujitsuAcSwingBoth) _swingMode = kFujitsuAcSwingBoth;
    }
    setCmd(kFujitsuAcCmdStayOn);  // No special command involved.
}

uint8_t Ir_Fujitsu::getSwing(const bool raw) {
    if (raw) _swingMode = GETBITS8(remote_state[10], kHighNibble,
                                   kFujitsuAcSwingSize);
    return _swingMode;
}

uint8_t * Ir_Fujitsu::getRaw() {
    buildState();
    return remote_state;
}

bool Ir_Fujitsu::setRaw(const uint8_t *newState, const uint16_t length) {
    if (length > kFujitsuAcStateLength) return false;
    for (uint16_t i = 0; i < kFujitsuAcStateLength; i++) {
        if (i < length)
            remote_state[i] = newState[i];
        else
            remote_state[i] = 0;
    }
    buildFromState(length);
    return true;
}

uint8_t Ir_Fujitsu::getStateLength() {
    buildState();  // Force an update of the internal state.
    if (((_model == fujitsu_ac_remote_model_t::ARRAH2E ||
          _model == fujitsu_ac_remote_model_t::ARREB1E ||
          _model == fujitsu_ac_remote_model_t::ARRY4) &&
         remote_state[5] != 0xFE) ||
        ((_model == fujitsu_ac_remote_model_t::ARDB1 ||
          _model == fujitsu_ac_remote_model_t::ARJW2) && remote_state[5] != 0xFC))
        return _state_length_short;
    else
        return _state_length;
}

bool Ir_Fujitsu::validChecksum(uint8_t *state, const uint16_t length) {
    uint8_t sum = 0;
    uint8_t sum_complement = 0;
    uint8_t checksum = state[length - 1];
    switch (length) {
        case kFujitsuAcStateLengthShort:  // ARRAH2E, ARREB1E, & ARRY4
            return state[length - 1] == (uint8_t)~state[length - 2];
        case kFujitsuAcStateLength - 1:  // ARDB1 & ARJW2
            sum = sumBytes(state, length - 1);
            sum_complement = 0x9B;
            break;
        case kFujitsuAcStateLength:  // ARRAH2E, ARRY4, & ARREB1E
            sum = sumBytes(state + kFujitsuAcStateLengthShort,
                           length - 1 - kFujitsuAcStateLengthShort);
            break;
        default:        // Includes ARDB1 & ARJW2 short.
            return true;  // Assume the checksum is valid for other lengths.
    }
    return checksum == (uint8_t)(sum_complement - sum);  // Does it match?
}

void Ir_Fujitsu::setPower(const bool on) {
    setCmd(on ? kFujitsuAcCmdTurnOn : kFujitsuAcCmdTurnOff);
}

bool Ir_Fujitsu::getPower() {
    return _cmd != kFujitsuAcCmdTurnOff;
}

void Ir_Fujitsu::off() {
    setPower(false);
}

void Ir_Fujitsu::on() {
    setPower(true);
}

void Ir_Fujitsu::setClean(const bool on) {
    _clean = on;
    setCmd(kFujitsuAcCmdStayOn);  // No special command involved.
}

bool Ir_Fujitsu::getClean(const bool raw) {
    if (raw) {
        return GETBIT8(remote_state[9], kFujitsuAcCleanOffset);
    } else {
        switch (getModel()) {
            case fujitsu_ac_remote_model_t::ARRY4: return _clean;
            default: return false;
        }
    }
}

void Ir_Fujitsu::setFilter(const bool on) {
    _filter = on;
    setCmd(kFujitsuAcCmdStayOn);  // No special command involved.
}

bool Ir_Fujitsu::getFilter(const bool raw) {
    if (raw) {
        return GETBIT8(remote_state[14], kFujitsuAcFilterOffset);
    } else {
        switch (getModel()) {
            case fujitsu_ac_remote_model_t::ARRY4: return _filter;
            default: return false;
        }
    }
}

void Ir_Fujitsu::setOutsideQuiet(const bool on) {
    _outsideQuiet = on;
    setCmd(kFujitsuAcCmdStayOn);  // No special command involved.
}

bool Ir_Fujitsu::getOutsideQuiet(const bool raw) {
    if ((_state_length == kFujitsuAcStateLength) && raw) {
        _outsideQuiet = GETBIT8(remote_state[14], kFujitsuAcOutsideQuietOffset);
        // Only ARREB1E seems to have this mode.
        if (_outsideQuiet) setModel(fujitsu_ac_remote_model_t::ARREB1E);
    }
    return _outsideQuiet;
}

uint8_t Ir_Fujitsu::getTimerType(const bool raw) {
    switch (getModel()) {
        // These models seem to have timer support.
        case fujitsu_ac_remote_model_t::ARRAH2E:
        case fujitsu_ac_remote_model_t::ARREB1E:
            if (raw)
                return GETBITS8(remote_state[kFujitsuAcTimerTypeByte],
                                kFujitsuAcTimerTypeOffset, kFujitsuAcTimerTypeSize);
            else
                return _timertype;
        default: return kFujitsuAcStopTimers;
    }
}

void Ir_Fujitsu::setTimerType(const uint8_t timertype) {
    switch (timertype) {
        case kFujitsuAcSleepTimer:
        case kFujitsuAcOnTimer:
        case kFujitsuAcOffTimer:
        case kFujitsuAcStopTimers:
            _timertype = timertype;
            break;
        default: setTimerType(kFujitsuAcStopTimers);
    }
}

uint16_t Ir_Fujitsu::getOnTimer(const bool raw) {
    if (getTimerType(raw) == kFujitsuAcOnTimer) {
        if (raw)
            return (GETBITS8(remote_state[13], 0, 7) << 4) +
                   GETBITS8(remote_state[12], 4, 4);
        else
            return _ontimer;
    } else {
        return 0;
    }
}

void Ir_Fujitsu::setOnTimer(const uint16_t nr_mins) {
    _ontimer = std::min(kFujitsuAcTimerMax, nr_mins);  // Bounds check.
    if (_ontimer) {
        setTimerType(kFujitsuAcOnTimer);
    } else {
        if (getTimerType() == kFujitsuAcOnTimer) setTimerType(kFujitsuAcStopTimers);
    }
}

uint16_t Ir_Fujitsu::getOffSleepTimer(const bool raw) {
    switch (getTimerType(raw)) {
        case kFujitsuAcOffTimer:
        case kFujitsuAcSleepTimer:
            if (raw)
                return (GETBITS8(remote_state[12], 0, 3) << 8) +
                       remote_state[11];
            else
                return _offtimer;
        default:
            return 0;
    }
}

void Ir_Fujitsu::setOffTimer(const uint16_t nr_mins) {
    setOffSleepTimer(nr_mins);
    if (nr_mins)
        setTimerType(kFujitsuAcOffTimer);
    else
    if (getTimerType() != kFujitsuAcOnTimer) setTimerType(kFujitsuAcStopTimers);
}

void Ir_Fujitsu::setSleepTimer(const uint16_t nr_mins) {
    setOffSleepTimer(nr_mins);
    if (nr_mins)
        setTimerType(kFujitsuAcSleepTimer);
    else
    if (getTimerType() != kFujitsuAcOnTimer) setTimerType(kFujitsuAcStopTimers);
}

void Ir_Fujitsu::buildState(void) {
    remote_state[0] = 0x14;
    remote_state[1] = 0x63;
    remote_state[2] = 0x00;
    remote_state[3] = 0x10;
    remote_state[4] = 0x10;
    bool fullCmd = false;
    switch (_cmd) {
        case kFujitsuAcCmdTurnOff:     // 0x02
        case kFujitsuAcCmdEcono:       // 0x09
        case kFujitsuAcCmdPowerful:    // 0x39
        case kFujitsuAcCmdStepVert:    // 0x6C
        case kFujitsuAcCmdToggleSwingVert:   // 0x6D
        case kFujitsuAcCmdStepHoriz:   // 0x79
        case kFujitsuAcCmdToggleSwingHoriz:  // 0x7A
            remote_state[5] = _cmd;
            break;
        default:
            switch (_model) {
                case fujitsu_ac_remote_model_t::ARRY4:
                case fujitsu_ac_remote_model_t::ARRAH2E:
                case fujitsu_ac_remote_model_t::ARREB1E:
                    remote_state[5] = 0xFE;
                    break;
                case fujitsu_ac_remote_model_t::ARDB1:
                case fujitsu_ac_remote_model_t::ARJW2:
                    remote_state[5] = 0xFC;
                    break;
            }
            fullCmd = true;
            break;
    }
    if (fullCmd) {  // long codes
        uint8_t tempByte = _temp - kFujitsuAcMinTemp;
        // Nr. of bytes in the message after this byte.
        remote_state[6] = _state_length - 7;

        remote_state[7] = 0x30;
        remote_state[8] = (_cmd == kFujitsuAcCmdTurnOn) | (tempByte << 4);
        remote_state[9] = _mode | (getTimerType() << 4);
        remote_state[10] = _fanSpeed;

        // Set the On/Off/Sleep timer Nr of mins.
        remote_state[11] = getOffSleepTimer();
        remote_state[12] = ((getOnTimer() & 0x0F) << 4) |
                           ((getOffSleepTimer() >> 8) & 0x07);
        remote_state[13] = getOnTimer() >> 4;
        // Enable bit for the Off/Sleep timer
        setBit(&remote_state[12], 3, getOffSleepTimer());
        // Enable bit for the On timer
        setBit(&remote_state[13], 7, getOnTimer());

        switch (_model) {
            case fujitsu_ac_remote_model_t::ARRY4:
                remote_state[14] = _filter << 3;
                remote_state[9] |= (_clean << 3);
                break;
            default:
                remote_state[14] = 0;
        }
        uint8_t checksum = 0;
        uint8_t checksum_complement = 0;
        switch (_model) {
            case fujitsu_ac_remote_model_t::ARDB1:
            case fujitsu_ac_remote_model_t::ARJW2:
                checksum = sumBytes(remote_state, _state_length - 1);
                checksum_complement = 0x9B;
                break;
            case fujitsu_ac_remote_model_t::ARREB1E:
                setBit(&remote_state[14], kFujitsuAcOutsideQuietOffset, _outsideQuiet);
                // FALL THRU
                break;
            case fujitsu_ac_remote_model_t::ARRAH2E:
            case fujitsu_ac_remote_model_t::ARRY4:
                setBit(&remote_state[14], 5);  // |= 0b00100000
                setBits(&remote_state[10], kHighNibble, kFujitsuAcSwingSize,
                        _swingMode);
                // FALL THRU
                break;
            default:
                checksum = sumBytes(remote_state + _state_length_short,
                                    _state_length - _state_length_short - 1);
        }
        // and negate the checksum and store it in the last byte.
        remote_state[_state_length - 1] = checksum_complement - checksum;
    } else {  // short codes
        switch (_model) {
            case fujitsu_ac_remote_model_t::ARRY4:
            case fujitsu_ac_remote_model_t::ARRAH2E:
            case fujitsu_ac_remote_model_t::ARREB1E:
                // The last byte is the inverse of penultimate byte
                remote_state[_state_length_short - 1] =
                        ~remote_state[_state_length_short - 2];
                break;
            default:
            {};  // We don't need to do anything for the others.
        }
        // Zero the rest of the state.
        for (uint8_t i = _state_length_short; i < kFujitsuAcStateLength; i++)
            remote_state[i] = 0;
    }
}

void Ir_Fujitsu::buildFromState(const uint16_t length) {
    switch (length) {
        case kFujitsuAcStateLength - 1:
        case kFujitsuAcStateLengthShort - 1:
            setModel(fujitsu_ac_remote_model_t::ARDB1);
            // ARJW2 has horizontal swing.
            if (getSwing(true) > kFujitsuAcSwingVert)
                setModel(fujitsu_ac_remote_model_t::ARJW2);
            break;
        default:
            switch (getCmd(true)) {
                case kFujitsuAcCmdEcono:
                case kFujitsuAcCmdPowerful:
                    setModel(fujitsu_ac_remote_model_t::ARREB1E);
                    break;
                default:
                    setModel(fujitsu_ac_remote_model_t::ARRAH2E);
            }
    }
    switch (remote_state[6]) {
        case 8:
            if (getModel() != fujitsu_ac_remote_model_t::ARJW2)
                setModel(fujitsu_ac_remote_model_t::ARDB1);
            break;
        case 9:
            if (getModel() != fujitsu_ac_remote_model_t::ARREB1E)
                setModel(fujitsu_ac_remote_model_t::ARRAH2E);
            break;
    }
    setTemp((remote_state[8] >> 4) + kFujitsuAcMinTemp);
    if (GETBIT8(remote_state[8], 0))
        setCmd(kFujitsuAcCmdTurnOn);
    else
        setCmd(kFujitsuAcCmdStayOn);
    setMode(GETBITS8(remote_state[9], kLowNibble, kModeBitsSize));
    setFanSpeed(GETBITS8(remote_state[10], kLowNibble, kFujitsuAcFanSize));
    setSwing(GETBITS8(remote_state[10], kHighNibble, kFujitsuAcSwingSize));
    setClean(getClean(true));
    setFilter(getFilter(true));
    // Currently the only way we know how to tell ARRAH2E & ARRY4 apart is if
    // either the raw Filter or Clean setting is on.
    if (getModel() == fujitsu_ac_remote_model_t::ARRAH2E && (getFilter(true) ||
                                                             getClean(true)))
        setModel(fujitsu_ac_remote_model_t::ARRY4);
    switch (remote_state[5]) {
        case kFujitsuAcCmdTurnOff:
        case kFujitsuAcCmdStepHoriz:
        case kFujitsuAcCmdToggleSwingHoriz:
        case kFujitsuAcCmdStepVert:
        case kFujitsuAcCmdToggleSwingVert:
        case kFujitsuAcCmdEcono:
        case kFujitsuAcCmdPowerful:
            setCmd(remote_state[5]);
            break;
    }
    _outsideQuiet = getOutsideQuiet(true);
    // Timers
    switch (getModel()) {
        // These models seem to have timer support.
        case fujitsu_ac_remote_model_t::ARRAH2E:
        case fujitsu_ac_remote_model_t::ARREB1E:
            _offtimer = getOffSleepTimer(true);
            _ontimer = getOnTimer(true);
            _timertype = getTimerType(true);
            break;
        default: break;
    }
}

void Ir_Fujitsu::setOffSleepTimer(const uint16_t nr_mins) {
    _offtimer = std::min(kFujitsuAcTimerMax, nr_mins);  // Bounds check.
}
/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/


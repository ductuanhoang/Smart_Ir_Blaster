/*
 * IrFujitsu.h
 *
 *  Created on: Jan 14, 2021
 *      Author: ductu
 */

#ifndef MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRFUJITSU_H_
#define MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRFUJITSU_H_

#include "IrSend.h"
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
// Constants
const uint8_t kFujitsuAcModeAuto = 0x00;
const uint8_t kFujitsuAcModeCool = 0x01;
const uint8_t kFujitsuAcModeDry = 0x02;
const uint8_t kFujitsuAcModeFan = 0x03;
const uint8_t kFujitsuAcModeHeat = 0x04;

const uint8_t kFujitsuAcCmdStayOn = 0x00;            // b00000000
const uint8_t kFujitsuAcCmdTurnOn = 0x01;            // b00000001
const uint8_t kFujitsuAcCmdTurnOff = 0x02;           // b00000010
const uint8_t kFujitsuAcCmdEcono = 0x09;             // b00001001
const uint8_t kFujitsuAcCmdPowerful = 0x39;          // b00111001
const uint8_t kFujitsuAcCmdStepVert = 0x6C;          // b01101100
const uint8_t kFujitsuAcCmdToggleSwingVert = 0x6D;   // b01101101
const uint8_t kFujitsuAcCmdStepHoriz = 0x79;         // b01111001
const uint8_t kFujitsuAcCmdToggleSwingHoriz = 0x7A;  // b01111010

const uint8_t kFujitsuAcFanAuto = 0x00;
const uint8_t kFujitsuAcFanHigh = 0x01;
const uint8_t kFujitsuAcFanMed = 0x02;
const uint8_t kFujitsuAcFanLow = 0x03;
const uint8_t kFujitsuAcFanQuiet = 0x04;
const uint8_t kFujitsuAcFanSize = 3;  // Bits

const uint8_t kFujitsuAcMinTemp = 16;  // 16C
const uint8_t kFujitsuAcMaxTemp = 30;  // 30C

const uint8_t kFujitsuAcSwingSize = 2;
const uint8_t kFujitsuAcSwingOff = 0x00;
const uint8_t kFujitsuAcSwingVert = 0x01;
const uint8_t kFujitsuAcSwingHoriz = 0x02;
const uint8_t kFujitsuAcSwingBoth = 0x03;

const uint8_t kFujitsuAcOutsideQuietOffset = 7;
const uint8_t kFujitsuAcCleanOffset = 3;
const uint8_t kFujitsuAcFilterOffset = 3;

const uint8_t kFujitsuAcTimerTypeByte = 9;
const uint8_t kFujitsuAcTimerTypeOffset = 4;  ///< Mask: 0b00xx0000
const uint8_t kFujitsuAcTimerTypeSize = 2;    ///< Mask: 0b00xx0000
const uint8_t kFujitsuAcStopTimers =                       0b00;  // 0
const uint8_t kFujitsuAcSleepTimer =                       0b01;  // 1
const uint8_t kFujitsuAcOffTimer =                         0b10;  // 2
const uint8_t kFujitsuAcOnTimer =                          0b11;  // 3
const uint16_t kFujitsuAcTimerMax = 12 * 60;  ///< Minutes.

// Legacy defines.
#define FUJITSU_AC_MODE_AUTO kFujitsuAcModeAuto
#define FUJITSU_AC_MODE_COOL kFujitsuAcModeCool
#define FUJITSU_AC_MODE_DRY kFujitsuAcModeDry
#define FUJITSU_AC_MODE_FAN kFujitsuAcModeFan
#define FUJITSU_AC_MODE_HEAT kFujitsuAcModeHeat
#define FUJITSU_AC_CMD_STAY_ON kFujitsuAcCmdStayOn
#define FUJITSU_AC_CMD_TURN_ON kFujitsuAcCmdTurnOn
#define FUJITSU_AC_CMD_TURN_OFF kFujitsuAcCmdTurnOff
#define FUJITSU_AC_CMD_STEP_HORIZ kFujitsuAcCmdStepHoriz
#define FUJITSU_AC_CMD_STEP_VERT kFujitsuAcCmdStepVert
#define FUJITSU_AC_FAN_AUTO kFujitsuAcFanAuto
#define FUJITSU_AC_FAN_HIGH kFujitsuAcFanHigh
#define FUJITSU_AC_FAN_MED kFujitsuAcFanMed
#define FUJITSU_AC_FAN_LOW kFujitsuAcFanLow
#define FUJITSU_AC_FAN_QUIET kFujitsuAcFanQuiet
#define FUJITSU_AC_MIN_TEMP kFujitsuAcMinTemp
#define FUJITSU_AC_MAX_TEMP kFujitsuAcMaxTemp
#define FUJITSU_AC_SWING_OFF kFujitsuAcSwingOff
#define FUJITSU_AC_SWING_VERT kFujitsuAcSwingVert
#define FUJITSU_AC_SWING_HORIZ kFujitsuAcSwingHoriz
#define FUJITSU_AC_SWING_BOTH kFujitsuAcSwingBoth
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
/// Fujitsu A/C model numbers
enum fujitsu_ac_remote_model_t {
  ARRAH2E = 1,  // (1) AR-RAH2E, AR-RAC1E, AR-RAE1E, AR-RCE1E (Default)
  ARDB1,        // (2) AR-DB1, AR-DL10 (AR-DL10 swing doesn't work)
  ARREB1E,      // (3) AR-REB1E
  ARJW2,        // (4) AR-JW2  (Same as ARDB1 but with horiz control)
  ARRY4,        // (5) AR-RY4 (Same as AR-RAH2E but with clean & filter)
};
/****************************************************************************/
/***         Exported global functions                                     ***/
/****************************************************************************/
class Ir_Fujitsu {
public:
	Ir_Fujitsu(const fujitsu_ac_remote_model_t model, uint32_t frequency, uint16_t dutycycle);
	virtual ~Ir_Fujitsu();
    void sendFujitsu(const uint16_t repeat = kFujitsuAcMinRepeat);
    void setModel(const fujitsu_ac_remote_model_t model);
    fujitsu_ac_remote_model_t getModel(void);
    void stateReset(void);
    void setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp);
private:
    uint8_t convertFan(stdAc::fanspeed_t speed);
    uint8_t convertMode(const stdAc::opmode_t mode);
    void send(const unsigned char data[], const uint16_t nbytes,
            const uint16_t repeat = kFujitsuAcMinRepeat);

    void stepHoriz(void);
    void toggleSwingHoriz(const bool update = true);
    void stepVert(void);
    void toggleSwingVert(const bool update = true);
    void setCmd(const uint8_t cmd);
    uint8_t getCmd(const bool raw = false);
    void setTemp(const uint8_t temp);
    uint8_t getTemp(void);
    void setFanSpeed(const uint8_t fan);
    uint8_t getFanSpeed(void);
    void setMode(const uint8_t mode);
    uint8_t getMode(void);
    void setSwing(const uint8_t swingMode);
    uint8_t getSwing(const bool raw = false);
    uint8_t* getRaw(void);
    bool setRaw(const uint8_t newState[], const uint16_t length);
    uint8_t getStateLength(void);
    static bool validChecksum(uint8_t* state, const uint16_t length);
    void setPower(const bool on);
    void off(void);
    void on(void);
    bool getPower(void);
    void setClean(const bool on);
    bool getClean(const bool raw = false);
    void setFilter(const bool on);
    bool getFilter(const bool raw = false);
    void setOutsideQuiet(const bool on);
    bool getOutsideQuiet(const bool raw = false);
    uint8_t getTimerType(const bool raw = false);
    void setTimerType(const uint8_t timertype);
    uint16_t getOnTimer(const bool raw = false);
    void setOnTimer(const uint16_t nr_mins);
    uint16_t getOffSleepTimer(const bool raw = false);
    void setOffTimer(const uint16_t nr_mins);
    void setSleepTimer(const uint16_t nr_mins);

    uint8_t remote_state[kFujitsuAcStateLength];  ///< The state of the IR remote.
    uint8_t _temp;
    uint8_t _fanSpeed;
    uint8_t _mode;
    uint8_t _swingMode;
    uint8_t _cmd;
    fujitsu_ac_remote_model_t _model;
    uint8_t _state_length;
    uint8_t _state_length_short;
    bool _outsideQuiet;
    bool _clean;
    bool _filter;
    uint16_t _ontimer;
    uint16_t _offtimer;  // Also is the sleep timer value
    uint8_t _timertype;
    void buildState(void);
    void buildFromState(const uint16_t length);
    void setOffSleepTimer(const uint16_t nr_mins);

    Ir_Send _ir_fujitsu_ac;
};
#endif /* MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRFUJITSU_H_ */

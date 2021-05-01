/*
 * IrGree.h
 *
 *  Created on: Mar 9, 2021
 *      Author: ductu
 */

#ifndef MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRGREE_H_
#define MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRGREE_H_


/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "IrSend.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
/// Native representation of a Gree A/C message.
union GreeProtocol{
    uint8_t remote_state[kGreeStateLength];  ///< The state in native IR code form
    struct {
        // Byte 0
        uint8_t Mode      :3;
        uint8_t Power     :1;
        uint8_t Fan       :2;
        uint8_t SwingAuto :1;
        uint8_t Sleep     :1;
        // Byte 1
        uint8_t Temp        :4;
        uint8_t TimerHalfHr :1;
        uint8_t TimerTensHr :2;
        uint8_t TimerEnabled:1;
        // Byte 2
        uint8_t TimerHours:4;
        uint8_t Turbo     :1;
        uint8_t Light     :1;
        uint8_t ModelA    :1;  // model==YAW1F
        uint8_t Xfan      :1;
        // Byte 3
        uint8_t :2;
        uint8_t TempExtraDegreeF:1;
        uint8_t UseFahrenheit   :1;
        uint8_t unknown1        :4;  // value=0b0101
        // Byte 4
        uint8_t Swing:4;
        uint8_t :0;
        // Byte 5
        uint8_t DisplayTemp :2;
        uint8_t IFeel       :1;
        uint8_t unknown2    :3;  // value = 0b100
        uint8_t WiFi        :1;
        uint8_t :0;
        // Byte 6
        uint8_t :8;
        // Byte 7
        uint8_t :4;
        uint8_t Sum:4;
    };
};

// Constants

const uint8_t kGreeAuto = 0;
const uint8_t kGreeCool = 1;
const uint8_t kGreeDry  = 2;
const uint8_t kGreeFan  = 3;
const uint8_t kGreeHeat = 4;

const uint8_t kGreeFanAuto = 0;
const uint8_t kGreeFanMin  = 1;
const uint8_t kGreeFanMed  = 2;
const uint8_t kGreeFanMax  = 3;

const uint8_t kGreeMinTempC = 16;  // Celsius
const uint8_t kGreeMaxTempC = 30;  // Celsius
const uint8_t kGreeMinTempF = 61;  // Fahrenheit
const uint8_t kGreeMaxTempF = 86;  // Fahrenheit
const uint16_t kGreeTimerMax = 24 * 60;

const uint8_t kGreeSwingLastPos    = 0b0000;
const uint8_t kGreeSwingAuto       = 0b0001;
const uint8_t kGreeSwingUp         = 0b0010;
const uint8_t kGreeSwingMiddleUp   = 0b0011;
const uint8_t kGreeSwingMiddle     = 0b0100;
const uint8_t kGreeSwingMiddleDown = 0b0101;
const uint8_t kGreeSwingDown       = 0b0110;
const uint8_t kGreeSwingDownAuto   = 0b0111;
const uint8_t kGreeSwingMiddleAuto = 0b1001;
const uint8_t kGreeSwingUpAuto     = 0b1011;

const uint8_t kGreeDisplayTempOff     = 0b00;  // 0
const uint8_t kGreeDisplayTempSet     = 0b01;  // 1
const uint8_t kGreeDisplayTempInside  = 0b10;  // 2
const uint8_t kGreeDisplayTempOutside = 0b11;  // 3

// Legacy defines.
#define GREE_AUTO kGreeAuto
#define GREE_COOL kGreeCool
#define GREE_DRY kGreeDry
#define GREE_FAN kGreeFan
#define GREE_HEAT kGreeHeat
#define GREE_MIN_TEMP kGreeMinTempC
#define GREE_MAX_TEMP kGreeMaxTempC
#define GREE_FAN_MAX kGreeFanMax
#define GREE_SWING_LAST_POS kGreeSwingLastPos
#define GREE_SWING_AUTO kGreeSwingAuto
#define GREE_SWING_UP kGreeSwingUp
#define GREE_SWING_MIDDLE_UP kGreeSwingMiddleUp
#define GREE_SWING_MIDDLE kGreeSwingMiddle
#define GREE_SWING_MIDDLE_DOWN kGreeSwingMiddleDown
#define GREE_SWING_DOWN kGreeSwingDown
#define GREE_SWING_DOWN_AUTO kGreeSwingDownAuto
#define GREE_SWING_MIDDLE_AUTO kGreeSwingMiddleAuto
#define GREE_SWING_UP_AUTO kGreeSwingUpAuto

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
/// Gree A/C model numbers
enum gree_ac_remote_model_t {
    YAW1F = 1,  // (1) Ultimate, EKOKAI, RusClimate (Default)
    YBOFB,     // (2) Green, YBOFB2, YAPOF3
};
/****************************************************************************/
/***         Exported global functions                                     ***/
/****************************************************************************/
class Ir_Gree {
public:
	Ir_Gree(uint32_t frequency, uint16_t dutycycle);
	virtual ~Ir_Gree();
    void sendGree(const uint16_t repeat = kGreeDefaultRepeat);
    void setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp);
private:
    uint8_t convertFan(const stdAc::fanspeed_t speed);
    uint8_t convertMode(const stdAc::opmode_t mode);
    void stateReset(void);

    void send(const uint64_t data, const uint16_t nbits = kGreeBits,
                  const uint16_t repeat = kGreeDefaultRepeat);
    void send(const uint8_t data[], const uint16_t nbytes = kGreeStateLength,
                  const uint16_t repeat = kGreeDefaultRepeat);
    void on(void);
    void off(void);
    void setModel(const gree_ac_remote_model_t model);
    gree_ac_remote_model_t getModel(void) const;
    void setPower(const bool on);
    bool getPower(void) const;
    void setTemp(const uint8_t temp, const bool fahrenheit = false);
    uint8_t getTemp(void) const;
    void setUseFahrenheit(const bool on);
    bool getUseFahrenheit(void) const;
    void setFan(const uint8_t speed);
    uint8_t getFan(void) const;
    void setMode(const uint8_t new_mode);
    uint8_t getMode(void) const;
    void setLight(const bool on);
    bool getLight(void) const;
    void setXFan(const bool on);
    bool getXFan(void) const;
    void setSleep(const bool on);
    bool getSleep(void) const;
    void setTurbo(const bool on);
    bool getTurbo(void) const;
    void setIFeel(const bool on);
    bool getIFeel(void) const;
    void setWiFi(const bool on);
    bool getWiFi(void) const;
    void setSwingVertical(const bool automatic, const uint8_t position);
    bool getSwingVerticalAuto(void) const;
    uint8_t getSwingVerticalPosition(void) const;
    uint16_t getTimer(void) const;
    void setTimer(const uint16_t minutes);
    void setDisplayTempSource(const uint8_t mode);
    uint8_t getDisplayTempSource(void) const;

    uint8_t* getRaw(void);
    void setRaw(const uint8_t new_code[]);
    static bool validChecksum(const uint8_t state[],
                              const uint16_t length = kGreeStateLength);

    GreeProtocol _;
    gree_ac_remote_model_t _model;
    void checksum(const uint16_t length = kGreeStateLength);
    void fixup(void);
    void setTimerEnabled(const bool on);
    bool getTimerEnabled(void) const;
private:
    Ir_Send _ir_gree;
};
#endif /* MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRGREE_H_ */

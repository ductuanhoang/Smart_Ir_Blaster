/*
 * IrKelvinator.h
 *
 *  Created on: Jan 19, 2021
 *      Author: ductu
 */

#ifndef MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRKELVINATOR_H_
#define MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRKELVINATOR_H_


/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "IrSend.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
/// Native representation of a Kelvinator A/C message.
union KelvinatorProtocol{
    uint8_t raw[kKelvinatorStateLength];  ///< The state in IR code form.
    struct {
        // Byte 0
        uint8_t Mode      :3;
        uint8_t Power     :1;
        uint8_t BasicFan  :2;
        uint8_t VentSwing :1;
        uint8_t           :1;  // Sleep Modes 1 & 3 (1 = On, 0 = Off)
        // Byte 1
        uint8_t Temp  :4;  // Degrees C.
        uint8_t       :4;
        // Byte 2
        uint8_t           :4;
        uint8_t Turbo     :1;
        uint8_t Light     :1;
        uint8_t IonFilter :1;
        uint8_t XFan      :1;
        // Byte 3
        uint8_t :4;
        uint8_t :2;  // (possibly timer related) (Typically 0b01)
        uint8_t :2;  // End of command block (B01)
        // (B010 marker and a gap of 20ms)
        // Byte 4
        uint8_t SwingV  :1;
        uint8_t         :3;
        uint8_t SwingH  :1;
        uint8_t         :3;
        // Byte 5~6
        uint8_t pad0[2];  // Timer related. Typically 0 except when timer in use.
        // Byte 7
        uint8_t       :4;  // (Used in Timer mode)
        uint8_t Sum1  :4;  // checksum of the previous bytes (0-6)
        // (gap of 40ms)
        // (header mark and space)
        // Byte 8~10
        uint8_t pad1[3];  // Repeat of byte 0~2
        // Byte 11
        uint8_t :4;
        uint8_t :2;  // (possibly timer related) (Typically 0b11)
        uint8_t :2;  // End of command block (B01)
        // (B010 marker and a gap of 20ms)
        // Byte 12
        uint8_t       :1;  // Sleep mode 2 (1 = On, 0=Off)
        uint8_t       :6;  // (Used in Sleep Mode 3, Typically 0b000000)
        uint8_t Quiet :1;
        // Byte 13
        uint8_t     :8;  // (Sleep Mode 3 related, Typically 0x00)
        // Byte 14
        uint8_t     :4;  // (Sleep Mode 3 related, Typically 0b0000)
        uint8_t Fan :3;
        // Byte 15
        uint8_t       :4;
        uint8_t Sum2  :4;  // checksum of the previous bytes (8-14)
    };
};

// Constants
const uint8_t kKelvinatorAuto = 0;  // (temp = 25C)
const uint8_t kKelvinatorCool = 1;
const uint8_t kKelvinatorDry = 2;  // (temp = 25C, but not shown)
const uint8_t kKelvinatorFan = 3;
const uint8_t kKelvinatorHeat = 4;
const uint8_t kKelvinatorBasicFanMax = 3;
const uint8_t kKelvinatorFanAuto = 0;
const uint8_t kKelvinatorFanMin = 1;
const uint8_t kKelvinatorFanMax = 5;
const uint8_t kKelvinatorMinTemp = 16;   // 16C
const uint8_t kKelvinatorMaxTemp = 30;   // 30C
const uint8_t kKelvinatorAutoTemp = 25;  // 25C

// Legacy defines (Deprecated)
#define KELVINATOR_MIN_TEMP kKelvinatorMinTemp
#define KELVINATOR_MAX_TEMP kKelvinatorMaxTemp
#define KELVINATOR_HEAT kKelvinatorHeat
#define KELVINATOR_FAN_MAX kKelvinatorFanMax
#define KELVINATOR_FAN_AUTO kKelvinatorFanAuto
#define KELVINATOR_FAN kKelvinatorFan
#define KELVINATOR_DRY kKelvinatorDry
#define KELVINATOR_COOL kKelvinatorCool
#define KELVINATOR_BASIC_FAN_MAX kKelvinatorBasicFanMax
#define KELVINATOR_AUTO_TEMP kKelvinatorAutoTemp
#define KELVINATOR_AUTO kKelvinatorAuto
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***         Exported global functions                                     ***/
/****************************************************************************/
class Ir_Kelvinator {
public:
	Ir_Kelvinator(uint32_t frequency, uint16_t dutycycle);
	virtual ~Ir_Kelvinator();
	void sendKelvinator(const uint16_t repeat = kKelvinatorDefaultRepeat);
	void setparam(uint8_t power, uint8_t mode, uint8_t speed, uint8_t temp);
	uint8_t convertMode(const stdAc::opmode_t mode);
    static uint8_t calcBlockChecksum(
            const uint8_t* block, const uint16_t length = kKelvinatorStateLength / 2);

	void send(const unsigned char data[],
	                    const uint16_t nbytes = kKelvinatorStateLength,
	                    const uint16_t repeat = kKelvinatorDefaultRepeat);
    void on(void);
    void off(void);
    void setPower(const bool on);
    bool getPower(void) const;
    void setTemp(const uint8_t degrees);
    uint8_t getTemp(void) const;
    void setFan(const uint8_t speed);
    uint8_t getFan(void) const;
    void setMode(const uint8_t mode);
    uint8_t getMode(void) const;
    void setSwingVertical(const bool on);
    bool getSwingVertical(void) const;
    void setSwingHorizontal(const bool on);
    bool getSwingHorizontal(void) const;
    void setQuiet(const bool on);
    bool getQuiet(void) const;
    void setIonFilter(const bool on);
    bool getIonFilter(void) const;
    void setLight(const bool on);
    bool getLight(void) const;
    void setXFan(const bool on);
    bool getXFan(void) const;
    void setTurbo(const bool on);
    bool getTurbo(void) const;
    uint8_t* getRaw(void);
    void setRaw(const uint8_t new_code[]);

    static bool validChecksum(const uint8_t state[],
                              const uint16_t length = kKelvinatorStateLength);

    void stateReset(void);
private:
    KelvinatorProtocol _;
    void checksum(void);
    void fixup(void);

	Ir_Send _ir_kelvinator_send;
};
#endif /* MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRKELVINATOR_H_ */

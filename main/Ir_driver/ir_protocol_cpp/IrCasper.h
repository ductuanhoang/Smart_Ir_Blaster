/*
 * IrCasper.h
 *
 *  Created on: Jan 12, 2021
 *      Author: ductu
 */

#ifndef MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRCASPER_H_
#define MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRCASPER_H_
#include "IrSend.h"

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef enum
{
	irCasper_SwingUpDown_ON = 0x00,
	irCasper_SwingUpDown_OFF = 0x07
}irCasper_Swing;

typedef enum
{
	irCasper_LeftRight_ON = 0x00,
	irCasper_LeftRight_OFF = 0xE0
}irCasper_LeftRight;

typedef enum
{
	irCasper_Speed_Low = 0x60,
	irCasper_Speed_Mid = 0x40,
	irCasper_Speed_High = 0x20,
	irCasper_Speed_Turbo = 0x24,
	irCasper_Speed_Auto = 0xA0
}irCasper_FanSpeed;


typedef enum
{
	irCasper_Mode_Cool = 0x20,
	irCasper_Mode_Heat = 0x80,
	irCasper_Mode_Fan = 0xC0,
	irCasper_Mode_Auto = 0x00,
	irCasper_Mode_Dry = 0x40
}irCasper_Mode;

typedef enum
{
	irCasper_Sleep_ON = 0x04,
	irCasper_Sleep_OFF = 0x00
}irCasper_SleepMode;

typedef enum
{
	irCasper_ChangeView_Celsius 	= 0x00,
	irCasper_ChangeView_Fahrenheit = 0x02
}irCasper_ChangeView;

typedef enum
{
	irCasper_Timers_OFF = 0x40,
	irCasper_Timer_ON = 0x80,
	irCasper_Timer_None = 0x00
}irCasper_Timers;

typedef enum
{
	irCasper_Power_ON = 0x20,
	irCasper_Power_OFF = 0x00
}irCasper_Power;

typedef enum
{
	irCasper_Eco_ON = 0x08,
	irCasper_Eco_OFF = 0x00
}irCasper_Eco;

typedef enum
{
	irCasper_IClean_ON = 0x04,
	irCasper_IClean_OFF = 0x00
}irCasper_iClean;

typedef enum
{
	irCasper_Health_ON = 0x02,
	irCasper_Health_OFF = 0x00
}irCasper_Health;

typedef enum
{
	irCasper_Display_Active 	= 0x01,
	irCasper_Display_None 	= 0x00
}irCasper_Display;

typedef enum
{
	irCasper_Mildew_ON = 0x08,
	irCasper_Mildew_OFF = 0x00
}irCasper_Mildew;

typedef enum
{
	irCasper_SC_Active = 0x01,
	irCasper_SC_None = 0x00
}irCasper_SmartConfig;

/// Native representation of a Daikin A/C message.
typedef struct
{
	/*byte 8,9*/
	unsigned char ucUknowByte8;
	unsigned char ucUknowByte9;
	/*byte 10*/
	unsigned char ucAirTemp;
	unsigned char ucAirSwing;
	/*byte 11*/
	unsigned char ucAirLeftRight;
	unsigned char ucUknowByte11;
	/*byte 12*/
	unsigned char ucAirPoint;
	unsigned char ucUknowByte12;
	/*byte 13*/
	unsigned char ucAirFanSpeed1;
	unsigned char ucAirTimerHour;
	/*byte 14*/
	unsigned char ucAirFanSpeed2;
	unsigned char ucUknowByte14;
	unsigned char ucAirTimerMinute;
	unsigned char ucAirFanSpeedSum;
	/*byte 15*/
	unsigned char ucAirMode;
	unsigned char ucAirSleepMode;
	unsigned char ucAirChangeView;
	unsigned char ucUknowByte15;
	/*byte 16, 17*/
	unsigned char ucUknowByte16;
	unsigned char ucUknowByte17;
	/*byte 18*/
	unsigned char ucUknowByte18;
	unsigned char ucAirTimers;
	unsigned char ucAirPower;
	unsigned char ucAirEco;
	unsigned char ucAiriClean;
	unsigned char ucAirHealth;
	/*byte 19*/
	unsigned char ucUknowByte19;
	/*byte 20*/
	unsigned char ucAirDisplay;
	unsigned char ucAirMildew;
	unsigned char ucAirSleepCurve;
	unsigned char ucUknowByte20;
	/*byte 21, 22*/
	unsigned char ucUknowByte21;
	unsigned char ucUknowByte22;
}CasperACProtocol_t;

/****************************************************************************/
/***         Exported global functions                                     ***/
/****************************************************************************/

class Ir_Casper
{
public:
	Ir_Casper(uint32_t frequency, uint16_t dutycycle);
	virtual ~Ir_Casper();
	void setparamCasper(uint8_t power, uint8_t mode, uint8_t swing, uint8_t speed_fan, uint8_t temp, uint8_t sleep_mode);
	void sendCasper(const uint16_t repeat = kNoRepeat);
private:
	void setPower(uint8_t power_onoff);
	void setTemp(uint8_t temp);
	void setFan(uint8_t fan_speed);
	void setMode(uint8_t mode);
	void setSwing(uint8_t swing);
	void setSleep(uint8_t sleep_mode);
	void PacketData(uint8_t *_msgData);
	uint8_t Cal_CheckSum(uint8_t hex_data[]);

	CasperACProtocol_t CasperACProtocol;

	Ir_Send _ir_casper_send;
};
#endif /* MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRCASPER_H_ */

/*
 * IrCasper.cpp
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
#include "IrCasper.h"

/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/
#define CASPER_REMOTE_HEADER 0xC3
#define CASPER_REMOTE_MAX_LENGTH 13 // max length of casper is 13 bytes

/* NEC Protocol */
//#define NEC_BITS              	32
#define CASPER_HEADER_HIGH_US 9000	/*!< header: positive 9ms */
#define CASPER_HEADER_LOW_US 4500	/*!< header: negative 4.5ms*/
#define CASPER_BIT_ONE_HIGH_US 560	/*!< data bit 1: positive 0.56ms */
#define CASPER_BIT_ONE_LOW_US 1690	/*!< data bit 1: negative 1.69ms */
#define CASPER_BIT_ZERO_HIGH_US 560 /*!< data bit 0: positive 0.56ms */
#define CASPER_BIT_ZERO_LOW_US 560	/*!< data bit 0: negative 0.56ms */
#define CASPER_BIT_END 560			/*!< end: positive 0.56ms */

#define CASPER_BIT_MARGIN 100 /*!< NEC parse error margin time */

#define CASPER_DATA_ITEM_NUM CASPER_REMOTE_MAX_LENGTH * 8 + 2 /*!< NEC code item number: header + 32bit data + end */
#define CASPER_BIT CASPER_REMOTE_MAX_LENGTH * 8

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
Ir_Casper::Ir_Casper(uint32_t frequency, uint16_t dutycycle): _ir_casper_send(frequency, dutycycle)
{
	// TODO Auto-generated constructor stub
}

Ir_Casper::~Ir_Casper()
{
	// TODO Auto-generated destructor stub
	APP_LOGI("~Ir_Casper call");
}

void Ir_Casper::sendCasper(const uint16_t repeat)
{
	size_t size = (sizeof(uint8_t) * CASPER_REMOTE_MAX_LENGTH);
	uint8_t *message = (uint8_t *)malloc(size);
	memset((void *)message, 0, size);

	PacketData(message);

	for (int var = 0; var < CASPER_REMOTE_MAX_LENGTH; ++var)
	{
		APP_LOGI("Casper packet = %x", message[var]);
	}
	_ir_casper_send.sendGeneric(CASPER_HEADER_HIGH_US, CASPER_HEADER_LOW_US,
				CASPER_BIT_ONE_HIGH_US, CASPER_BIT_ONE_LOW_US,
				CASPER_BIT_ZERO_HIGH_US, CASPER_BIT_ZERO_LOW_US,
				CASPER_BIT_ZERO_HIGH_US, CASPER_BIT_END,
				message, CASPER_REMOTE_MAX_LENGTH,
				38, false,
				repeat, 50);
	free(message);
}

void Ir_Casper::setparamCasper(uint8_t power, uint8_t mode, uint8_t swing, uint8_t speed_fan, uint8_t temp, uint8_t sleep_mode)
{
	setPower(power);
	setMode(mode);
	setFan(speed_fan);
	setTemp(temp);
	setSwing(swing);
	setSleep(sleep_mode);
}
/***********************************************************************************************************************
 * Static Functions
 ***********************************************************************************************************************/
void Ir_Casper::PacketData(uint8_t *_msgData)
{
	_msgData[0] = CASPER_REMOTE_HEADER;
	_msgData[1] = (CasperACProtocol.ucAirTemp) | (CasperACProtocol.ucAirSwing);
	_msgData[2] = (CasperACProtocol.ucAirLeftRight) | (CasperACProtocol.ucUknowByte11);
	_msgData[3] = 0x00;
	_msgData[4] = ((CasperACProtocol.ucAirFanSpeedSum) & 0xF0) | (CasperACProtocol.ucAirTimerHour);
	_msgData[5] = (((CasperACProtocol.ucAirFanSpeedSum) & 0x0F) << 4) | (CasperACProtocol.ucUknowByte14) | (CasperACProtocol.ucAirTimerMinute);
	_msgData[6] = (CasperACProtocol.ucAirMode) | (CasperACProtocol.ucAirSleepMode) | (CasperACProtocol.ucAirChangeView) | (CasperACProtocol.ucUknowByte15);
	_msgData[7] = 0x00;
	_msgData[8] = 0x00;
	_msgData[9] = (CasperACProtocol.ucUknowByte18) | (CasperACProtocol.ucAirTimers) |
				  (CasperACProtocol.ucAirPower) | (CasperACProtocol.ucAirEco) |
				  (CasperACProtocol.ucAiriClean) | (CasperACProtocol.ucAirHealth);

	_msgData[10] = 0x00;
	_msgData[11] = (CasperACProtocol.ucUknowByte20) | (CasperACProtocol.ucAirDisplay) |
				   (CasperACProtocol.ucAirMildew) | (CasperACProtocol.ucAirSleepCurve);

	_msgData[12] = Cal_CheckSum(_msgData);
}

uint8_t Ir_Casper::Cal_CheckSum(uint8_t hex_data[])
{
	uint32_t CheckSum = 0;
	for (int var = 0; var < CASPER_REMOTE_MAX_LENGTH - 1; ++var)
	{
		CheckSum = ((CheckSum + hex_data[var]) & 0xFF);
	}
	return ((uint8_t)CheckSum);
}

void Ir_Casper::setPower(uint8_t power_onoff)
{
	if (power_onoff == 0)
		CasperACProtocol.ucAirPower = irCasper_Power_OFF;
	else if (power_onoff == 1)
		CasperACProtocol.ucAirPower = irCasper_Power_ON;
	else
		CasperACProtocol.ucAirPower = irCasper_Power_OFF;
}

void Ir_Casper::setTemp(uint8_t temp)
{
	if ((temp > 16) || (temp < 32))
		CasperACProtocol.ucAirTemp = (temp - 16) * 8 + 0x40; // change tem to hex
}
void Ir_Casper::setFan(uint8_t fan_speed)
{
	switch (fan_speed)
	{
	case 0:
		CasperACProtocol.ucAirFanSpeedSum = irCasper_Speed_Low;
		break;
	case 1:
		CasperACProtocol.ucAirFanSpeedSum = irCasper_Speed_Mid;
		break;
	case 2:
		CasperACProtocol.ucAirFanSpeedSum = irCasper_Speed_High;
		break;
	case 3:
		CasperACProtocol.ucAirFanSpeedSum = irCasper_Speed_Turbo;
		break;
	default:
		CasperACProtocol.ucAirFanSpeedSum = irCasper_Speed_Auto;
		break;
	}
}
void Ir_Casper::setMode(uint8_t mode)
{
	switch (mode)
	{
	case 0:
		CasperACProtocol.ucAirMode = irCasper_Mode_Cool;
		break;
	case 1:
		CasperACProtocol.ucAirMode = irCasper_Mode_Fan;
		break;
	case 2:
		CasperACProtocol.ucAirMode = irCasper_Mode_Auto;
		break;
	case 3:
		CasperACProtocol.ucAirMode = irCasper_Mode_Dry;
		break;
	default:
		CasperACProtocol.ucAirMode = irCasper_Mode_Auto;
		break;
	}
}
void Ir_Casper::setSwing(uint8_t swing)
{
	switch (swing)
	{
	case 0:
		CasperACProtocol.ucAirSwing = irCasper_SwingUpDown_OFF;
		break;
	case 1:
		CasperACProtocol.ucAirSwing = irCasper_SwingUpDown_ON;
		break;
	default:
		CasperACProtocol.ucAirSwing = irCasper_SwingUpDown_OFF;
		break;
	}
}

void Ir_Casper::setSleep(uint8_t sleep_mode)
{
	switch (sleep_mode)
	{
	case 0:
		CasperACProtocol.ucAirSleepMode = irCasper_Sleep_OFF;
		break;
	case 1:
		CasperACProtocol.ucAirSleepMode = irCasper_Sleep_ON;
		break;
	default:
		CasperACProtocol.ucAirSleepMode = irCasper_Sleep_OFF;
		break;
	}
}
/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/
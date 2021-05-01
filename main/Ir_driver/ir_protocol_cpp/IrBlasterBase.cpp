/*
 * IrBlasterBase.cpp
 *
 *  Created on: Jan 11, 2021
 *      Author: ductu
 */

#include "IrBlasterBase.h"

/***********************************************************************************************************************
 * Pragma directive
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes <System Includes>
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/
//#define USER_TEST_DATA 0
#define USER_DEFAULT_NBITS 1
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
IrBlasterBase::IrBlasterBase()
{
	// TODO Auto-generated constructor stub
}

IrBlasterBase::~IrBlasterBase()
{
	// TODO Auto-generated destructor stub
}

void IrBlasterBase::SendCommand(eProtocol_type type_remote, eCommand_type type_command, uint64_t data, uint8_t nbits, uint8_t repeat)
{
	APP_LOGD("SendCommand");
	switch (type_remote)
	{
	case ePROTOCOL_NEC_01: //oke
	{
		/* code */
		Ir_NEC nec_remote(38000, 50);
#ifdef USER_TEST_DATA
		nec_remote.sendNEC(0x1FE807F, 32, 2);
#else
		nec_remote.sendNEC(data);
#endif
		// read flash to hex
		break;
	}
	case ePROTOCOL_RC5: // error
	{
		Ir_RC5_RC6 _ir_rc5_remote(36000, 33);
#ifdef USER_TEST_DATA
		_ir_rc5_remote.sendRC5(0x175);
#else
		_ir_rc5_remote.sendRC5(data);
#endif
		break;
	}
	case ePROTOCOL_RC6:// error
	{
		Ir_RC5_RC6 _ir_rc6_remote(36000, 50);
#ifdef USER_TEST_DATA
		_ir_rc6_remote.sendRC6(0x15555);
#else
		_ir_rc6_remote.sendRC6(data);
#endif
		break;
	}
	case ePROTOCOL_JVC: //oke
	{
		Ir_JVC jvc_remote(38000, 50);
#ifdef USER_TEST_DATA
		jvc_remote.sendJVC(0xC2B8, 16);
#else
		jvc_remote.sendJVC(data);
#endif
		break;
	}
	case ePROTOCOL_SAMSUNG: //oke
	{
		Ir_Samsung samsung_remote(38000, 50);
#ifdef USER_TEST_DATA
		samsung_remote.sendSamsung(0xE0E09966);
#else
		samsung_remote.sendSamsung(data);
#endif
		break;
	}

	case ePROTOCOL_DENON:// error
	{
		Ir_Denon _ir_denon_remote; // add new
#ifdef USER_TEST_DATA
		_ir_denon_remote.sendDenon(0x2278);
#else
		_ir_denon_remote.sendDenon(data);
#endif
		break;
	}
	case ePROTOCOL_SHERWOOD: // oke
	{
		IrSherwood _ir_shewood_remote;
#ifdef USER_TEST_DATA
		_ir_shewood_remote.sendSherwood(0xC1A28877, 32, 2);
#else
		_ir_shewood_remote.sendSherwood(data);
#endif
		break;
	}
	case ePROTOCOL_RCMM: //error
	{
		Ir_RCMM _ir_rcmm_remote(36000, 33);
#ifdef USER_TEST_DATA
		_ir_rcmm_remote.sendRCMM(0x28e0a600, 32, 2); // 2 repeats.
#else
		_ir_rcmm_remote.sendRCMM(data);
#endif
		break;
	}
	case ePROTOCOL_SANYO_LC7461: //ok
	{
		IrSanyo _ir_sanyo_remote( 38000, 50);
#ifdef USER_TEST_DATA
		_ir_sanyo_remote.sendSanyoLC7461(0x2468DCB56A9);
#else
		_ir_sanyo_remote.sendSanyoLC7461(data);
#endif
		break;
	}
	case ePROTOCOL_NIKAI: // ok
	{
		IrNikai _ir_nikai_remote( 38000, 50);
#ifdef USER_TEST_DATA
		_ir_nikai_remote.sendNikai(0x123456);
#else
		_ir_nikai_remote.sendNikai(data);
#endif
		break;
	}
	case ePROTOCOL_MAGIQUEST: // ok
	{
		Ir_Magiquest _ir_magiquest_remote( 38000, 50);
#ifdef USER_TEST_DATA
		_ir_magiquest_remote.sendMagiQuest(0x123456789ABC);
#else
		_ir_magiquest_remote.sendMagiQuest(data);
#endif
		break;
	}
	case ePROTOCOL_LASERTAG: // error
	{
		Ir_Lasertag _ir_lasertag_remote( 36000, 50);
#ifdef USER_TEST_DATA
		_ir_lasertag_remote.sendLasertag(0x06);  // Red 6
#else
		_ir_lasertag_remote.sendLasertag(data);
#endif
		break;
	}
	case ePROTOCOL_GICABLE: // ok
	{
		Ir_GICable _ir_GICable_remote( 39000, 50);
#ifdef USER_TEST_DATA
		_ir_GICable_remote.sendGICable(0x8807);
#else
		_ir_GICable_remote.sendGICable(data);
#endif
		break;
	}
	case ePROTOCOL_LUTRON: // error
	{
		Ir_Lutron _ir_lutron_remote(38000, 40);
#ifdef USER_TEST_DATA
		_ir_lutron_remote.sendLutron(0x7F88BD120, kLutronBits, 0);
#else
		_ir_lutron_remote.sendLutron(data);
#endif
		break;
	}
	case ePROTOCOL_PIONEER: // add new ok
	{
		Ir_Pioneer _ir_Pioneer_remote(38000, 40);
#ifdef USER_TEST_DATA
		_ir_Pioneer_remote.sendPioneer(0xA55A38C7A55A38C7, 64, 1);
#else
		_ir_Pioneer_remote.sendPioneer(data);
#endif
		break;
	}
	case ePROTOCOL_SAMSUNG36: // add ok
	{
		Ir_Samsung _ir_samsung_remote(38000, 40);
#ifdef USER_TEST_DATA
		_ir_samsung_remote.sendSamsung36(0x400E00FF, kSamsung36Bits, 1);  // 1 repeat.
#else
		_ir_samsung_remote.sendSamsung36(data);
#endif
		break;
	}
	case ePROTOCOL_LEGOPF: //doing
	{
		APP_LOGD("go to ePROTOCOL_LEGOPF");
		IrLego _ir_legof_remote(38000, 50);
#ifdef USER_TEST_DATA
		_ir_legof_remote.sendLegoPf(0x100E, kLegoPfBits, 2);
#else
		_ir_legof_remote.sendLegoPf(data);
#endif
		break;
	}
	case ePROTOCOL_INAX: // ok
	{
		Ir_Inax _ir_inax_remote(38000, 50);
#ifdef USER_TEST_DATA
		_ir_inax_remote.sendInax(0x5C32CD); //
#else
		_ir_inax_remote.sendInax(data);
#endif
		break;
	}
	case ePROTOCOL_SONY38: // maybe different frequency
	{
		Ir_Sony _ir_sony38_remote(38000, 50);
#ifdef USER_TEST_DATA
		_ir_sony38_remote.sendSony38(0x240C, kSony20Bits);
#else
		_ir_sony38_remote.sendSony38(data);
#endif
		break;
	}
	case ePROTOCOL_SONY: // ok
	{
		Ir_Sony _ir_sony_remote(40000, 33);
#ifdef USER_TEST_DATA
		_ir_sony_remote.sendSony(0x240C, kSony20Bits, 0);
#else
		_ir_sony_remote.sendSony(data);
#endif
		break;
	}
	case ePROTOCOL_AIWA: //oke
	{
		Ir_Aiwa aiwa_remote;
#ifdef USER_TEST_DATA
		aiwa_remote.sendAiwaRCT501(0x7F, kAiwaRcT501Bits, 0);
#else
		aiwa_remote.sendAiwaRCT501(data);
#endif
		break;
	}
	case ePROTOCOL_PANASONIC_64: // oke
	{
		Ir_Panasonic ir_panasonic64(38000, 50);
#ifdef USER_TEST_DATA
		ir_panasonic64.sendPanasonic64(0x1234567890ABCDEF);
#else
		ir_panasonic64.sendPanasonic64(data);
#endif
		break;
	}

	case ePROTOCOL_PANASONIC: // oke
	{
		Ir_Panasonic ir_panasonic_remote(38000, 50);
#ifdef USER_TEST_DATA
		ir_panasonic_remote.sendPanasonic(0x4004, 0x0190ED7C);
#else
		ir_panasonic_remote.sendPanasonic(0x4004, data);
#endif
		break;
	}
	case ePROTOCOL_MWM:
	{
		Ir_MWM _ir_mwm_remote( 38000, 50);
#ifdef USER_TEST_DATA
		  unsigned char test2[] = {
		      0x99, 0x26, 0x66, 0x6E, 0xD1, 0x42, 0x06,
		      0x20, 0xD0, 0x32, 0xF0, 0x0B};
		_ir_mwm_remote.sendMWM(test2, sizeof(test2), 0);
#else
//		_ir_mwm_remote.sendMWM(data, sizeof(data));
#endif
		break;
	}

	default:
		break;
	}
}

/***********************************************************************************************************************
 * Function Name:
 * Description  :
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
void IrBlasterBase::SendHAVC(eProtocol_type type_havc_remote)
{
	switch (type_havc_remote)
	{

	case ePROTOCOL_ARGO_AC: //oke
	{
		Ir_Argo argo_remote(38000, 50);
		argo_remote.setparam(DeviceData.IrMessageCtr_Info.irAcValue.Power,
								 DeviceData.IrMessageCtr_Info.irAcValue.Mode,
								 DeviceData.IrMessageCtr_Info.irAcValue.FanSpeed,
								 DeviceData.IrMessageCtr_Info.irAcValue.Temp);
		argo_remote.sendArgo();
		break;
	}
	case ePROTOCOL_SANYO_AC: //ok
	{
		IrSanyo _ir_sanyo_remote(38000, 50);
		_ir_sanyo_remote.setparam(DeviceData.IrMessageCtr_Info.irAcValue.Power,
								 DeviceData.IrMessageCtr_Info.irAcValue.Mode,
								 DeviceData.IrMessageCtr_Info.irAcValue.FanSpeed,
								 DeviceData.IrMessageCtr_Info.irAcValue.Temp);
		_ir_sanyo_remote.sendSanYoAc(0);
		break;
	}
	case ePROTOCOL_HITACHI_AC1: //ok
	{
		IRHitachiAc1 _ir_hitachiAC1_remote(38000, 50);
		_ir_hitachiAC1_remote.setparam(DeviceData.IrMessageCtr_Info.irAcValue.Power,
								 DeviceData.IrMessageCtr_Info.irAcValue.Mode,
								 DeviceData.IrMessageCtr_Info.irAcValue.FanSpeed,
								 DeviceData.IrMessageCtr_Info.irAcValue.Temp);
		_ir_hitachiAC1_remote.sendHitachiAc1(0);
		break;
	}
	case ePROTOCOL_HITACHI_AC2: // ok
	{
		IRHitachiAc2 _irhitachi_ac;
		uint8_t hitachi_code[kHitachiAc2StateLength] = {
		      0x80, 0x08, 0x00, 0x02, 0xFD, 0xFF, 0x00, 0x33, 0xCC, 0x49, 0xB6,
		      0x22, 0xDD, 0x01, 0xFE, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
		      0xFF, 0x00, 0xFF, 0xCA, 0x35, 0x8F, 0x70, 0x00, 0xFF, 0x00, 0xFF,
		      0x01, 0xFE, 0xC0, 0x3F, 0x80, 0x7F, 0x11, 0xEE, 0x00, 0xFF, 0x00,
		      0xFF, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00};
		_irhitachi_ac.sendHitachiAc2(hitachi_code);
		break;
	}
	case ePROTOCOL_HITACHI_AC3:// no function send data
	{
		IRHitachiAc3 _ir_hitachiAC3_remote(38000, 50);
		_ir_hitachiAC3_remote.sendHitachiAc3(0);
		break;
	}
	case ePROTOCOL_HITACHI_AC424: // error
	{
		IRHitachiAc424 _ir_hitachiAC424_remote(38000, 50);
		_ir_hitachiAC424_remote.sendHitachi_Ac424(0);
		break;
	}
	case ePROTOCOL_HITACHI_AC344: // error
	{
//		IRHitachiAc344 _ir_hitachiAC344_remote(38000, 50);
//		_ir_hitachiAC344_remote.sendHitachiAc344(0);
		break;
	}
	case ePROTOCOL_CASPER: //oke
	{
		Ir_Casper casper_remote(38000, 50);

		casper_remote.setparamCasper(DeviceData.IrMessageCtr_Info.irAcValue.Power,
									 DeviceData.IrMessageCtr_Info.irAcValue.Mode,
									 0,
									 DeviceData.IrMessageCtr_Info.irAcValue.FanSpeed,
									 DeviceData.IrMessageCtr_Info.irAcValue.Temp,
									 0);
		casper_remote.sendCasper();

		break;
	}
	case ePROTOCOL_FUJITSU_AC: //error
	{
		Ir_Fujitsu fujitsu_remote(ARRAH2E, 38000, 50);
		fujitsu_remote.setparam(DeviceData.IrMessageCtr_Info.irAcValue.Power,
								 DeviceData.IrMessageCtr_Info.irAcValue.Mode,
								 DeviceData.IrMessageCtr_Info.irAcValue.FanSpeed,
								 DeviceData.IrMessageCtr_Info.irAcValue.Temp);
		fujitsu_remote.sendFujitsu();
		break;
	}
	case ePROTOCOL_DAIKIN2: // add new error
	{
		Ir_Daikin2 _ir_daikin2_remote(38000, 50);
		_ir_daikin2_remote.setparam(DeviceData.IrMessageCtr_Info.irAcValue.Power,
								 DeviceData.IrMessageCtr_Info.irAcValue.Mode,
								 DeviceData.IrMessageCtr_Info.irAcValue.FanSpeed,
								 DeviceData.IrMessageCtr_Info.irAcValue.Temp);
		_ir_daikin2_remote.sendDaikin2();
		break;
	}
	case ePROTOCOL_DAIKIN216: // add new ok
	{
		Ir_Daikin216 _ir_daikin216_remote(38000, 50);
		_ir_daikin216_remote.setparam(DeviceData.IrMessageCtr_Info.irAcValue.Power,
								 DeviceData.IrMessageCtr_Info.irAcValue.Mode,
								 DeviceData.IrMessageCtr_Info.irAcValue.FanSpeed,
								 DeviceData.IrMessageCtr_Info.irAcValue.Temp);
		_ir_daikin216_remote.sendDaikin216();
		break;
	}
	case ePROTOCOL_DAIKIN160: // add new ok
	{
		Ir_Daikin160 _ir_daikin160_remote(38000, 50);
		_ir_daikin160_remote.setparam(DeviceData.IrMessageCtr_Info.irAcValue.Power,
								 DeviceData.IrMessageCtr_Info.irAcValue.Mode,
								 DeviceData.IrMessageCtr_Info.irAcValue.FanSpeed,
								 DeviceData.IrMessageCtr_Info.irAcValue.Temp);
		_ir_daikin160_remote.sendDaikin160();
		break;
	}
	case ePROTOCOL_DAIKIN176: //ok
	{
		Ir_Daikin176 _ir_daikin176_remote(38000, 50);
		_ir_daikin176_remote.setparam(DeviceData.IrMessageCtr_Info.irAcValue.Power,
								 DeviceData.IrMessageCtr_Info.irAcValue.Mode,
								 DeviceData.IrMessageCtr_Info.irAcValue.FanSpeed,
								 DeviceData.IrMessageCtr_Info.irAcValue.Temp);
		_ir_daikin176_remote.sendDaikin176();
		break;
	}
	case ePROTOCOL_DAIKIN128: // add new ok
	{
		Ir_Daikin128 daikin128_remote(38000, 50);
		daikin128_remote.setparam(DeviceData.IrMessageCtr_Info.irAcValue.Power,
								 DeviceData.IrMessageCtr_Info.irAcValue.Mode,
								 DeviceData.IrMessageCtr_Info.irAcValue.FanSpeed,
								 DeviceData.IrMessageCtr_Info.irAcValue.Temp);
		daikin128_remote.sendDaikin128();
		break;
	}
	case ePROTOCOL_DAIKIN152x: // error check time stem ok
	{
		Ir_Daikin152 _ir_daikin152_remote(38000, 50);
		_ir_daikin152_remote.setparam(DeviceData.IrMessageCtr_Info.irAcValue.Power,
								 DeviceData.IrMessageCtr_Info.irAcValue.Mode,
								 DeviceData.IrMessageCtr_Info.irAcValue.FanSpeed,
								 DeviceData.IrMessageCtr_Info.irAcValue.Temp);
		_ir_daikin152_remote.sendDaikin152();
		break;
	}
	case ePROTOCOL_DAIKIN64: // add new ok
	{
		Ir_Daikin64 _ir_remote_daikin64(38000, 50);
		_ir_remote_daikin64.setparam(DeviceData.IrMessageCtr_Info.irAcValue.Power,
								 DeviceData.IrMessageCtr_Info.irAcValue.Mode,
								 DeviceData.IrMessageCtr_Info.irAcValue.FanSpeed,
								 DeviceData.IrMessageCtr_Info.irAcValue.Temp);
		_ir_remote_daikin64.sendDaikin64();
		break;
	}
	case ePROTOCOL_DAIKINESP: // oke
	{
		Ir_DaikinESP daikinesp_remote(38000, 50);
		daikinesp_remote.setparam(DeviceData.IrMessageCtr_Info.irAcValue.Power,
								 DeviceData.IrMessageCtr_Info.irAcValue.Mode,
								 DeviceData.IrMessageCtr_Info.irAcValue.FanSpeed,
								 DeviceData.IrMessageCtr_Info.irAcValue.Temp);
		daikinesp_remote.sendDaikinESP();
		break;
	}
	case ePROTOCOL_PANASONIC_AC: // oke
	{
		Ir_Panasonic ir_panasonicAC(38000, 50);
		ir_panasonicAC.setparam(DeviceData.IrMessageCtr_Info.irAcValue.Power,
								 DeviceData.IrMessageCtr_Info.irAcValue.Mode,
								 DeviceData.IrMessageCtr_Info.irAcValue.FanSpeed,
								 DeviceData.IrMessageCtr_Info.irAcValue.Temp);
		ir_panasonicAC.sendPanasonicAC();
		break;
	}
	case ePROTOCOL_PANASONIC_AC32: //oke
	{
		Ir_Panasonic ir_panasonicAC32(38000, 50);
		ir_panasonicAC32.sendPanasonicAC32(0x3586, kPanasonicAc32Bits / 2);
		break;
	}
	case ePROTOCOL_SAMSUNG_AC: //oke
	{
		Ir_Samsung samsung_remote(38000, 50);
		samsung_remote.setparam(DeviceData.IrMessageCtr_Info.irAcValue.Power,
				 DeviceData.IrMessageCtr_Info.irAcValue.Mode,
				 DeviceData.IrMessageCtr_Info.irAcValue.FanSpeed,
				 DeviceData.IrMessageCtr_Info.irAcValue.Temp);
		samsung_remote.sendSamsungAC(0);
		break;
	}
	case ePROTOCOL_AIRWELL_AC: // oke
	{
		Ir_Airwell airwell_remote(38000, 50);
		airwell_remote.setparam(DeviceData.IrMessageCtr_Info.irAcValue.Power,
				 DeviceData.IrMessageCtr_Info.irAcValue.Mode,
				 DeviceData.IrMessageCtr_Info.irAcValue.FanSpeed,
				 DeviceData.IrMessageCtr_Info.irAcValue.Temp);
		airwell_remote.sendAirwell();
		break;
	}
	case ePROTOCOL_AMCOR: // oke
	{
		Ir_Amcor amcor_remote(38000, 50);
		amcor_remote.setparam(DeviceData.IrMessageCtr_Info.irAcValue.Power,
						 DeviceData.IrMessageCtr_Info.irAcValue.Mode,
						 DeviceData.IrMessageCtr_Info.irAcValue.FanSpeed,
						 DeviceData.IrMessageCtr_Info.irAcValue.Temp);
		amcor_remote.sendAmcor();
		break;
	}
	case ePROTOCOL_SHARP: // ok
	{
		Ir_Sharp _ir_sharp_remote(38000, 50);
		_ir_sharp_remote.setparam(DeviceData.IrMessageCtr_Info.irAcValue.Power,
								 DeviceData.IrMessageCtr_Info.irAcValue.Mode,
								 DeviceData.IrMessageCtr_Info.irAcValue.FanSpeed,
								 DeviceData.IrMessageCtr_Info.irAcValue.Temp);
		_ir_sharp_remote.sendSharpAC();
		break;
	}
	case ePROTOCOL_LG_AC: // add new ok
	{
		Ir_LG _ir_remote_lg(38000, 50);
		_ir_remote_lg.setparam(DeviceData.IrMessageCtr_Info.irAcValue.Power,
								 DeviceData.IrMessageCtr_Info.irAcValue.Mode,
								 DeviceData.IrMessageCtr_Info.irAcValue.FanSpeed,
								 DeviceData.IrMessageCtr_Info.irAcValue.Temp);
		_ir_remote_lg.sendLGAC();
		break;
	}
	case ePROTOCOL_Whirlpool_AC: // add new ok
	{
		Ir_Whirlpool _irWhirlpool_remote(38000, 50);
		_irWhirlpool_remote.setparam(DeviceData.IrMessageCtr_Info.irAcValue.Power,
								 DeviceData.IrMessageCtr_Info.irAcValue.Mode,
								 DeviceData.IrMessageCtr_Info.irAcValue.FanSpeed,
								 DeviceData.IrMessageCtr_Info.irAcValue.Temp);
		_irWhirlpool_remote.sendWhirlpool();
		break;
	}
	case ePROTOCOL_Goodweather_AC: // add new ok
	{
		Ir_Goodweather _ir_Goodweather_remote(38000, 50);
		_ir_Goodweather_remote.setparam(DeviceData.IrMessageCtr_Info.irAcValue.Power,
								 DeviceData.IrMessageCtr_Info.irAcValue.Mode,
								 DeviceData.IrMessageCtr_Info.irAcValue.FanSpeed,
								 DeviceData.IrMessageCtr_Info.irAcValue.Temp);
		_ir_Goodweather_remote.sendGoodweather();
		break;
	}
	case ePROTOCOL_GREE: // add new ok
	{
		Ir_Gree _ir_gree_remote(38000, 50);
		_ir_gree_remote.setparam(DeviceData.IrMessageCtr_Info.irAcValue.Power,
								 DeviceData.IrMessageCtr_Info.irAcValue.Mode,
								 DeviceData.IrMessageCtr_Info.irAcValue.FanSpeed,
								 DeviceData.IrMessageCtr_Info.irAcValue.Temp);
		_ir_gree_remote.sendGree();
		break;
	}
	case ePROTOCOL_TOSHIBA: // add new ok
	{
		Ir_Toshiba _ir_remote(38000, 50);
		_ir_remote.setparam(DeviceData.IrMessageCtr_Info.irAcValue.Power,
								 DeviceData.IrMessageCtr_Info.irAcValue.Mode,
								 DeviceData.IrMessageCtr_Info.irAcValue.FanSpeed,
								 DeviceData.IrMessageCtr_Info.irAcValue.Temp);
		_ir_remote.sendToshibaAC();
		break;
	}
	case ePROTOCOL_Kelvinator: // add new ok
	{
		Ir_Kelvinator _ir_remote(38000, 50);
		_ir_remote.setparam(DeviceData.IrMessageCtr_Info.irAcValue.Power,
								 DeviceData.IrMessageCtr_Info.irAcValue.Mode,
								 DeviceData.IrMessageCtr_Info.irAcValue.FanSpeed,
								 DeviceData.IrMessageCtr_Info.irAcValue.Temp);
		_ir_remote.sendKelvinator();
		break;
	}
	case ePROTOCOL_NeoclimaAc: // add new ok
	{
		Ir_NeoclimaAc _ir_remote(38000, 50);
		_ir_remote.setparam(DeviceData.IrMessageCtr_Info.irAcValue.Power,
								 DeviceData.IrMessageCtr_Info.irAcValue.Mode,
								 DeviceData.IrMessageCtr_Info.irAcValue.FanSpeed,
								 DeviceData.IrMessageCtr_Info.irAcValue.Temp);
		_ir_remote.sendNeoclimaAc();
		break;
	}
	case ePROTOCOL_Teco_AC: // add new ok
	{
		Ir_Teco _ir_remote(38000, 50);
		_ir_remote.setparam(DeviceData.IrMessageCtr_Info.irAcValue.Power,
								 DeviceData.IrMessageCtr_Info.irAcValue.Mode,
								 DeviceData.IrMessageCtr_Info.irAcValue.FanSpeed,
								 DeviceData.IrMessageCtr_Info.irAcValue.Temp);
		_ir_remote.sendTecoAc();
		break;
	}
	case ePROTOCOL_MEDIA_AC: // add new ok
	{
		Ir_Midea _ir_remote(38000, 50);
		_ir_remote.setparam(DeviceData.IrMessageCtr_Info.irAcValue.Power,
								 DeviceData.IrMessageCtr_Info.irAcValue.Mode,
								 DeviceData.IrMessageCtr_Info.irAcValue.FanSpeed,
								 DeviceData.IrMessageCtr_Info.irAcValue.Temp);
		_ir_remote.sendMidea();
		break;
	}
	case ePROTOCOL_TCL_AC: //add new ok
	{
		Ir_TCL_112Ac _ir_remote(38000, 50);
		_ir_remote.setparam(DeviceData.IrMessageCtr_Info.irAcValue.Power,
								 DeviceData.IrMessageCtr_Info.irAcValue.Mode,
								 DeviceData.IrMessageCtr_Info.irAcValue.FanSpeed,
								 DeviceData.IrMessageCtr_Info.irAcValue.Temp);
		_ir_remote.sendTCL_112Ac();
		break;
	}
	case ePROTOCOL_MITSHUBISHI_AC: // add new ok
	{
		IRMitsubishiAC _ir_remote(38000, 50);
		_ir_remote.setparam(DeviceData.IrMessageCtr_Info.irAcValue.Power,
								 DeviceData.IrMessageCtr_Info.irAcValue.Mode,
								 DeviceData.IrMessageCtr_Info.irAcValue.FanSpeed,
								 DeviceData.IrMessageCtr_Info.irAcValue.Temp);
		_ir_remote.sendMitsubishiAC();
		break;
	}
	case ePROTOCOL_MITSHUBISHI_112_AC: // add new ok
	{
		IRMitsubishi112 _ir_remote(38000, 50);
		_ir_remote.setparam(DeviceData.IrMessageCtr_Info.irAcValue.Power,
								 DeviceData.IrMessageCtr_Info.irAcValue.Mode,
								 DeviceData.IrMessageCtr_Info.irAcValue.FanSpeed,
								 DeviceData.IrMessageCtr_Info.irAcValue.Temp);
		_ir_remote.sendMitsubishi112();
		break;
	}
	case ePROTOCOL_MITSHUBISHI_136_AC: // add new ok
	{
		IRMitsubishi136 _ir_remote(38000, 50);
		_ir_remote.setparam(DeviceData.IrMessageCtr_Info.irAcValue.Power,
								 DeviceData.IrMessageCtr_Info.irAcValue.Mode,
								 DeviceData.IrMessageCtr_Info.irAcValue.FanSpeed,
								 DeviceData.IrMessageCtr_Info.irAcValue.Temp);
		_ir_remote.sendMitsubishi136();
		break;
	}
	case ePROTOCOL_MITSHUBISHI_HEAVY88_AC: // add new ok
	{
		IRMitsubishiHeavy88Ac _ir_remote(38000, 50);
		_ir_remote.setparam(DeviceData.IrMessageCtr_Info.irAcValue.Power,
								 DeviceData.IrMessageCtr_Info.irAcValue.Mode,
								 DeviceData.IrMessageCtr_Info.irAcValue.FanSpeed,
								 DeviceData.IrMessageCtr_Info.irAcValue.Temp);
		_ir_remote.sendMitsubishiHeavy88();
		break;
	}
	case ePROTOCOL_MITSHUBISHI_HEAVY152_AC:// add new ok
	{
		IRMitsubishiHeavy152Ac _ir_remote;
		_ir_remote.setparam(DeviceData.IrMessageCtr_Info.irAcValue.Power,
								 DeviceData.IrMessageCtr_Info.irAcValue.Mode,
								 DeviceData.IrMessageCtr_Info.irAcValue.FanSpeed,
								 DeviceData.IrMessageCtr_Info.irAcValue.Temp);
		_ir_remote.sendMitsubishiHeavy152Ac();
		break;
	}
	default:
		break;
	}
}
/***********************************************************************************************************************
 * Function Name:
 * Description  :
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/

void IrBlasterBase::SendRaw(uint16_t *data_raw, uint16_t length, uint16_t frequency, uint16_t dutycycle, uint8_t repeat)
{
	Ir_Send ir_raw(frequency, dutycycle);
//	for (int i = 0; i < repeat; i++)
//	{
		ir_raw.sendGeneric(data_raw, length);
//	}
}
/***********************************************************************************************************************
 * Static Functions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************/

/*
 * ir_remote_define.h
 *
 *  Created on: Dec 26, 2020
 *      Author: ductu
 */

#ifndef MAIN_IR_DRIVER_IR_PROTOCOL_IR_REMOTE_DEFINE_H_
#define MAIN_IR_DRIVER_IR_PROTOCOL_IR_REMOTE_DEFINE_H_

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
typedef enum
{
	ePROTOCOL_RAW,
	ePROTOCOL_NEC_01, //done normal
	ePROTOCOL_NEC_02, //done normal

	ePROTOCOL_RC5,	//error normal
	ePROTOCOL_RC5X, //error normal
	ePROTOCOL_RC6,	//error normal

	ePROTOCOL_JVC,		// done normal
	ePROTOCOL_SAMSUNG,	//done normal
	ePROTOCOL_DENON,	// normal
	ePROTOCOL_SHERWOOD, // done normal
	ePROTOCOL_RCMM,		// normal error

	ePROTOCOL_SANYO_LC7461, // normal
	ePROTOCOL_NIKAI,		// normal
	ePROTOCOL_MAGIQUEST,	//normal
	ePROTOCOL_LASERTAG,		//normal
	ePROTOCOL_GICABLE,		// normal
	ePROTOCOL_LUTRON,		// normal error
	ePROTOCOL_PIONEER,		// normal
	ePROTOCOL_SAMSUNG36,	// normal
	ePROTOCOL_LEGOPF,		// normal
	ePROTOCOL_INAX,			// ok normal
	ePROTOCOL_SONY38,		// normal
	ePROTOCOL_SONY,			//normal doing
	ePROTOCOL_AIWA,			// normal oke
	ePROTOCOL_AMCOR,		// air condinator oke
	ePROTOCOL_PANASONIC_64, //normal oke
	ePROTOCOL_PANASONIC,	//normal ok
	ePROTOCOL_MWM,
	ePROTOCOL_Whynter,
	ePROTOCOL_DISH,

	ePROTOCOL_ARGO_AC,	// air condinator ok
	ePROTOCOL_SANYO_AC, // air condinator

	ePROTOCOL_CARRIER_AC,  // air condinator
	ePROTOCOL_HITACHI_AC1, // air condinator
	ePROTOCOL_HITACHI_AC2, // air condinator
	ePROTOCOL_HITACHI_AC3,
	ePROTOCOL_HITACHI_AC344,
	ePROTOCOL_HITACHI_AC424,
	ePROTOCOL_CASPER,	   // air condinator ok
	ePROTOCOL_FUJITSU_AC,  // air condinator error

	ePROTOCOL_DAIKIN2,	  // air condinator
	ePROTOCOL_DAIKIN216,  // air condinator
	ePROTOCOL_DAIKIN160,  // air condinator
	ePROTOCOL_DAIKIN176,  // air condinator
	ePROTOCOL_DAIKIN128,  // air condinator
	ePROTOCOL_DAIKIN152x, // air condinator
	ePROTOCOL_DAIKIN64,	  // air condinator
	ePROTOCOL_DAIKINESP,  // air condinator ok

	ePROTOCOL_PANASONIC_AC,	  //air condinator ok
	ePROTOCOL_PANASONIC_AC32, //air condinator ok
	ePROTOCOL_SAMSUNG_AC,
	ePROTOCOL_AIRWELL_AC, //air condinator ok

	ePROTOCOL_SHARP, // air condinator
	ePROTOCOL_LG_AC,
	ePROTOCOL_Whirlpool_AC,
	ePROTOCOL_Goodweather_AC,
	ePROTOCOL_GREE,
	ePROTOCOL_TOSHIBA,
	ePROTOCOL_Kelvinator,
	ePROTOCOL_NeoclimaAc,
	ePROTOCOL_Teco_AC,
	ePROTOCOL_MEDIA_AC,
	ePROTOCOL_TCL_AC,
	ePROTOCOL_MITSHUBISHI_AC,
	ePROTOCOL_MITSHUBISHI_112_AC,
	ePROTOCOL_MITSHUBISHI_136_AC,
	ePROTOCOL_MITSHUBISHI_HEAVY88_AC,
	ePROTOCOL_MITSHUBISHI_HEAVY152_AC,
} eProtocol_type;

typedef enum
{
	eCOMMAND_IR_RAW,
	eCOMMAND_HAVC,
	eCOMMAND_IR_NORMAL,
} eCommand_type;
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***         Exported global functions                                     ***/
/****************************************************************************/

#endif /* MAIN_IR_DRIVER_IR_PROTOCOL_IR_REMOTE_DEFINE_H_ */

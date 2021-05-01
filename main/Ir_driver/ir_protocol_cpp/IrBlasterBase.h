/*
 * IrBlasterBase.h
 *
 *  Created on: Jan 11, 2021
 *      Author: ductu
 */

#ifndef MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRBLASTERBASE_H_
#define MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRBLASTERBASE_H_

#include "../../Common.h"
#include "ir_remote_define.h"
#include "IrNEC.h"
#include "IrJVC.h"
#include "IrCasper.h"
#include "IrSamsung.h"
#include "IrAirwell.h"
#include "IrAiwa.h"
#include "IrFujitsu.h"
#include "IrAmcor.h"
#include "IrArgo.h"
#include "IrDaikin.h"
#include "IrPanasonic.h"
#include "IrRC5_RC6.h"
#include "IrSony.h"
#include "IrLutron.h"
#include "IrRCMM.h"
#include "IrInax.h"
#include "IrLego.h"
#include "IrSherwood.h"
#include "IrSharp.h"
#include "IrMWM.h"
#include "IrSanyo.h"
#include "IrHitachi.h"
#include "IrNikai.h"
#include "IrMagiquest.h"
#include "IrLasertag.h"
#include "IrGICable.h"
#include "IrPioneer.h"
#include "IrDenon.h"
#include "IrWhirlpool.h"
#include "IrGoodweather.h"
#include "IrWhynter.h"
#include "IrLG.h"
#include "IrToshiba.h"
#include "IrKelvinator.h"
#include "IrGree.h"
#include "IrNeoclimaAc.h"
#include "IrTeco.h"
#include "IrTCL112Ac.h"
#include "IrDish.h"
#include "IrMidea.h"
#include "IrMitsubishi112.h"
#include "IrMitsubishiHeavy.h"
class IrBlasterBase
{
public:
	IrBlasterBase();
	virtual ~IrBlasterBase();
	void SendCommand(eProtocol_type type_remote, eCommand_type type_command, uint64_t data, uint8_t nbits, uint8_t repeat);
	void SendHAVC(eProtocol_type type_havc_remote);
	void SendRaw(uint16_t *data_raw, uint16_t length, uint16_t frequency, uint16_t dutycycle, uint8_t repeat);
private:
};
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***         Exported global functions                                     ***/
/****************************************************************************/

#endif /* MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRBLASTERBASE_H_ */

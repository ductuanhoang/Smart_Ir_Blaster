/*
 * IrMagiquest.h
 *
 *  Created on: Mar 9, 2021
 *      Author: ductu
 */

#ifndef MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRMAGIQUEST_H_
#define MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRMAGIQUEST_H_


/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "IrSend.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
/// MagiQuest packet is both Wand ID and magnitude of swish and flick
union magiquest {
  uint64_t llword;
  uint8_t byte[8];
  //  uint16_t   word[4];
  uint32_t lword[2];
  struct {
    uint16_t magnitude;
    uint32_t wand_id;
    uint8_t padding;
    uint8_t scrap;
  } cmd;
};

const uint16_t kMagiQuestTotalUsec = 1150;
const uint8_t kMagiQuestZeroRatio = 30;  // usually <= ~25%
const uint8_t kMagiQuestOneRatio = 38;   // usually >= ~50%
const uint16_t kMagiQuestMarkZero = 280;
const uint16_t kMagiQuestSpaceZero = 850;
const uint16_t kMagiQuestMarkOne = 580;
const uint16_t kMagiQuestSpaceOne = 600;
const uint32_t kMagiQuestGap = kDefaultMessageGap;  // Just a guess.
/****************************************************************************/
/***         Exported global functions                                     ***/
/****************************************************************************/
class Ir_Magiquest {
public:
	Ir_Magiquest(uint32_t frequency, uint16_t dutycycle);
	virtual ~Ir_Magiquest();
	void sendMagiQuest(const uint64_t data, const uint16_t nbits = kMagiquestBits,
	                     const uint16_t repeat = kNoRepeat);
private:
	Ir_Send _ir_magiquest;
};
#endif /* MAIN_IR_DRIVER_IR_PROTOCOL_CPP_IRMAGIQUEST_H_ */

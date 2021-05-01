/*
 * vsm_LedStatus.h
 *
 *  Created on: Aug 23, 2020
 *      Author: Yolo
 */

#ifndef MAIN_LED_DRIVER_LEDSTATUS_H_
#define MAIN_LED_DRIVER_LEDSTATUS_H_

#include "../Common.h"

#define MAX_LED_SUPPORT             (4)

/**
 * @brief      Execute the blink pattern
 *
 * @param[in]  u8Idx       Led index
 * @param[in]  u8Count     Number of blink
 * @param[in]  u16OnTime   On time period
 * @param[in]  u16OffTime  Off time period
 */
void vLedStatusBlink(uint8_t u8Idx, uint8_t u8Count, uint16_t u16OnTime, uint16_t u16OffTime);

/**
 * @brief      Stop blink pattern
 *
 * @param[in]  u8Idx  Led index
 */
void vLedStatusStopBlink(uint8_t u8Idx);

/**
 * @brief      Set function callback to turn on led
 *
 * @param[in]  u8Idx  Led index
 * @param[in]  cb     Function callback
 *
 * @code{
 *  void ledon(void);
 *
 *  void ledon(void)
 *  {
 *      halSetLed(BOARDLED1);
 *  }
 *
 *  vLedStatusSetOnCallback(0, ledon0);
 * }
 */
void vLedStatusSetOnCallback(uint8_t u8Idx, void(*cb)(void));

/**
 * @brief      Set function callback to turn off led
 *
 * @param[in]  u8Idx  Led index
 * @param[in]  cb     Function callback
 */
void vLedStatusSetOffCallback(uint8_t u8Idx, void(*cb)(void));

/**
 * @brief      Set the function callback that want to execute after the led blink pattern finish
 *
 * @param[in]  u8Idx  Led index
 * @param[in]  cb     Function callback
 */
void vLedStatusSetPostBlinkCallback(uint8_t u8Idx, void(*cb)(void));

/**
 * @brief      Start led status service
 */
void vLedStatusStart(void);

/**
 * @brief      Stop led status service
 */
void vLedStatusStop(void);

void vLedStatusTask(void);

void vLedStatusSetGetTickCallback(uint32_t(*gettickCb)(void));

#endif /* MAIN_LED_DRIVER_LEDSTATUS_H_ */

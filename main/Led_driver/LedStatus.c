/*
 * vsm_LedStatus.c
 *
 *  Created on: Aug 23, 2020
 *      Author: Yolo
 */
#include "LedStatus.h"


typedef struct
{
    uint8_t u8Status;
    uint8_t u8BlinkState;
    uint8_t u8LastBlinkState;
    uint8_t u8BlinkCount;
    uint8_t u8IsBlinking;
    uint16_t u16OnTime;
    uint16_t u16OffTime;
    uint32_t u32LastTime;
    void(*led_on_cb)(void);
    void(*led_off_cb)(void);
    void(*post_blink_cb)(void);
} tsLedStatus;

static tsLedStatus tsContext[MAX_LED_SUPPORT];
static uint8_t u8LedStatusStart = 0;
static uint32_t(*gettick)(void);

void vLedStatusSetGetTickCallback(uint32_t(*gettickCb)(void))
{
    gettick = gettickCb;
}

void vLedStatusStart(void)
{
    u8LedStatusStart = 1;
}

void vLedStatusStop(void)
{
    u8LedStatusStart = 0;
}

void vLedStatusBlink(uint8_t u8Idx, uint8_t u8Count, uint16_t u16OnTime, uint16_t u16OffTime)
{
    if (u8Idx > (MAX_LED_SUPPORT - 1))
    {
        APP_LOGD("Over max led support");
        return;
    }

    if (tsContext[u8Idx].u8IsBlinking == 1)
    {
        return;
    }

    //APP_LOGD("idx: %d, count: %d, onT: %d, OffT: %d", u8Idx, u8Count, u16OnTime, u16OffTime);
    tsContext[u8Idx].u8BlinkCount = u8Count;
    tsContext[u8Idx].u16OnTime = u16OnTime;
    tsContext[u8Idx].u16OffTime = u16OffTime;
    tsContext[u8Idx].u32LastTime = 0;
    tsContext[u8Idx].u8Status = 1;
    tsContext[u8Idx].u8BlinkState = 1;
    tsContext[u8Idx].u8LastBlinkState = 0;
    tsContext[u8Idx].u8IsBlinking = 1;
}

void vLedStatusStopBlink(uint8_t u8Idx)
{
    if (u8Idx > (MAX_LED_SUPPORT - 1))
    {
        APP_LOGD("Over max led support");
        return;
    }
//    APP_LOGD("idx: %d, count: %d, onT: %d, OffT: %d", u8Idx, u8Count, u16OnTime, u16OffTime);
    tsContext[u8Idx].u8BlinkCount = 0;
    tsContext[u8Idx].u16OnTime = 0;
    tsContext[u8Idx].u16OffTime = 0;
    tsContext[u8Idx].u32LastTime = 0;
    tsContext[u8Idx].u8Status = 0;
    tsContext[u8Idx].u8BlinkState = 0;
    tsContext[u8Idx].u8LastBlinkState = 0;

    if (tsContext[u8Idx].led_off_cb != 0)
    {
        tsContext[u8Idx].led_off_cb();
    }
}

void vLedStatusSetPostBlinkCallback(uint8_t u8Idx, void(*cb)(void))
{
    if (u8Idx > (MAX_LED_SUPPORT - 1))
    {
        APP_LOGD("Over max led support");
        return;
    }

    if (cb != 0)
    {
        tsContext[u8Idx].post_blink_cb = cb;
    }
}

void vLedStatusSetOnCallback(uint8_t u8Idx, void(*cb)(void))
{
    if (u8Idx > (MAX_LED_SUPPORT - 1))
    {
        APP_LOGD("Over max led support");
        return;
    }

    if (cb != 0)
    {
        tsContext[u8Idx].led_on_cb = cb;
    }
}

void vLedStatusSetOffCallback(uint8_t u8Idx, void(*cb)(void))
{
    if (u8Idx > (MAX_LED_SUPPORT - 1))
    {
        APP_LOGD("Over max led support");
        return;
    }

    if (cb != 0)
    {
        tsContext[u8Idx].led_off_cb = cb;
    }
}

void vLedStatusTask(void)
{
    uint8_t i = 0;

    if (!u8LedStatusStart)
    {
        return;
    }

    for (i = 0; i < MAX_LED_SUPPORT; i++)
    {
        if (tsContext[i].u8Status)
        {
            if (tsContext[i].u32LastTime == 0)
            {
                tsContext[i].u32LastTime = gettick();
            }
            else
            {
                if (tsContext[i].u8BlinkState ^ tsContext[i].u8LastBlinkState)
                {
                    tsContext[i].u8LastBlinkState = tsContext[i].u8BlinkState;
                    if (tsContext[i].u8BlinkState)
                    {
                        if (tsContext[i].led_on_cb != 0)
                        {
                            tsContext[i].led_on_cb();
                        }
                    }
                    else
                    {
                        if (tsContext[i].led_off_cb != 0)
                        {
                            tsContext[i].led_off_cb();
                        }
                    }
                }

                if (tsContext[i].u8BlinkState)
                {
                    if (gettick() - tsContext[i].u32LastTime >= tsContext[i].u16OnTime)
                    {
                        tsContext[i].u32LastTime = gettick();
                        tsContext[i].u8BlinkState = 0;
                    }
                }
                else
                {
                    if (gettick() - tsContext[i].u32LastTime >= tsContext[i].u16OffTime)
                    {
                        tsContext[i].u32LastTime = gettick();
                        tsContext[i].u8BlinkState = 1;
                        tsContext[i].u8BlinkCount--;
                    }
                }
            }
            if (tsContext[i].u8BlinkCount == 0)
            {
                tsContext[i].u8Status = 0;
                tsContext[i].u8IsBlinking = 0;
                if (tsContext[i].post_blink_cb != 0)
                {
                    tsContext[i].post_blink_cb();
                }
            }
        }
    }
}





/*
 * ir_driver_basic.c
 *
 *  Created on: Jan 7, 2021
 *      Author: ductu
 */
/***********************************************************************************************************************
* Pragma directive
***********************************************************************************************************************/

/***********************************************************************************************************************
* Includes <System Includes>
***********************************************************************************************************************/
#include "ir_driver_basic.h"
/***********************************************************************************************************************
* Macro definitions
***********************************************************************************************************************/

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
void ir_basic_mask_space(uint32_t mask, uint32_t space)
{
    size_t size = (sizeof(rmt_item32_t) * (1));
    rmt_item32_t *item = (rmt_item32_t *)malloc(size);
    memset((void *)item, 0, size);

    item->level0 = 1;
    item->duration0 = (mask) / 10 * RMT_TICK_10_US;
    item->level1 = 0;
    if( space > 0x7FFF) item->duration1 = (0x7FFF) / 10 * RMT_TICK_10_US;
    else
    	item->duration1 = (space) / 10 * RMT_TICK_10_US;

    int item_num = 1 * 1;
    //To send data according to the waveform items.

    rmt_write_items(CHANNEL_IR_RMT_TX, item, item_num, false);
    //Wait until sending is done.
    rmt_wait_tx_done(CHANNEL_IR_RMT_TX, 50000);
//    while( rmt_wait_tx_done(CHANNEL_IR_RMT_TX, 100) != (ESP_OK | ESP_ERR_TIMEOUT));
    free(item);
}

void ir_basic_mark(uint16_t mask)
{
    size_t size = (sizeof(rmt_item32_t) * (1));
    rmt_item32_t *item = (rmt_item32_t *)malloc(size);
    memset((void *)item, 0, size);

    if( mask > 0x7FFF)mask = 0x7FFF;
	item->level0 = 0;
	item->level1 = 1;
	item->duration0 = (mask) / 10 * RMT_TICK_10_US;
    item->duration1 = (mask) / 10 * RMT_TICK_10_US;

    int item_num = 1 * 1;
    //To send data according to the waveform items.

    rmt_write_items(CHANNEL_IR_RMT_TX, item, item_num, false);
    //Wait until sending is done.
    rmt_wait_tx_done(CHANNEL_IR_RMT_TX, 200);
//    while( rmt_wait_tx_done(CHANNEL_IR_RMT_TX, 100) != (ESP_OK | ESP_ERR_TIMEOUT));
    free(item);
}

void ir_basic_space(uint32_t space)
{
    size_t size = (sizeof(rmt_item32_t) * (1));
    rmt_item32_t *item = (rmt_item32_t *)malloc(size);
    memset((void *)item, 0, size);

    if( space > 0x7FFF)space = 0x7FFF;
	item->level0 = 1;
	item->level1 = 0;
	item->duration0 = (space) / 10 * RMT_TICK_10_US;
    item->duration1 = (space) / 10 * RMT_TICK_10_US;

    int item_num = 1 * 1;
    //To send data according to the waveform items.

    rmt_write_items(CHANNEL_IR_RMT_TX, item, item_num, false);
    //Wait until sending is done.
    rmt_wait_tx_done(CHANNEL_IR_RMT_TX, 200);
//    while( rmt_wait_tx_done(CHANNEL_IR_RMT_TX, 100) != (ESP_OK | ESP_ERR_TIMEOUT));
    free(item);
}


//void ir_basic_build_items()
/***********************************************************************************************************************
* Function Name:
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void ir_basic_install_tx_channel( uint32_t freq, uint32_t duty_percent)
{
	ir_driver_tx_init(CHANNEL_IR_RMT_TX, freq, duty_percent);
}

/***********************************************************************************************************************
* Function Name:
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void ir_basic_uninstall_tx_channel(void)
{
	ir_driver_uninstall(CHANNEL_IR_RMT_TX);
}
/***********************************************************************************************************************
* Static Functions
***********************************************************************************************************************/

/***********************************************************************************************************************
* End of file
***********************************************************************************************************************/

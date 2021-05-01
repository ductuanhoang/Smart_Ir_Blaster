

/** @file:    template.h
 *  @details:  This file is the template of C header file
 *
 * THE SOURCE CODE AND ITS RELATED DOCUMENTATION IS PROVIDED "AS IS". VINSMART
 * JSC MAKES NO OTHER WARRANTY OF ANY KIND, WHETHER EXPRESS, IMPLIED OR,
 * STATUTORY AND DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF MERCHANTABILITY,
 * SATISFACTORY QUALITY, NON INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * THE SOURCE CODE AND DOCUMENTATION MAY INCLUDE ERRORS. VINSMART JSC
 * RESERVES THE RIGHT TO INCORPORATE MODIFICATIONS TO THE SOURCE CODE IN LATER
 * REVISIONS OF IT, AND TO MAKE IMPROVEMENTS OR CHANGES IN THE DOCUMENTATION OR
 * THE PRODUCTS OR TECHNOLOGIES DESCRIBED THEREIN AT ANY TIME.
 *
 * VINSMART JSC SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
 * CONSEQUENTIAL DAMAGE OR LIABILITY ARISING FROM YOUR USE OF THE SOURCE CODE OR
 * ANY DOCUMENTATION, INCLUDING BUT NOT LIMITED TO, LOST REVENUES, DATA OR
 * PROFITS, DAMAGES OF ANY SPECIAL, INCIDENTAL OR CONSEQUENTIAL NATURE, PUNITIVE
 * DAMAGES, LOSS OF PROPERTY OR LOSS OF PROFITS ARISING OUT OF OR IN CONNECTION
 * WITH THIS AGREEMENT, OR BEING UNUSABLE, EVEN IF ADVISED OF THE POSSIBILITY OR
 * PROBABILITY OF SUCH DAMAGES AND WHETHER A CLAIM FOR SUCH DAMAGE IS BASED UPON
 * WARRANTY, CONTRACT, TORT, NEGLIGENCE OR OTHERWISE.
 *
 * (C)Copyright VINSMART JSC 2019 All rights reserved
 ******************************************************************************/

#ifndef __VSM_RETARGET_H__
#define __VSM_RETARGET_H__

/***************************** Include Files *********************************/
//#include "FreeRTOS.h"
//#include "queue.h"
#include "../../Common.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/
//#ifdef _RAISONANCE_
//#define PUTCHAR_PROTOTYPE int putchar (char c)
//#define GETCHAR_PROTOTYPE int getchar (void)
//#elif defined (_COSMIC_)
//#define PUTCHAR_PROTOTYPE char putchar (char c)
//#define GETCHAR_PROTOTYPE char getchar (void)
//#else /* _IAR_ */
//#define PUTCHAR_PROTOTYPE int putchar (int c)
//#define GETCHAR_PROTOTYPE int getchar (void)
//#endif /* _RAISONANCE_ */

/************************** Variable Definitions *****************************/

/************************** Function Prototypes ******************************/
// int xSerialGetChar (portCHAR *pcRxedChar, portTickType xBlockTime );

int retarget_init_fifo(void);
void HAL_UART_RxCpltCallback(uint8_t *u8RxByte);
int uart_debug_printf(const char *Format, ...);
long user_putchar(const char * str);
long user_getchar (void);
// int serial_get_char(uint8_t *c);
/*****************************************************************************/



#endif /* __VSM_RETARGET_H__ */

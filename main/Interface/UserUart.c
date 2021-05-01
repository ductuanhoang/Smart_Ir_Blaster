/*
 * User_Uart.c
 *
 *  Created on: Mar 4, 2019
 *      Author: giangnv3
 */

#include "UserUart.h"
#include "shell/vsm_retarget.h"

#define EX_UART_NUM UART_NUM_2

static intr_handle_t handle_console;
static void IRAM_ATTR uart_intr_handle(void *arg);
static uint8_t rxbuf[256];

/******************************************************************************
 *
 ******************************************************************************/
void UserUart_Config(uart_port_t uart_port, uint32_t baudrate)
{

	/* Configure parameters of an UART driver,
	* communication pins and install the driver */
	uart_config_t uart_config = {
		.baud_rate = baudrate,
		.data_bits = UART_DATA_8_BITS,
		.parity = UART_PARITY_DISABLE,
		.stop_bits = UART_STOP_BITS_1,
		.flow_ctrl = UART_HW_FLOWCTRL_DISABLE
	};

	ESP_ERROR_CHECK(uart_param_config(uart_port, &uart_config));

	//Set UART pins (using UART0 default pins ie no changes.)
	ESP_ERROR_CHECK(uart_set_pin(uart_port, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

	//Install UART driver, and get the queue.
	ESP_ERROR_CHECK(uart_driver_install(uart_port, BUFF_SIZE * 2, 0, 0, NULL, 0));

	// release the pre registered UART handler/subroutine
	ESP_ERROR_CHECK(uart_isr_free(uart_port));

	// register new UART subroutine
	ESP_ERROR_CHECK(uart_isr_register(uart_port,uart_intr_handle, NULL, ESP_INTR_FLAG_IRAM, &handle_console));

	// enable RX interrupt
	ESP_ERROR_CHECK(uart_enable_rx_intr(uart_port));

}

void UserUart_2_Init(uint32_t baud)
{
    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = baud,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_NUM_2, &uart_config);
    uart_set_pin(UART_NUM_2, GPIO_NUM_17, GPIO_NUM_16, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
//    uart_set_pin(UART_NUM_2, GPIO_NUM_27, GPIO_NUM_26, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE); //for test with TTcall
    uart_driver_install(UART_NUM_2, 1024 * 2, 0, 0, NULL, 0);
}

/******************************************************************************
 *
 ******************************************************************************/
int UserUart_ReadData(unsigned char* buff)
{
	int len = 0;
	len = uart_read_bytes(EX_UART_NUM, buff, BUFF_SIZE, 40/portTICK_RATE_MS); // @suppress("Symbol is not resolved")
	return len;
}
/******************************************************************************
 *
 ******************************************************************************/
void UserUart_WriteData(unsigned char* buf, unsigned char size)
{
	char Hex[17] = "0123456789ABCDEF";
	printf("\r\n--- UserUart_WriteData: Sending data = ");
	for(unsigned char ui = 0; ui < size; ui++)
	{
		unsigned char p = buf[ui];
		printf("%c%c", Hex[p / 16], Hex[p % 16]);
	}
	printf("\r\n");
	uart_write_bytes( EX_UART_NUM, (const char*)buf, (size_t)size);
}
/******************************************************************************
 *
 ******************************************************************************/
void UserUart_UartRx(unsigned char* data_rev, unsigned char len, bool from_uart)
{
	printf("\r\n--- UserUart_UartRx: data = %s, len = %d\r\n", data_rev, len);
	memset(data_rev, 0x00, len);
}
/******************************************************************************
 *
 ******************************************************************************/
void UserUart_UartTX(unsigned char* data_tx, unsigned char len)
{
	printf("\r\n--- UserUart_UartTXCB: Sending data....\r\n");
	UserUart_WriteData(data_tx, len);
}

/******************************************************************************
 *
 ******************************************************************************/
/*
 * Define UART interrupt subroutine to ackowledge interrupt
 */
static void IRAM_ATTR uart_intr_handle(void *arg)
{
	uint16_t rx_fifo_len, status;

	status = UART0.int_st.val; // read UART interrupt Status
	rx_fifo_len = UART0.status.rxfifo_cnt; // read number of bytes in UART buffer

	while(rx_fifo_len)
	{
		HAL_UART_RxCpltCallback(UART0.fifo.rw_byte);
		rx_fifo_len--;
	}
	// after reading bytes from buffer clear UART interrupt status
	uart_clear_intr_status(UART_NUM_0, UART_RXFIFO_FULL_INT_CLR|UART_RXFIFO_TOUT_INT_CLR);
}





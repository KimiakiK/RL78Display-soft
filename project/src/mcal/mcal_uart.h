/*
 * File: mcal_uart.h
 * Brief: UARTドライバ
 * Author: KimiakiK
 * Date: 2022/12/13
 */

#ifndef MCAL_UART_H_
#define MCAL_UART_H_

/********** Include **********/

#include "typedef.h"

/********** Define **********/

/* 送信データ長最大 */
#define TX_LENGTH_MAX       (200)
#define RX_LENGTH_MAX       (200)

/********** Enum **********/

typedef enum {
	UART_RESULT_OK = 0,
	UART_RESULT_ERROR,
	UART_RESULT_BUSY,
	UART_RESULT_EMPTY
} uart_result_t;

/********** Type **********/

/********** Constant **********/

/********** Variable **********/

/********** Function Prototype **********/

void InitUart(void);
uart_result_t SendAsyncUart(uint8_t* data, uint16_t length);
uart_result_t GetReceiveUart(uint8_t* data);

#endif /* MCAL_UART_H_ */

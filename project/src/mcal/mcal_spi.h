/*
 * File: mcal_spi.h
 * Brief: SPIドライバ
 * Author: KimiakiK
 * Date: 2021/07/04
 */


#ifndef MCAL_SPI_H_
#define MCAL_SPI_H_

/********** Include **********/

#include "typedef.h"

/********** Define **********/

/********** Enum **********/

typedef enum {
	SEND_SYNC = 0,
	SEND_ASYNC
} send_mode_t;

/********** Type **********/

/********** Constant **********/

/********** Variable **********/

/********** Function Prototype **********/

void InitSpi(void);
void SendDataSpi(uint32_t data_address, uint32_t length, send_mode_t mode);
void SendFixDataSyncSpi(uint8_t data, uint32_t length, send_mode_t mode);

extern void EndCallbackSpi(void);

#endif /* MCAL_SPI_H_ */

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

/********** Type **********/

/********** Constant **********/

/********** Variable **********/

/********** Function Prototype **********/

void InitSpi(void);
void SendSyncSpi(uint32_t data_address, uint32_t length);
void SendAsyncSpi(uint32_t data_address, uint32_t length);

#endif /* MCAL_SPI_H_ */

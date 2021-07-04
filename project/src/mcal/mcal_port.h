/*
 * File: mcal_port.h
 * Brief: ポートドライバ
 * Author: KimiakiK
 * Date: 2021/07/04
 */

#ifndef MCAL_PORT_H_
#define MCAL_PORT_H_

/********** Include **********/

#include "typedef.h"

/********** Define **********/

/* 出力レベル */
#define LEVEL_LOW   (0)
#define LEVEL_HIGH  (1)

/* ピン */
#define PIN_TFT_DC      (0)
#define PIN_TFT_CS      (1)
#define PIN_TFT_RESET   (2)

/********** Enum **********/

/********** Type **********/

/********** Constant **********/

/********** Variable **********/

/********** Function Prototype **********/

void InitPort(void);
void WritePin(uint8_t pin, uint8_t level);

#endif /* MCAL_PORT_H_ */

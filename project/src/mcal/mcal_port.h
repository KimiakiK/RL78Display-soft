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

/* 入出力レベル */
#define LEVEL_LOW   (0)
#define LEVEL_HIGH  (1)

/********** Enum **********/

typedef enum pin_id_enum {
    PIN_ID_TFT_DC = 0,
    PIN_ID_TFT_CS,
    PIN_ID_TFT_RESET,
    PIN_ID_LED,
    PIN_ID_SW_L,
    PIN_ID_SW_R,
    PIN_ID_SW_A,
    PIN_ID_SW_B,
    PIN_ID_SW_C,
    PIN_ID_SW_D
} pin_id_t;

/********** Type **********/

/********** Constant **********/

/********** Variable **********/

/********** Function Prototype **********/

void InitPort(void);
void WritePin(pin_id_t pin, uint8_t level);
uint8_t ReadPin(pin_id_t pin);

#endif /* MCAL_PORT_H_ */

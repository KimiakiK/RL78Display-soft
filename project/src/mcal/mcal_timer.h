/*
 * File: mcal_timer.h
 * Brief: タイマドライバ
 * Author: KimiakiK
 * Date: 2021/07/04
 */

#ifndef include_define
#define include_define

/********** Include **********/

#include "typedef.h"

/********** Define **********/

#define DUTY_MAX        (5000)

/********** Enum **********/

/********** Type **********/

/********** Constant **********/

/********** Variable **********/

/********** Function Prototype **********/

void InitTimer(void);
void WaitMs(uint16_t ms);
void SetDuty(uint16_t duty);
void Start10msTimer(void);
uint8_t Get10msTimerState(void);
void Clear10msTimerState(void);

#endif /* include_define */

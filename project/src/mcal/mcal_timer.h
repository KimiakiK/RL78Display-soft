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

/********** Enum **********/

/********** Type **********/

/********** Constant **********/

/********** Variable **********/

/********** Function Prototype **********/

void InitTimer(void);
void WaitMs(uint16_t ms);
void SetDuty(uint16_t duty);
void Start1msTimer(void);
uint8_t Get1msTimerState(void);
void Clear1msTimerState(void);

#endif /* include_define */

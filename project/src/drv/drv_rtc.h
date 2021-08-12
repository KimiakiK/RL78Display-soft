/*
 * File: drv_rtc.h
 * Brief: RTCドライバ
 * Author: KimiakiK
 * Date: 2021/07/04
 */

#ifndef DRV_RTC_H_
#define DRV_RTC_H_

/********** Include **********/

#include "typedef.h"

/********** Define **********/

/********** Enum **********/

/********** Type **********/

typedef struct {
    uint8_t sec;    /* 秒: 00～59(BCD) */
    uint8_t min;    /* 分: 00～59(BCD) */
    uint8_t hour;   /* 時: 00～23(BCD) */
    uint8_t week;   /* 曜日 0:日, 1:月, 2:火, 3:水, 4:木, 5:金, 6:土 */
    uint8_t day;    /* 日: 01～31(BCD) */
    uint8_t month;  /* 月: 01～12(BCD) */
    uint8_t year;   /* 年: 00～99(BCD) */
} datetime_t;

/********** Constant **********/

/********** Variable **********/

/********** Function Prototype **********/

void InitRtc(void);
void GetDatetime(datetime_t* datetime);

#endif /* DRV_RTC_H_ */

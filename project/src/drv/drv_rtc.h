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
    uint8_t sec;
    uint8_t min;
    uint8_t hour;
    uint8_t week;
    uint8_t day;
    uint8_t month;
    uint8_t year;
} datetime_t;

/********** Constant **********/

/********** Variable **********/

/********** Function Prototype **********/

void InitRtc(void);
void getExRtc(datetime_t* datetime);

#endif /* DRV_RTC_H_ */

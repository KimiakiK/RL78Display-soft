/*
 * File: mcal_rtc.h
 * Brief: 内蔵RTCドライバ
 * Author: KimiakiK
 * Date: 2021/07/31
 */

#ifndef MCAL_RTC_H_
#define MCAL_RTC_H_

/********** Include **********/

#include "typedef.h"
#include "drv_rtc.h"

/********** Define **********/

/********** Enum **********/

/********** Type **********/

/********** Constant **********/

/********** Variable **********/

/********** Function Prototype **********/

void InitMicomRtc(datetime_t* datetime);
void GetMicomRtcDatatime(datetime_t* datetime);
void SetMicomRtcDatetime(datetime_t* datetime);

#endif /* MCAL_RTC_H_ */

/*
 * File: mcal_rtc.c
 * Brief: 内蔵RTCドライバ
 * Author: KimiakiK
 * Date: 2021/07/31
 */

/********** Include **********/

#include "typedef.h"
#include "iodefine.h"
#include "drv_rtc.h"
#include "mcal_rtc.h"

/********** Define **********/

/********** Enum **********/

/********** Type **********/

/********** Constant **********/

/********** Variable **********/

/********** Function Prototype **********/

/********** Function **********/

/*
 * Function: 
 * Argument: 
 * Return: 
 * Note: 
 */
void InitMicomRtc(datetime_t* datetime)
{
    RTCEN = 1;      /* クロック供給 */
    RTCC0 = 0x08;   /* カウンタ動作停止、RTC1HZ端子の出力(1Hz)禁止、24時間制、定周期割り込み禁止 */
    SEC = datetime->sec;
    MIN = datetime->min;
    HOUR = datetime->hour;
    WEEK = datetime->week;
    DAY = datetime->day;
    MONTH = datetime->month;
    YEAR = datetime->year;
    RTCE = 1;       /* カウンタ動作開始 */
}

/*
 * Function: 
 * Argument: 
 * Return: 
 * Note: 
 */
void GetMicomRtcDatatime(datetime_t* datetime)
{
    /* SEC～YEARカウンタ停止。カウンタ値読み出し，書き込みモードへ移行 */
    RWAIT = 1;
    while (RWAIT != 1) {};
    /* 日時取得 */
    datetime->sec = SEC;
    datetime->min = MIN;
    datetime->hour = HOUR;
    datetime->week = WEEK;
    datetime->day = DAY;
    datetime->month = MONTH;
    datetime->year = YEAR;
    /* カウンタ動作設定 */
    RWAIT = 0;
}

/*
 * Function: 
 * Argument: 
 * Return: 
 * Note: 
 */
void SetMicomRtcDatetime(datetime_t* datetime)
{
    /* SEC～YEARカウンタ停止。カウンタ値読み出し，書き込みモードへ移行 */
    RWAIT = 1;
    while (RWAIT != 1) {};
    /* 日時設定 */
    SEC = datetime->sec;
    MIN = datetime->min;
    HOUR = datetime->hour;
    WEEK = datetime->week;
    DAY = datetime->day;
    MONTH = datetime->month;
    YEAR = datetime->year;
    /* カウンタ動作設定 */
    RWAIT = 0;
}

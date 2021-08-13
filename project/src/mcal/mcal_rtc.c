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
 * Function: 内蔵RTC初期化
 * Argument: 日時
 * Return: 無し
 * Note: 内蔵RTCを指定日時で初期化
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
 * Function: 内蔵RTC日時取得
 * Argument: 日時
 * Return: 無し
 * Note: 内蔵RTCの日時を返す
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
 * Function: 内蔵RTC日時設定
 * Argument: 日時
 * Return: 無し
 * Note: 内蔵RTCに指定日時を設定
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

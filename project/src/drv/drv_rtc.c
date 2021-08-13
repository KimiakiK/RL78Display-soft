/*
 * File: drv_rtc.c
 * Brief: RTCドライバ
 * Author: KimiakiK
 * Date: 2021/07/04
 */

/********** Include **********/

#include "typedef.h"
#include "mcal_iic.h"
#include "mcal_rtc.h"
#include "drv_rtc.h"

/********** Define **********/

#define DEV_ADDRESS     (0x64)

#define BIT_VLF         (0x02)

/********** Enum **********/

/********** Type **********/

/********** Constant **********/

/********** Variable **********/

static uint8_t send_data[8];
static uint8_t receive_data[7];

/********** Function Prototype **********/

void initExRtc(datetime_t* datetime);
void getExRtc(datetime_t* datetime);
void setExRtc(datetime_t* datetime);
uint8_t convertWeekMicomToExternal(uint8_t week);
uint8_t convertWeekExternalToMicom(uint8_t week);

/********** Function **********/

/*
 * Function: RTC初期化
 * Argument: 無し
 * Return: 無し
 * Note: 外部RTCの状態を判定して、内部のRTCを初期化
 */
void InitRtc(void)
{
    datetime_t datetime;

     /* 日時初期値 */
    datetime.sec = 0x00;    /* 秒: 0秒 */
    datetime.min = 0x00;    /* 分: 0分 */
    datetime.hour = 0x00;   /* 時: 0時 */
    datetime.week = 0x20;   /* 曜日: 金曜日 */
    datetime.day = 0x01;    /* 日: 1日 */
    datetime.month = 0x01;  /* 月: 1月 */
    datetime.year = 0x21;   /* 年: 2021年 */

    /* 電源電圧低下フラグ(VLF)読み出し */
    send_data[0] = 0x0E;
    ReceiveIic(DEV_ADDRESS, send_data, 1, receive_data, 1, IIC_SYNC);
    /* 外部RTCの動作状態範囲 */
    if ((receive_data[0] & BIT_VLF) == BIT_VLF) {
        /* 外部RTC初期化 */
        initExRtc(&datetime);
    } else {
        /* 外部RTCの日時読み出し */
        getExRtc(&datetime);
    }

    /* 内蔵RTC初期化 */
    InitMicomRtc(&datetime);
}

/*
 * Function: 日時取得
 * Argument: 日時
 * Return: 無し
 * Note: 内蔵RTCの日時を返す
 */
void GetDatetime(datetime_t* datetime)
{
    GetMicomRtcDatatime(datetime);
}

/*
 * Function: 外部RTC初期化
 * Argument: 日時
 * Return: 無し
 * Note: 外部RTC ICのレジスタを全初期化
 */
void initExRtc(datetime_t* datetime)
{
    /* コントロールレジスタ・フラグレジスタ 0x0D, 0x0E 初期化 */
    send_data[0] = 0x0D;    /* レジスタアドレス指定 */
    send_data[1] = 0x00;    /* Extension Register: 初期値設定 */
    send_data[2] = 0x00;    /* Flag Register: 全フラグクリア */
    SendIic(DEV_ADDRESS, send_data, 3, IIC_SYNC);

    /* RAM初期化 0x07～0x0B */
    send_data[0] = 0x07;    /* レジスタアドレス指定 */
    send_data[1] = 0x00;
    send_data[2] = 0x00;
    send_data[3] = 0x00;
    send_data[4] = 0x00;
    send_data[5] = 0x00;
    SendIic(DEV_ADDRESS, send_data, 6, IIC_SYNC);

    /* 日時初期設定 */
    setExRtc(datetime);
}

/*
 * Function: 外部RTC日時設定
 * Argument: 日時
 * Return: 無し
 * Note: 外部RTC ICの日時を設定
 */
void setExRtc(datetime_t* datetime)
{
    /* 秒未満のカウンタリセット 0x0F */
    send_data[0] = 0x0F;    /* レジスタアドレス指定 */
    send_data[1] = 0x41;    /* Control Register: 温度補償間隔2.0s、RESET */

    /* 時刻初期値設定 0x00～0x06 */
    send_data[0] = 0x00;    /* レジスタアドレス指定 */
    send_data[1] = datetime->sec;
    send_data[2] = datetime->min;
    send_data[3] = datetime->hour;
    send_data[4] = convertWeekMicomToExternal(datetime->week);
    send_data[5] = datetime->day;
    send_data[6] = datetime->month;
    send_data[7] = datetime->year;
    SendIic(DEV_ADDRESS, send_data, 8, IIC_SYNC);

    /* 秒未満のカウンタリセット 0x0F */
    send_data[0] = 0x0F;    /* レジスタアドレス指定 */
    send_data[1] = 0x41;    /* Control Register: 温度補償間隔2.0s、RESET */
}

/*
 * Function: 外部RTC日時取得
 * Argument: 日時
 * Return: 無し
 * Note: 外部RTC ICの日時取得
 */
void getExRtc(datetime_t* datetime)
{
    /* 日時読み出し */
    send_data[0] = 0x00;    /* レジスタアドレス指定 */
    ReceiveIic(DEV_ADDRESS, send_data, 1, receive_data, 7, IIC_SYNC);
    datetime->sec = receive_data[0];
    datetime->min = receive_data[1];
    datetime->hour = receive_data[2];
    datetime->week = convertWeekExternalToMicom(receive_data[3]);
    datetime->day = receive_data[4];
    datetime->month = receive_data[5];
    datetime->year = receive_data[6];
}

/*
 * Function: 曜日変換(内蔵→外部)
 * Argument: 曜日
 * Return: 曜日
 * Note: 曜日を内蔵RTCの形式から外部RTCの形式に変換
 */
uint8_t convertWeekMicomToExternal(uint8_t week)
{
    week = 1 << week;

    return week;
}

/*
 * Function: 曜日変換(外部→内蔵)
 * Argument: 曜日
 * Return: 曜日
 * Note: 曜日を外部RTCの形式から内蔵RTCの形式に変換
 */
uint8_t convertWeekExternalToMicom(uint8_t week)
{
    switch (week) {
    case 0x01:
        week = 0;
        break;
    case 0x02:
        week = 1;
        break;
    case 0x04:
        week = 2;
        break;
    case 0x08:
        week = 3;
        break;
    case 0x10:
        week = 4;
        break;
    case 0x20:
        week = 5;
        break;
    case 0x40:
        week = 6;
        break;
    default:
        week = 0;
    break;
    }

    return week;
}

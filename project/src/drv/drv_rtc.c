/*
 * File: drv_rtc.c
 * Brief: RTCドライバ
 * Author: KimiakiK
 * Date: 2021/07/04
 */

/********** Include **********/

#include "typedef.h"
#include "mcal_iic.h"
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

void setExRtc(datetime_t* datetime);

/********** Function **********/

/*
 * Function: 
 * Argument: 
 * Return: 
 * Note: 
 */
void InitRtc(void)
{
    datetime_t datatime;
    /* 電源電圧低下フラグ(VLF)読み出し */
    send_data[0] = 0x0E;
    ReceiveIic(DEV_ADDRESS, send_data, 1, receive_data, 1, IIC_SYNC);
    
    if ((receive_data[0] & BIT_VLF) == BIT_VLF) {
        /* RTC初期化 */

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

        /* 日時初期値 */
        datatime.sec = 0x00;    /* 秒: 0秒 */
        datatime.min = 0x00;    /* 分: 0分 */
        datatime.hour = 0x00;   /* 時: 0時 */
        datatime.week = 0x20;   /* 曜日: 金曜日 */
        datatime.day = 0x01;    /* 日: 1日 */
        datatime.month = 0x01;  /* 月: 1月 */
        datatime.year = 0x21;   /* 年: 2021年 */

        setExRtc(&datatime);
    } else {
        /* 日時読み出し */
        send_data[0] = 0x00;    /* レジスタアドレス指定 */
        ReceiveIic(DEV_ADDRESS, send_data, 1, receive_data, 7, IIC_SYNC);
        send_data[0] = 0x00;
    }

}

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
    send_data[4] = datetime->week;
    send_data[5] = datetime->day;
    send_data[6] = datetime->month;
    send_data[7] = datetime->year;
    SendIic(DEV_ADDRESS, send_data, 8, IIC_SYNC);

    /* 秒未満のカウンタリセット 0x0F */
    send_data[0] = 0x0F;    /* レジスタアドレス指定 */
    send_data[1] = 0x41;    /* Control Register: 温度補償間隔2.0s、RESET */
}

void getExRtc(datetime_t* datetime)
{
    /* 日時読み出し */
    send_data[0] = 0x00;    /* レジスタアドレス指定 */
    ReceiveIic(DEV_ADDRESS, send_data, 1, receive_data, 7, IIC_SYNC);
    datetime->sec = receive_data[0];
    datetime->min = receive_data[1];
    datetime->hour = receive_data[2];
    datetime->week = receive_data[3];
    datetime->day = receive_data[4];
    datetime->month = receive_data[5];
    datetime->year = receive_data[6];
}

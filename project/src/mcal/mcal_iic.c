/*
 * File: mcal_iic.c
 * Brief: IICドライバ
 * Author: KimiakiK
 * Date: 2021/07/04
 */

/********** Include **********/

#include "typedef.h"
#include "iodefine.h"
#include "mcal_iic.h"

/********** Define **********/

/********** Enum **********/

/********** Type **********/

/********** Constant **********/

/********** Variable **********/

static uint8_t iic_state;

static uint8_t target_address;
static uint8_t* send_data;
static uint8_t send_length;
static uint8_t* receive_data;
static uint8_t receive_length;

/********** Function Prototype **********/

/********** Function **********/

/*
 * Function: IIC初期化
 * Argument: 無し
 * Return: 無し
 * Note: IICレジスタ初期化
 */
void InitIic(void)
{
    iic_state = IIC_STATE_IDLE;
    send_data = (uint8_t*)0;
    send_length = 0;
    receive_data = (uint8_t*)0;
    receive_length = 0;

    IICA0EN = 1;        /* クロック供給許可 */
    IICE0 = 1;          /* リセット */
    P6 &= 0xFC;         /* SCLA0とSDAA0端子の出力ラッチを0に設定 */
    PM6 |= 0x03;        /* SCLA0とSDAA0端子を入力に設定 */
    SMC0 = 1;           /* ファースト・モード（最大転送レート：400kbps） */
    IICWL0 = 21;        /* 400kbps */
    IICWH0 = 20;        /* 400kbps */
    SVA0 = 0x00;        /* マスタなので自局アドレスは初期値 */
    IICF0 = 0x02;       /* ストップ・コンディションを検出せずに，スタート・コンディションを生成許可 */
    ACKE0 = 1;          /* アクノリッジを許可 */
    WTIM0 = 1;          /* 9クロック目の立ち下がりで割り込み要求発生 */
    SPIE0 = 1;          /* ストップ・コンディション検出による割り込み要求発生の許可 */
    IICAIF0 = 0;        /* 割り込みフラグクリア */
    IICAMK0 = 0;        /* 割り込みマスク解除 */
    IICE0 = 1;          /* 動作許可 */
    PM6 &= 0xFC;        /* SCLA0とSDAA0端子を出力に設定 */
}

/*
 * Function: IIC通信状態取得
 * Argument: 無し
 * Return: IIC通信状態
 * Note: IIC通信状態を返す
 */
uint8_t GetIicState(void)
{
    return iic_state;
}

/*
 * Function: IICデータ送信
 * Argument: 通信対象デバイスアドレス、送信データ先頭アドレス、送信データ長、送信方法(同期/非同期)
 * Return: 無し
 * Note: 指定データを送信
 */
void SendIic(uint8_t address, uint8_t* data, uint8_t length, uint8_t type)
{
    if (iic_state == IIC_STATE_IDLE) {
        iic_state = IIC_STATE_ADDRESS;

        target_address = address;
        send_data = data;
        send_length = length;
        receive_data = (uint8_t*)0;
        receive_length = 0;

        /* スタート・コンディションを生成 */
        STT0 = 1;
        /* アドレスを送信、bit0はwrite(0) */
        IICA0 = target_address & 0xFE;

        if (type == IIC_SYNC) {
            while (iic_state != IIC_STATE_IDLE) { }
        }
    }
}

/*
 * Function: IICデータ受信
 * Argument: 通信対象デバイスアドレス、送信データ先頭アドレス、送信データ長、受信データ格納アドレス、受信データ長、通信方法(同期/非同期)
 * Return: 無し
 * Note: 指定データを送信後にデータ受信
 */
void ReceiveIic(uint8_t address, uint8_t* s_data, uint8_t s_length, uint8_t* r_data, uint8_t r_length, uint8_t type)
{
    if (iic_state == IIC_STATE_IDLE) {
        iic_state = IIC_STATE_ADDRESS;

        target_address = address;
        send_data = s_data;
        send_length = s_length;
        receive_data = r_data;
        receive_length = r_length;

        /* スタート・コンディションを生成 */
        STT0 = 1;
        /* アドレスを送信、bit0はwrite(0) */
        IICA0 = target_address & 0xFE;

        if (type == IIC_SYNC) {
            while (iic_state != IIC_STATE_IDLE) { }
        }
    }
}

#pragma interrupt IntrIic(vect=INTIICA0)
/*
 * Function: IIC割り込み処理関数
 * Argument: 無し
 * Return: 無し
 * Note: IIC通信の各種割り込みを処理
 */
void IntrIic(void)
{
    switch (iic_state) {
    case IIC_STATE_ADDRESS:
        /* アクノリッジ検出を判定 */
        if (ACKD0 == 1) {
            /* 送信／受信状態判定 */
            if (TRC0 == 1) {
                iic_state = IIC_STATE_SEND;
                /* 最初のデータを送信 */
                IICA0 = *send_data;
                send_data ++;
                send_length --;
            } else {
                iic_state = IIC_STATE_RECEIVE;
                /* アクノリッジを許可 */
                ACKE0 = 1;
                /* 8クロック目の立ち下がりで割り込み要求発生 */
                WTIM0 = 0;
                /* 受信開始 */
                WREL0 = 1;
            }
        } else {
            /* アクノリッジが無いので通信終了 */
            iic_state = IIC_STATE_IDLE;
        }
        break;
    case IIC_STATE_SEND:
        /* アクノリッジ検出を判定 */
        if (ACKD0 == 1) {
            if (send_length > 0) {
                /* 連続データ送信 */
                IICA0 = *send_data;
                send_data ++;
                send_length --;
            } else {
                if (receive_length > 0) {
                    /* データの受信を開始する */
                    iic_state = IIC_STATE_ADDRESS;
                    /* スタート・コンディションを生成 */
                    STT0 = 1;
                    /* アドレスを送信、bit0はread(1) */
                    IICA0 = target_address | 0x01;
                } else {
                    /* 送信終了、ストップ・コンディションを生成 */
                    SPT0 = 1;
                }
            }
        } else {
            /* アクノリッジが無いので通信終了 */
            iic_state = IIC_STATE_IDLE;
        }
        break;
    case IIC_STATE_RECEIVE:
        if (SPD0 == 0) {
            if (receive_length > 0) {
                /* 受信データ格納 */
                *receive_data = IICA0;
                receive_data ++;
                receive_length --;
                if (receive_length > 0) {
                    /* 連続受信 */
                    WREL0 = 1;
                } else {
                    /* アクノリッジを禁止 */
                    ACKE0 = 0;
                    /* 9クロック目の立ち下がりで割り込み要求発生 */
                    WTIM0 = 1;
                    /* ウェイト解除 */
                    WREL0 = 1;
                }
            } else {
                /* 受信終了、ストップ・コンディションを生成 */
                SPT0 = 1;
            }
        } else {
            /* ストップ・コンディション検出、通信終了 */
            iic_state = IIC_STATE_IDLE;
        }
        break;
    default:
        /* 処理なし */
        break;
    }
}

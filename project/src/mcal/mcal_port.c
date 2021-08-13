/*
 * File: mcal_port.c
 * Brief: ポートドライバ
 * Author: KimiakiK
 * Date: 2021/07/04
 */

/********** Include **********/

#include "typedef.h"
#include "iodefine.h"
#include "mcal_port.h"

/********** Define **********/

/********** Enum **********/

/********** Type **********/

/********** Constant **********/

/********** Variable **********/

/********** Function Prototype **********/

/********** Function **********/

/*
 * Function: 端子初期設定
 * Argument: 無し
 * Return: 無し
 * Note: 周辺機能の端子切り替えは各ドライバで設定、未接続端子はLow出力
 */
void InitPort(void)
{
    P0 = 0x00;
    PM0 = 0x8C;

    P1 = 0x38;
    POM1 = 0x3D;
    PM1 = 0x05;

    P2 = 0x00;
    PM2 = 0x03;

    P3 = 0x02;
    PM3 = 0xFC;

    P4 = 0x00;
    PM4 = 0xF1;

    P5 = 0x00;
    PU5 = 0x3C;
    PM5 = 0xFF;

    P6 = 0x00;
    PM6 = 0xF3;

    P7 = 0x00;
    PM7 = 0x00;

    P12 = 0x00;
    PM12 = 0xFE;

    P13 = 0x00;

    P14 = 0x00;
    PM14 = 0x3C;
}

/*
 * Function: 端子出力設定
 * Argument: 端子、出力レベル
 * Return: 無し
 * Note: 指定端子を指定出力レベルに設定
 */
void WritePin(pin_id_t pin, uint8_t level)
{
    switch (pin) {
    case PIN_ID_TFT_DC:
        P1_bit.no4 = level;
        break;
    case PIN_ID_TFT_CS:
        P1_bit.no3 = level;
        break;
    case PIN_ID_TFT_RESET:
        P1_bit.no5 = level;
        break;
    case PIN_ID_LED:
        P3_bit.no1 = level;
        break;
    default:
        /* 処理なし */
        break;
    }
}

/*
 * Function: 端子入力取得
 * Argument: 端子
 * Return: 入力レベル
 * Note: 指定端子の入力レベルを返す
 */
uint8_t ReadPin(pin_id_t pin)
{
    uint8_t level;

    switch (pin) {
    case PIN_ID_SW_L:
        level = P5_bit.no0;
        break;
    case PIN_ID_SW_R:
        level = P5_bit.no1;
        break;
    case PIN_ID_SW_A:
        level = P5_bit.no2;
        break;
    case PIN_ID_SW_B:
        level = P5_bit.no3;
        break;
    case PIN_ID_SW_C:
        level = P5_bit.no4;
        break;
    case PIN_ID_SW_D:
        level = P5_bit.no5;
        break;
    default:
        level = LEVEL_LOW;
        break;
    }

    return level;
}

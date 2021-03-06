/*
 * File: mcal_timer.c
 * Brief: タイマドライバ
 * Author: KimiakiK
 * Date: 2021/07/04
 */

/********** Include **********/

#include "typedef.h"
#include "iodefine.h"
#include "mcal_timer.h"

/********** Define **********/

/********** Enum **********/

/********** Type **********/

/********** Constant **********/

/********** Variable **********/

/********** Function Prototype **********/

/********** Function **********/

/*
 * Function: タイマ初期化
 * Argument: 無し
 * Return: 無し
 * Note: 使用するタイマのレジスタを初期化
 */
void InitTimer(void)
{
    TAU0EN = 1;         /* クロック供給許可 */

    TPS0 = 0x0005;      /* CK00: 1MHz */

    /* ユニット0 チャネル0と1を使用してTO01(P16)にPWM出力(200Hz) */
    TMR00 = 0x0801;     /* PWMマスタチャネル CK00 */
    TMR01 = 0x0409;     /* PWMスレイブチャネル CK00 */
    TDR00 = 4999;       /* PWM周期 200Hz */
    TDR01 = 0;          /* PWM Duty 0% */

    /* ユニット0 チャネル6をウェイト用として使用 */
    TMR06 = 0x0000;     /* インターバルタイマ CK00 */
    TDR06 = 999;        /* 1ms */

    /* ユニット0 チャネル7をメイン周期用として使用 */
    TMR07 = 0x0000;     /* インターバルタイマ CK00 */
    TDR07 = 9999;        /* 10ms */

    /* 出力設定 */
    TOM0 = 0x0002;      /* PWMスレイブチャネル出力 */
    TOL0 = 0x0000;      /* アクティブ・ハイ */
    TO0 = 0x0000;       /* 出力初期値: Low */
    TOE0 = 0x0002;      /* PWMスレイブチャネル出力 */

    /* PWM出力開始 */
    TS0 |= 0x0003;
}

/*
 * Function: ミリ秒待機
 * Argument: 待機ミリ秒[ms]
 * Return: 無し
 * Note: 指定された時間待機
 */
void WaitMs(uint16_t ms)
{
    while (ms > 0) {
        TMIF06 = 0;
        TS0 |= 0x0040;  /* ユニット0 チャネル6 動作開始 */
        while (TMIF06 == 0) { };
        ms --;
    }
    TT0 |= 0x0040;  /* ユニット0 チャネル6 動作停止 */
}

/*
 * Function: PWMデューティ設定
 * Argument: PWMデューティ値
 * Return: 無し
 * Note: PWMのデューティを設定
 */
void SetDuty(uint16_t duty)
{
    TDR01 = duty;
}

/*
 * Function: 10ms周期タイマ開始
 * Argument: 無し
 * Return: 無し
 * Note: 10ms周期用のタイマを開始
 */
void Start10msTimer(void)
{
    TMIF07 = 0;
    TS0 |= 0x0080;  /* ユニット0 チャネル7 動作開始 */
}

/*
 * Function: 10msタイマ状態取得
 * Argument: 無し
 * Return: 10msタイマ状態
 * Note: 10msタイマの経過状態を返す
 */
uint8_t Get10msTimerState(void)
{
    return TMIF07;
}

/*
 * Function: 10msタイマ状態クリア
 * Argument: 無し
 * Return: 無し
 * Note: 10msタイマのフラグをクリア
 */
void Clear10msTimerState(void)
{
    TMIF07 = 0;
}

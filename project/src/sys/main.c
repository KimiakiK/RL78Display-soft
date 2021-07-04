/*
 * File: main.c
 * Brief: メイン関数定義
 * Author: KimiakiK
 * Date: 2021/07/04
 */

/********** Include **********/

#include "typedef.h"
#include "mcal_port.h"
#include "mcal_timer.h"
#include "mcal_spi.h"
#include "mcal_iic.h"
#include "drv_tft.h"
#include "drv_rtc.h"
#include "apl_clock.h"

/********** Define **********/

/********** Enum **********/

/********** Type **********/

/********** Constant **********/

/********** Variable **********/

/********** Function Prototype **********/

/********** Function **********/

/*
 * Function: 初期化とメインループ
 * Argument: 無し
 * Return: 無し
 * Note: 無し
 */
void main(void)
{
    /* 初期化 */
    InitPort();
    InitTimer();
    InitSpi();
    InitIic();

    /* 割り込み許可 */
    __EI();

    InitTft();
    InitRtc();

    InitClock();

    Start1msTimer();

    /* メインループ */
    while (1) {
        MainClock();

        while (Get1msTimerState() == 0) { }
        Clear1msTimerState();
    }
}

/*
 * File: main.c
 * Brief: メイン関数定義
 * Author: KimiakiK
 * Date: 2021/07/04
 */

/********** Include **********/

#include "typedef.h"
#include "mcal_port.h"
#include "mcal_adc.h"
#include "mcal_timer.h"
#include "mcal_spi.h"
#include "mcal_iic.h"
#include "mcal_uart.h"
#include "drv_sw.h"
#include "drv_tft.h"
#include "drv_rtc.h"
#include "drv_light.h"
#include "drv_bm71.h"
#include "apl_display.h"
#include "apl_ble.h"

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
    InitAdc();
    InitTimer();
    InitSpi();
    InitIic();
    InitUart();
    InitSw();
    InitLight();
    InitBM71();
    InitBle();

    /* 割り込み許可 */
    __EI();

    InitTft();
    InitRtc();

    InitDisplay();

    Start10msTimer();

    /* メインループ */
    while (1) {
        MainSw();
        MainBM71();
        MainBle();
        MainDisplay();

        while (Get10msTimerState() == 0) { }
        Clear10msTimerState();
    }
}

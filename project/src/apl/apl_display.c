/*
 * File: apl_display.c
 * Brief: 画面表示アプリ
 * Author: KimiakiK
 * Date: 2021/08/01
 */

/********** Include **********/

#include "typedef.h"
#include "lib_graphic.h"
#include "mcal_port.h"
#include "mcal_pclbuz.h"
#include "drv_sw.h"
#include "drv_tft.h"
#include "drv_rtc.h"
#include "apl_clock.h"
#include "apl_controller.h"
#include "apl_display.h"

/********** Define **********/

#define IMAGE_ADDRESS_TITLE_BACKGROUND      (0x63D00)
#define IMAGE_ADDRESS_CLOCK_BACKGROUND      (0x47B00)
#define IMAGE_ADDRESS_CONTROLLER_BACKGROUND (0x2B900)

/********** Enum **********/

typedef enum display_state_type {
    DISPLAY_STATE_INIT = 0,
    DISPLAY_STATE_OFF,
    DISPLAY_STATE_ON
} display_state_t;

typedef enum display_id_type {
    DISPLAY_ID_TITLE = 0,
    DISPLAY_ID_CLOCK,
    DISPLAY_ID_CONTROLLER,
    DISPLAY_ID_NUM
} display_id_t;

/********** Type **********/

/********** Constant **********/

/********** Variable **********/

static display_state_t display_state;
static display_id_t display_id;

/********** Function Prototype **********/

void transitionDisplayState(void);
void processDisplayState(void);
void transitionDisplayId(void);
void processDisplayId(void);
void displayBackground(void);

/********** Function **********/

/*
 * Function: 画面表示初期化
 * Argument: 無し
 * Return: 無し
 * Note: 変数初期化
 */
void InitDisplay(void)
{
    display_state = DISPLAY_STATE_INIT;
    display_id = DISPLAY_ID_TITLE;
}

/*
 * Function: 画面表示周期処理
 * Argument: 無し
 * Return: 無し
 * Note: 画面遷移と表示更新
 */
void MainDisplay(void)
{
    transitionDisplayState();
    processDisplayState();
}

/*
 * Function: 表示状態遷移
 * Argument: 無し
 * Return: 無し
 * Note: 表示状態の遷移を判定
 */
void transitionDisplayState(void)
{
    switch (display_state) {
    case DISPLAY_STATE_INIT:
        /* 無条件で画面背景を描画して表示開始 */
        displayBackground();
        TftOn();
        InitClock();
        display_state = DISPLAY_STATE_ON;
        break;
    case DISPLAY_STATE_OFF:
        if ( (GetSw(SW_ID_L) == SW_ON) && (GetSw(SW_ID_R) == SW_ON) ) {
            /* SW LとSW R同時押しで画面表示再開 */
            InitRtc();
            displayBackground();
            TftOn();
            InitClock();
            display_state = DISPLAY_STATE_ON;
        }
        break;
    case DISPLAY_STATE_ON:
        if (GetNoInputTime() > 20000) {
            /* 無操作一定時間経過で画面表示停止 */
            TftOff();
            display_state = DISPLAY_STATE_OFF;
        }
        break;
    default:
        /* 処理なし */
        break;
    }
}

/*
 * Function: 表示状態処理
 * Argument: 無し
 * Return: 無し
 * Note: 各表示状態の処理実行
 */
void processDisplayState(void)
{
    switch (display_state) {
    case DISPLAY_STATE_INIT:
        /* 処理なし */
        break;
        /* 処理なし */
    case DISPLAY_STATE_OFF:
        break;
    case DISPLAY_STATE_ON:
        /* 画面遷移 */
        transitionDisplayId();
        /* 画面処理 */
        processDisplayId();
        break;
    default:
        /* 処理なし */
        break;
    }
}

/*
 * Function: 画面遷移
 * Argument: 無し
 * Return: 無し
 * Note: 画面の遷移判定
 */
void transitionDisplayId(void)
{
    display_id_t old_display_id;
    sw_state_t sw_state;

    old_display_id = display_id;

    sw_state = GetSw(SW_ID_L);
    if (sw_state == SW_PUSH) {
        if (display_id == (display_id_t)0) {
            display_id = (display_id_t)(DISPLAY_ID_NUM - 1);
        } else {
            display_id --;
        }
    }
    
    sw_state = GetSw(SW_ID_R);
    if (sw_state == SW_PUSH) {
        if (display_id == (display_id_t)(DISPLAY_ID_NUM - 1)) {
            display_id = (display_id_t)0;
        } else {
            display_id ++;
        }
    }

    if (display_id != old_display_id) {
        displayBackground();
        InitClock();
    }
}

/*
 * Function: 画面処理
 * Argument: 無し
 * Return: 無し
 * Note: 各画面の表示処理実行
 */
void processDisplayId(void)
{
    switch (display_id) {
    case DISPLAY_ID_TITLE:
        /* 画面右上の時計表示 */
        DrawMiniClock();
        
        /* SW操作でブザーとLED制御 */
        if (GetSw(SW_ID_A) == SW_PUSH) {
            PlayBuzzer(FREQUENCY_ID_512HZ);
        } else if (GetSw(SW_ID_A) == SW_RELEASE) {
            StopBuzzer();
        } else {
            /* 処理なし */
        }
        if (GetSw(SW_ID_B) == SW_PUSH) {
            WritePin(PIN_ID_LED, LEVEL_LOW);
        } else if (GetSw(SW_ID_B) == SW_RELEASE) {
            WritePin(PIN_ID_LED, LEVEL_HIGH);
        } else {
            /* 処理なし */
        }

        break;
    case DISPLAY_ID_CLOCK:
        /* 画面中央に日時表示 */
        DrawCenterClock();
        break;
    case DISPLAY_ID_CONTROLLER:
        /* コントローラ操作表示 */
        DrawControllerSw();
        break;
    default:
        /* 処理なし */
        break;
    }
}

/*
 * Function: 画面背景表示
 * Argument: 無し
 * Return: 無し
 * Note: 各画面の背景画像を表示
 */
void displayBackground(void)
{
    switch (display_id) {
    case DISPLAY_ID_TITLE:
        DrawTft(0, 0 , image[IMAGE_ID_BACKGROUND_TITLE].width, image[IMAGE_ID_BACKGROUND_TITLE].heigth, image[IMAGE_ID_BACKGROUND_TITLE].address);
        break;
    case DISPLAY_ID_CLOCK:
        DrawTft(0, 0 , image[IMAGE_ID_BACKGROUND_CLOCK].width, image[IMAGE_ID_BACKGROUND_CLOCK].heigth, image[IMAGE_ID_BACKGROUND_CLOCK].address);
        break;
    case DISPLAY_ID_CONTROLLER:
        DrawTft(0, 0 , image[IMAGE_ID_BACKGROUND_CONTROLLER].width, image[IMAGE_ID_BACKGROUND_CONTROLLER].heigth, image[IMAGE_ID_BACKGROUND_CONTROLLER].address);
        break;
    default:
        /* 処理なし */
        break;
    }
}

/*
 * File: apl_display.c
 * Brief: 画面表示アプリ
 * Author: KimiakiK
 * Date: 2021/08/01
 */

/********** Include **********/

#include "typedef.h"
#include "lib_graphic.h"
#include "drv_sw.h"
#include "drv_tft.h"
#include "apl_clock.h"
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
 * Function: %function%
 * Argument: %argument%
 * Return: %return%
 * Note: %note%
 */
void InitDisplay(void)
{
    display_state = DISPLAY_STATE_INIT;
    display_id = DISPLAY_ID_TITLE;
}

void MainDisplay(void)
{
    transitionDisplayState();
    processDisplayState();
}

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
        break;
    case DISPLAY_STATE_ON:
        break;
    default:
        /* 処理なし */
        break;
    }
}

void processDisplayState(void)
{
    switch (display_state) {
    case DISPLAY_STATE_INIT:
        break;
    case DISPLAY_STATE_OFF:
        break;
    case DISPLAY_STATE_ON:
        transitionDisplayId();
        processDisplayId();
        break;
    default:
        /* 処理なし */
        break;
    }
}

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

void processDisplayId(void)
{
    switch (display_id) {
    case DISPLAY_ID_TITLE:
        DrawMiniClock();
        break;
    case DISPLAY_ID_CLOCK:
        DrawCenterClock();
        break;
    case DISPLAY_ID_CONTROLLER:
        break;
    default:
        /* 処理なし */
        break;
    }
}

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
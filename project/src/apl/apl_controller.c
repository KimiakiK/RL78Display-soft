/*
 * File: apl_controller.c
 * Brief: コントローラ表示アプリ
 * Author: KimiakiK
 * Date: 2021/08/12
 */

/********** Include **********/

#include "typedef.h"
#include "lib_graphic.h"
#include "drv_sw.h"
#include "drv_tft.h"
#include "apl_controller.h"

/********** Define **********/

/********** Enum **********/

/********** Type **********/

/********** Constant **********/

/********** Variable **********/

/********** Function Prototype **********/

/********** Function **********/

/*
 * Function: コントローラ操作表示初期化
 * Argument: 無し
 * Return: 無し
 * Note: コントローラのSW操作OFF状態を表示
 */
void DrawControllerSwInit(void)
{
    uint8_t sw_id;

    for (sw_id=0; sw_id<8; sw_id++) {
        DrawTft(sw_off_sprite[sw_id].x, sw_off_sprite[sw_id].y, sw_off_sprite[sw_id].w, sw_off_sprite[sw_id].h, sw_off_sprite[sw_id].address);
    }
}

/*
 * Function: コントローラ操作表示
 * Argument: 無し
 * Return: 無し
 * Note: コントローラのSW操作を表示
 */
void DrawControllerSw(void)
{
    uint8_t sw_id;
    sw_state_t sw_state;

    for (sw_id=0; sw_id<8; sw_id++) {
        sw_state = GetSw((sw_id_t)(sw_id + SW_ID_A));
        if (sw_state == SW_PUSH) {
            DrawTft(sw_on_sprite[sw_id].x, sw_on_sprite[sw_id].y, sw_on_sprite[sw_id].w, sw_on_sprite[sw_id].h, sw_on_sprite[sw_id].address);
        } else if (sw_state == SW_RELEASE) {
            DrawTft(sw_off_sprite[sw_id].x, sw_off_sprite[sw_id].y, sw_off_sprite[sw_id].w, sw_off_sprite[sw_id].h, sw_off_sprite[sw_id].address);
        } else {
            /* 処理なし */
        }
    }
}
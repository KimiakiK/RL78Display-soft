/*
 * File: apl_clock.c
 * Brief: 時計表示アプリ
 * Author: KimiakiK
 * Date: 2021/07/04
 */

/********** Include **********/

#include "typedef.h"
#include "lib_graphic.h"
#include "lib_font.h"
#include "drv_rtc.h"
#include "drv_tft.h"
#include "apl_clock.h"

/********** Define **********/

#define IMAGE_FONT_30PX_ADDRESS     (0x24F00)
#define IMAGE_FONT_30PX_WIDTH       (450)
#define IMAGE_FONT_30PX_HEIGHT      (30)
#define IMAGE_FONT_50PX_ADDRESS     (0x20E00)
#define IMAGE_FONT_50PX_WIDTH       (330)
#define IMAGE_FONT_50PX_HEIGHT      (50)

#define MINI_CLOCK_WIDTH    (18*6+15*2)
#define MINI_CLOCK_HEIGTH   (30)

/********** Enum **********/

/********** Type **********/

/********** Constant **********/

/********** Variable **********/

static uint8_t old_sec;
static uint8_t old_day;

/********** Function Prototype **********/

/********** Function **********/

/*
 * Function: 
 * Argument: 
 * Return: 
 * Note: 
 */
void InitClock(void)
{
    old_sec = 0xFF;
    old_day = 0xFF;
}

void DrawMiniClock(void)
{
    datetime_t datetime;
    image_t mini_clock_image;

    GetDatetime(&datetime);

    mini_clock_image.address = (uint32_t)((uint8_t __far *)Video_memory);
    mini_clock_image.width = 138;
    mini_clock_image.heigth = 30;

    if (datetime.sec != old_sec) {
        /* 時計表示更新 */
        drawFont(&mini_clock_image, 0, (font_id_t)((datetime.hour >> 4) + FONT_ID_30PX_NUM_0) );
        drawFont(&mini_clock_image, 18, (font_id_t)((datetime.hour & 0x0F) + FONT_ID_30PX_NUM_0) );
        drawFont(&mini_clock_image, 36, FONT_ID_30PX_COLON);
        drawFont(&mini_clock_image, 51, (font_id_t)((datetime.min >> 4) + FONT_ID_30PX_NUM_0) );
        drawFont(&mini_clock_image, 69, (font_id_t)((datetime.min & 0x0F) + FONT_ID_30PX_NUM_0) );
        drawFont(&mini_clock_image, 87, FONT_ID_30PX_COLON);
        drawFont(&mini_clock_image, 102, (font_id_t)((datetime.sec >> 4) + FONT_ID_30PX_NUM_0) );
        drawFont(&mini_clock_image, 120, (font_id_t)((datetime.sec & 0x0F) + FONT_ID_30PX_NUM_0) );
        DrawTft(100, 10, mini_clock_image.width, mini_clock_image.heigth, mini_clock_image.address);
        old_sec = datetime.sec;
    }
}

void DrawCenterClock(void)
{
    datetime_t datetime;
    image_t day_image;
    image_t time_image;

    GetDatetime(&datetime);

    day_image.address = (uint32_t)((uint8_t __far *)Video_memory);
    day_image.width = 234;
    day_image.heigth = 30;

    if (datetime.day != old_day) {
        drawFont(&day_image, 0, FONT_ID_30PX_NUM_2);
        drawFont(&day_image, 18, FONT_ID_30PX_NUM_0);
        drawFont(&day_image, 36, (font_id_t)((datetime.year >> 4) + FONT_ID_30PX_NUM_0) );
        drawFont(&day_image, 54, (font_id_t)((datetime.year & 0x0F) + FONT_ID_30PX_NUM_0) );
        drawFont(&day_image, 72, FONT_ID_30PX_SLASH);
        drawFont(&day_image, 87, (font_id_t)((datetime.month >> 4) + FONT_ID_30PX_NUM_0) );
        drawFont(&day_image, 105, (font_id_t)((datetime.month & 0x0F) + FONT_ID_30PX_NUM_0) );
        drawFont(&day_image, 123, FONT_ID_30PX_SLASH);
        drawFont(&day_image, 138, (font_id_t)((datetime.day >> 4) + FONT_ID_30PX_NUM_0) );
        drawFont(&day_image, 156, (font_id_t)((datetime.day & 0x0F) + FONT_ID_30PX_NUM_0) );
        drawFont(&day_image, 174, FONT_ID_30PX_BRACKET_L);
        drawFont(&day_image, 189, (font_id_t)(datetime.week + FONT_ID_30PX_SUNDAY) );
        drawFont(&day_image, 219, FONT_ID_30PX_BRACKET_R);
        DrawTft(3, 70, day_image.width, day_image.heigth, day_image.address);
        old_day = datetime.day;
    }

    time_image.address = (uint32_t)((uint8_t __far *)Video_memory);
    time_image.width = 240;
    time_image.heigth = 50;

    if (datetime.sec != old_sec) {
        drawFont(&time_image, 0, (font_id_t)((datetime.hour >> 4) + FONT_ID_50PX_NUM_0));
        drawFont(&time_image, 30, (font_id_t)((datetime.hour & 0x0F) + FONT_ID_50PX_NUM_0) );
        drawFont(&time_image, 60, FONT_ID_50PX_COLON);
        drawFont(&time_image, 90, (font_id_t)((datetime.min >> 4) + FONT_ID_50PX_NUM_0));
        drawFont(&time_image, 120, (font_id_t)((datetime.min & 0x0F) + FONT_ID_50PX_NUM_0) );
        drawFont(&time_image, 150, FONT_ID_50PX_COLON);
        drawFont(&time_image, 180, (font_id_t)((datetime.sec >> 4) + FONT_ID_50PX_NUM_0));
        drawFont(&time_image, 210, (font_id_t)((datetime.sec & 0x0F) + FONT_ID_50PX_NUM_0) );
        DrawTft(0, 110, time_image.width, time_image.heigth, time_image.address);
        old_sec = datetime.sec;
    }
}

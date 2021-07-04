/*
 * File: apl_clock.c
 * Brief: 時計表示アプリ
 * Author: KimiakiK
 * Date: 2021/07/04
 */

/********** Include **********/

#include "typedef.h"
#include "drv_rtc.h"
#include "drv_tft.h"

/********** Define **********/

#define IMG_ADDRESS     (0x30000)

#define LETTER_WIDTH    (20)
#define LETTER_HEIGTH   (31)
#define LETTER_NUM      (8)

/********** Enum **********/

/********** Type **********/

/********** Constant **********/

/********** Variable **********/

static uint16_t count;
static datetime_t datetime;

static uint8_t draw_buffer[LETTER_WIDTH * LETTER_HEIGTH * LETTER_NUM * 2];

/********** Function Prototype **********/

void drawNumber(uint8_t number, uint8_t pos);

/********** Function **********/

/*
 * Function: 
 * Argument: 
 * Return: 
 * Note: 
 */
void InitClock(void)
{
    count = 0;
}

void MainClock(void)
{
    if (count == 0) {
        /* 時計表示更新 */
        getExRtc(&datetime);
        drawNumber(datetime.hour >> 4, 0);
        drawNumber(datetime.hour & 0x0F, 1);
        drawNumber(10, 2);
        drawNumber(datetime.min >> 4, 3);
        drawNumber(datetime.min & 0x0F, 4);
        drawNumber(10, 5);
        drawNumber(datetime.sec >> 4, 6);
        drawNumber(datetime.sec & 0x0F, 7);
        DrawTft(70, 10, LETTER_WIDTH * LETTER_NUM, LETTER_HEIGTH, (uint32_t)((uint8_t __far *)draw_buffer));
    }

    count ++;
    if (count >= 100) {
        count = 0;
    }
}

void drawNumber(uint8_t number, uint8_t pos)
{
    uint8_t x, y;
    uint16_t index;
    uint32_t address;

    for (y=0; y<LETTER_HEIGTH; y++) {
        for (x=0; x<LETTER_WIDTH; x++) {
            index = (x * 2 + pos * LETTER_WIDTH * 2) + (y * LETTER_WIDTH * LETTER_NUM * 2);
            address = IMG_ADDRESS + ((uint32_t)x * 2) + ((uint32_t)y * LETTER_WIDTH * 2) + ((uint32_t)number * LETTER_WIDTH * LETTER_HEIGTH * 2);
            draw_buffer[index] = *(uint8_t __far *)address;
            draw_buffer[index + 1] = *(uint8_t __far *)(address + 1);
        }
    }
}

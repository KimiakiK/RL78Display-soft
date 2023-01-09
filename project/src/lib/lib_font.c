/*
 * File: lib_font.c
 * Brief: フォントライブラリ
 * Author: KimiakiK
 * Date: 2021/08/12
 */

/********** Include **********/

#include "typedef.h"
#include "lib_graphic.h"
#include "lib_font.h"
#include "drv_tft.h"

/********** Define **********/

/********** Enum **********/

/********** Type **********/

typedef struct font_struct {
    image_t* font_image;
    uint16_t width;
    uint16_t pos_x;
} font_t;

/********** Constant **********/

const font_t font[FONT_ID_NUM] = {
    /* FONT_ID_30PX_NUM_0 */
    {(image_t*)&image[IMAGE_ID_FONT_30PX], 18, 0},
    /* FONT_ID_30PX_NUM_1 */
    {(image_t*)&image[IMAGE_ID_FONT_30PX], 18, 18},
    /* FONT_ID_30PX_NUM_2 */
    {(image_t*)&image[IMAGE_ID_FONT_30PX], 18, 36},
    /* FONT_ID_30PX_NUM_3 */
    {(image_t*)&image[IMAGE_ID_FONT_30PX], 18, 54},
    /* FONT_ID_30PX_NUM_4 */
    {(image_t*)&image[IMAGE_ID_FONT_30PX], 18, 72},
    /* FONT_ID_30PX_NUM_5 */
    {(image_t*)&image[IMAGE_ID_FONT_30PX], 18, 90},
    /* FONT_ID_30PX_NUM_6 */
    {(image_t*)&image[IMAGE_ID_FONT_30PX], 18, 108},
    /* FONT_ID_30PX_NUM_7 */
    {(image_t*)&image[IMAGE_ID_FONT_30PX], 18, 126},
    /* FONT_ID_30PX_NUM_8 */
    {(image_t*)&image[IMAGE_ID_FONT_30PX], 18, 144},
    /* FONT_ID_30PX_NUM_9 */
    {(image_t*)&image[IMAGE_ID_FONT_30PX], 18, 162},
    /* FONT_ID_30PX_COLON */
    {(image_t*)&image[IMAGE_ID_FONT_30PX], 15, 180},
    /* FONT_ID_30PX_SLASH */
    {(image_t*)&image[IMAGE_ID_FONT_30PX], 15, 195},
    /* FONT_ID_30PX_BRACKET_L */
    {(image_t*)&image[IMAGE_ID_FONT_30PX], 15, 210},
    /* FONT_ID_30PX_BRACKET_R */
    {(image_t*)&image[IMAGE_ID_FONT_30PX], 15, 225},
    /* FONT_ID_30PX_SUNDAY */
    {(image_t*)&image[IMAGE_ID_FONT_30PX], 30, 240},
    /* FONT_ID_30PX_MONDAY */
    {(image_t*)&image[IMAGE_ID_FONT_30PX], 30, 270},
    /* FONT_ID_30PX_TUESDAY */
    {(image_t*)&image[IMAGE_ID_FONT_30PX], 30, 300},
    /* FONT_ID_30PX_WEDNESDAY */
    {(image_t*)&image[IMAGE_ID_FONT_30PX], 30, 330},
    /* FONT_ID_30PX_THURSDAY */
    {(image_t*)&image[IMAGE_ID_FONT_30PX], 30, 360},
    /* FONT_ID_30PX_FRIDAY */
    {(image_t*)&image[IMAGE_ID_FONT_30PX], 30, 390},
    /* FONT_ID_30PX_SATURDAY */
    {(image_t*)&image[IMAGE_ID_FONT_30PX], 30, 420},
    /* FONT_ID_50PX_NUM_0 */
    {(image_t*)&image[IMAGE_ID_FONT_50PX], 30, 0},
    /* FONT_ID_50PX_NUM_1 */
    {(image_t*)&image[IMAGE_ID_FONT_50PX], 30, 30},
    /* FONT_ID_50PX_NUM_2 */
    {(image_t*)&image[IMAGE_ID_FONT_50PX], 30, 60},
    /* FONT_ID_50PX_NUM_3 */
    {(image_t*)&image[IMAGE_ID_FONT_50PX], 30, 90},
    /* FONT_ID_50PX_NUM_4 */
    {(image_t*)&image[IMAGE_ID_FONT_50PX], 30, 120},
    /* FONT_ID_50PX_NUM_5 */
    {(image_t*)&image[IMAGE_ID_FONT_50PX], 30, 150},
    /* FONT_ID_50PX_NUM_6 */
    {(image_t*)&image[IMAGE_ID_FONT_50PX], 30, 180},
    /* FONT_ID_50PX_NUM_7 */
    {(image_t*)&image[IMAGE_ID_FONT_50PX], 30, 210},
    /* FONT_ID_50PX_NUM_8 */
    {(image_t*)&image[IMAGE_ID_FONT_50PX], 30, 240},
    /* FONT_ID_50PX_NUM_9 */
    {(image_t*)&image[IMAGE_ID_FONT_50PX], 30, 270},
    /* FONT_ID_50PX_COLON */
    {(image_t*)&image[IMAGE_ID_FONT_50PX], 30, 300},
};

/********** Variable **********/

/********** Function Prototype **********/

/********** Function **********/

/*
 * Function: 文字描画
 * Argument: 描画対象メモリ、横位置、フォントID
 * Return: 無し
 * Note: 指定文字を描画対象メモリに描画
 */
void drawFont(image_t* target_image, uint16_t pos_x, font_id_t font_id)
{
    uint8_t x, y;
    uint16_t index;
    uint32_t address;

    for (y=0; y<font[font_id].font_image->heigth; y++) {
        for (x=0; x<font[font_id].width; x++) {
            index = (x * 2 + pos_x * 2) + (y * target_image->width * 2);
            address = font[font_id].font_image->address + ((uint32_t)x * 2 + (uint32_t)font[font_id].pos_x * 2) + ((uint32_t)y * font[font_id].font_image->width * 2);
            Video_memory[index] = *(uint8_t __far *)address;
            Video_memory[index + 1] = *(uint8_t __far *)(address + 1);
        }
    }
}

/*
 * Function: ASCII文字描画
 * Argument: ASCIIコード、横位置、縦位置
 * Return: 無し
 * Note: 指定文字を描画
 */
void DrawAscii(uint8_t ascii, uint16_t x, uint16_t y)
{
    if ((ascii >= 0x20) && ascii <= 0x7E) {
        DrawTft(x, y, 10, 20, (uint32_t)image[IMAGE_ID_FONT_20PX].address + (uint32_t)(ascii - 0x20) * (10 * 20) * 2);
    }
}

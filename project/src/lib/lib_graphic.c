/*
 * File: lib_graphic.c
 * Brief: 描画ライブラリ
 * Author: KimiakiK
 * Date: 2021/08/12
 */

/********** Include **********/

#include "typedef.h"
#include "lib_graphic.h"

/********** Define **********/

/********** Enum **********/

/********** Type **********/

/********** Constant **********/

const image_t image[IMAGE_ID_NUM] = {
    /* IMAGE_ID_BACKGROUND_TITLE */
    { 0x63D00, 240, 240},
    /* IMAGE_ID_BACKGROUND_CLOCK */
    { 0x47B00, 240, 240},
    /* IMAGE_ID_FONT_20PX */
    { 0x3E600, 950, 20},
    /* IMAGE_ID_FONT_30PX */
    { 0x37C00, 450, 30},
    /* IMAGE_ID_FONT_50PX */
    { 0x2FB00, 330, 50},
    /* IMAGE_ID_SW_OFF */
    { 0x25200, 416, 52},
    /* IMAGE_ID_SW_ON */
    { 0x1A900, 416, 52}
};

const sprite_t sw_off_sprite[8] = {
    /* SW A */
    { 0x25200, 184, 184, 52, 52},
    /* SW B  */
    { 0x26720, 4, 184, 52, 52},
    /* SW C */
    { 0x27C40, 4, 4, 52, 52},
    /* SW D  */
    { 0x29160, 184, 4, 52, 52},
    /* SW POS U */
    { 0x2A680, 94, 24, 52, 52},
    /* SW POS D */
    { 0x2BBA0, 94, 164, 52, 52},
    /* SW POS L */
    { 0x2D0C0, 24, 94, 52, 52},
    /* SW POS R */
    { 0x2E5E0, 164, 94, 52, 52},
};

const sprite_t sw_on_sprite[8] = {
    /* SW A */
    { 0x1A900, 184, 184, 52, 52},
    /* SW B  */
    { 0x1BE20, 4, 184, 52, 52},
    /* SW C */
    { 0x1D340, 4, 4, 52, 52},
    /* SW D  */
    { 0x1E860, 184, 4, 52, 52},
    /* SW POS U */
    { 0x1FD80, 94, 24, 52, 52},
    /* SW POS D */
    { 0x212A0, 94, 164, 52, 52},
    /* SW POS L */
    { 0x227C0, 24, 94, 52, 52},
    /* SW POS R */
    { 0x23CE0, 164, 94, 52, 52},
};

/********** Variable **********/

uint8_t Video_memory[240*60*2];

/********** Function Prototype **********/

/********** Function **********/

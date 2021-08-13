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
    /* IMAGE_ID_BACKGROUND_CONTROLLER */
    { 0x2B900, 240, 240},
    /* IMAGE_ID_FONT_30PX */
    { 0x24F00, 450, 30},
    /* IMAGE_ID_FONT_50PX */
    { 0x1CE00, 330, 50},
    /* IMAGE_ID_SW_OFF */
    { 0x12500, 416, 52},
    /* IMAGE_ID_SW_ON */
    { 0x7C00, 416, 52}
};

const sprite_t sw_off_sprite[8] = {
    /* SW A */
    { 0x12500, 184, 184, 52, 52},
    /* SW  */
    { 0x13A20, 4, 184, 52, 52},
    /* SW  */
    { 0x14F40, 4, 4, 52, 52},
    /* SW  */
    { 0x16460, 184, 4, 52, 52},
    /* SW POS U */
    { 0x17980, 94, 24, 52, 52},
    /* SW POS D */
    { 0x18EA0, 94, 164, 52, 52},
    /* SW POS L */
    { 0x1A3C0, 24, 94, 52, 52},
    /* SW POS R */
    { 0x1B8E0, 164, 94, 52, 52},
};

const sprite_t sw_on_sprite[8] = {
    /* SW A */
    { 0x7C00, 184, 184, 52, 52},
    /* SW  */
    { 0x9120, 4, 184, 52, 52},
    /* SW  */
    { 0xA640, 4, 4, 52, 52},
    /* SW  */
    { 0xBB60, 184, 4, 52, 52},
    /* SW POS U */
    { 0xD080, 94, 24, 52, 52},
    /* SW POS D */
    { 0xE5A0, 94, 164, 52, 52},
    /* SW POS L */
    { 0xFAC0, 24, 94, 52, 52},
    /* SW POS R */
    { 0x10FE0, 164, 94, 52, 52},
};

/********** Variable **********/

uint8_t Video_memory[240*60*2];

/********** Function Prototype **********/

/********** Function **********/

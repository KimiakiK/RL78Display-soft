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
    { 0x1CE00, 330, 50}
};

/********** Variable **********/

uint8_t Video_memory[240*60*2];

/********** Function Prototype **********/

/********** Function **********/

/*
 * Function: 
 * Argument: 
 * Return: 
 * Note: 
 */

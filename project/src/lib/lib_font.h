/*
 * File: lib_font.h
 * Brief: フォントライブラリ
 * Author: KimiakiK
 * Date: 2021/08/12
 */

#ifndef LIB_FONT_H_
#define LIB_FONT_H_

/********** Include **********/

#include "typedef.h"

/********** Define **********/

/********** Enum **********/

typedef enum font_id_enum {
    FONT_ID_30PX_NUM_0 = 0,
    FONT_ID_30PX_NUM_1,
    FONT_ID_30PX_NUM_2,
    FONT_ID_30PX_NUM_3,
    FONT_ID_30PX_NUM_4,
    FONT_ID_30PX_NUM_5,
    FONT_ID_30PX_NUM_6,
    FONT_ID_30PX_NUM_7,
    FONT_ID_30PX_NUM_8,
    FONT_ID_30PX_NUM_9,
    FONT_ID_30PX_COLON,
    FONT_ID_30PX_SLASH,
    FONT_ID_30PX_BRACKET_L,
    FONT_ID_30PX_BRACKET_R,
    FONT_ID_30PX_SUNDAY,
    FONT_ID_30PX_MONDAY,
    FONT_ID_30PX_TUESDAY,
    FONT_ID_30PX_WEDNESDAY,
    FONT_ID_30PX_THURSDAY,
    FONT_ID_30PX_FRIDAY,
    FONT_ID_30PX_SATURDAY,
    FONT_ID_50PX_NUM_0,
    FONT_ID_50PX_NUM_1,
    FONT_ID_50PX_NUM_2,
    FONT_ID_50PX_NUM_3,
    FONT_ID_50PX_NUM_4,
    FONT_ID_50PX_NUM_5,
    FONT_ID_50PX_NUM_6,
    FONT_ID_50PX_NUM_7,
    FONT_ID_50PX_NUM_8,
    FONT_ID_50PX_NUM_9,
    FONT_ID_50PX_COLON,
    FONT_ID_NUM
} font_id_t;

/********** Type **********/

/********** Constant **********/

/********** Variable **********/

/********** Function Prototype **********/

void drawFont(image_t* target_image, uint16_t pos_x, font_id_t font_id);

#endif /* LIB_FONT_H_ */

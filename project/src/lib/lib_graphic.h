/*
 * File: lib_graphic.h
 * Brief: 描画ライブラリ
 * Author: KimiakiK
 * Date: 2021/08/12
 */

#ifndef LIB_GRAPHIC_H_
#define LIB_GRAPHIC_H_

/********** Include **********/

#include "typedef.h"

/********** Define **********/

/********** Enum **********/

typedef enum image_id_enum {
    IMAGE_ID_BACKGROUND_TITLE = 0,
    IMAGE_ID_BACKGROUND_CLOCK,
    IMAGE_ID_BACKGROUND_CONTROLLER,
    IMAGE_ID_FONT_30PX,
    IMAGE_ID_FONT_50PX,
    IMAGE_ID_NUM
} image_id_t;

/********** Type **********/

typedef struct image_struct {
    uint32_t address;
    uint16_t width;
    uint16_t heigth;
} image_t;

/********** Constant **********/

extern const image_t image[IMAGE_ID_NUM];

/********** Variable **********/

extern uint8_t Video_memory[240*60*2];      /* 28800byte */

/********** Function Prototype **********/

#endif /* LIB_GRAPHIC_H_ */

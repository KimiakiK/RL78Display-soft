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
    IMAGE_ID_FONT_20PX,
    IMAGE_ID_FONT_30PX,
    IMAGE_ID_FONT_50PX,
    IMAGE_ID_SW_OFF,
    IMAGE_ID_SW_ON,
    IMAGE_ID_NUM
} image_id_t;

/********** Type **********/

typedef struct image_struct {
    uint32_t address;
    uint16_t width;
    uint16_t heigth;
} image_t;

typedef struct sprite_struct {
    uint32_t address;
    uint8_t x;
    uint8_t y;
    uint8_t w;
    uint8_t h;
} sprite_t;

/********** Constant **********/

extern const image_t image[IMAGE_ID_NUM];
extern const sprite_t sw_off_sprite[8];
extern const sprite_t sw_on_sprite[8];

/********** Variable **********/

extern uint8_t Video_memory[240*60*2];      /* 28800byte */

/********** Function Prototype **********/

#endif /* LIB_GRAPHIC_H_ */

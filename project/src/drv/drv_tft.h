/*
 * File: drv_tft.h
 * Brief: TFT(ST7789)ドライバ
 * Author: KimiakiK
 * Date: 2021/07/04
 */

#ifndef DRV_TFT_H_
#define DRV_TFT_H_

/********** Include **********/

#include "typedef.h"

/********** Define **********/

/********** Enum **********/

/********** Type **********/

/********** Constant **********/

/********** Variable **********/

/********** Function Prototype **********/

void InitTft(void);
void DrawTft(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint32_t data);

#endif /* DRV_TFT_H_ */
/*
 * File: apl_display.h
 * Brief: 画面表示アプリ
 * Author: KimiakiK
 * Date: 2021/08/01
 */

#ifndef APL_DISPLAY_H_
#define APL_DISPLAY_H_

/********** Include **********/

#include "typedef.h"

/********** Define **********/

/********** Enum **********/

typedef enum {
	DISPLAY_TRANSITION_DISABLE = 0,
	DISPLAY_TRANSITION_ENABLE
} display_transition_enable_t;

typedef enum {
	DISPLAY_SLEEP_DISABLE = 0,
	DISPLAY_SLEEP_ENABLE
} display_sleep_enable_t;

/********** Type **********/

/********** Constant **********/

/********** Variable **********/

/********** Function Prototype **********/

void InitDisplay(void);
void MainDisplay(void);

#endif /* APL_DISPLAY_H_ */

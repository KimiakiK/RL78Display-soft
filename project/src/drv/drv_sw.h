/*
 * File: drv_sw.h
 * Brief: スイッチ入力ドライバ
 * Author: KimiakiK
 * Date: 2021/08/12
 */

#ifndef DRV_SW_H_
#define DRV_SW_H_

/********** Include **********/

#include "typedef.h"

/********** Define **********/

#define SW_ID_NUM   (10)

/********** Enum **********/

typedef enum sw_state_enum {
    SW_OFF = 0,
    SW_PUSH,
    SW_ON,
    SW_RELEASE
} sw_state_t;

typedef enum sw_id_enum {
    SW_ID_L = 0,
    SW_ID_R,
    SW_ID_A,
    SW_ID_B,
    SW_ID_C,
    SW_ID_D,
    SW_ID_POS_U,
    SW_ID_POS_D,
    SW_ID_POS_L,
    SW_ID_POS_R
} sw_id_t;

/********** Type **********/

/********** Constant **********/

/********** Variable **********/

/********** Function Prototype **********/

void InitSw(void);
void MainSw(void);
sw_state_t GetSw(sw_id_t sw_id);
uint16_t GetNoInputTime(void);

#endif /* DRV_SW_H_ */

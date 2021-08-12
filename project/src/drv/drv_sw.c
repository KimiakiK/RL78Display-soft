/*
 * File: drv_sw.c
 * Brief: スイッチ入力ドライバ
 * Author: KimiakiK
 * Date: 2021/08/12
 */

/********** Include **********/

#include "typedef.h"
#include "mcal_port.h"
#include "mcal_adc.h"
#include "drv_sw.h"

/********** Define **********/

#define SW_FIX_MATCH_NUM    (4)

/********** Enum **********/

typedef enum sw_value_enum {
    SW_VALUE_OFF = 0,
    SW_VALUE_ON
} sw_value_t;

/********** Type **********/

/********** Constant **********/

/********** Variable **********/

sw_state_t sw_state[SW_ID_NUM];
sw_value_t current_raw_value[SW_ID_NUM];
sw_value_t old_raw_value[SW_ID_NUM];
sw_value_t current_fix_value[SW_ID_NUM];
sw_value_t old_fix_value[SW_ID_NUM];
uint8_t match_num[SW_ID_NUM];


/********** Function Prototype **********/

void makeRowValue(void);
void judgeFixValue(void);
void judgeSwState(void);
void updateOldValue(void);

/********** Function **********/

/*
 * Function: 
 * Argument: 
 * Return: 
 * Note: 
 */
void InitSw(void)
{
    uint8_t index;

    for (index=0; index< SW_ID_NUM; index++) {
        sw_state[index] = SW_OFF;
        current_raw_value[index] = SW_VALUE_OFF;
        old_raw_value[index] = SW_VALUE_OFF;
        current_fix_value[index] = SW_VALUE_OFF;
        old_fix_value[index] = SW_VALUE_OFF;
        match_num[index] = SW_FIX_MATCH_NUM;
    }
}

/*
 * Function: 
 * Argument: 
 * Return: 
 * Note: 
 */
void MainSw(void)
{
    makeRowValue();
    judgeFixValue();
    judgeSwState();
    updateOldValue();
}

/*
 * Function: 
 * Argument: 
 * Return: 
 * Note: 
 */
sw_state_t GetSw(sw_id_t sw_id)
{
    return sw_state[sw_id];
}

/*
 * Function: 
 * Argument: 
 * Return: 
 * Note: 
 */
void judgeFixValue(void)
{
    uint8_t index;

    for (index=0; index< SW_ID_NUM; index++) {
        if (current_raw_value[index] == old_raw_value[index]) {
            if (match_num[index] == 0) {
                /* 入力確定 */
                current_fix_value[index] = current_raw_value[index];
            } else {
                /* 一致回数計算 */
                match_num[index] --;
            }
        } else {
            match_num[index] = SW_FIX_MATCH_NUM;
        }
    }
}

/*
 * Function: 
 * Argument: 
 * Return: 
 * Note: 
 */
void judgeSwState(void)
{
    uint8_t index;

    for (index=0; index< SW_ID_NUM; index++) {
        if (current_fix_value[index] == SW_VALUE_ON) {
            if (old_fix_value[index] == SW_VALUE_ON) {
                sw_state[index] = SW_ON;
            } else {
                sw_state[index] = SW_PUSH;
            }
        } else {
            if (old_fix_value[index] == SW_VALUE_OFF) {
                sw_state[index] = SW_OFF;
            } else {
                sw_state[index] = SW_RELEASE;
            }
        }
    }
}

/*
 * Function: 
 * Argument: 
 * Return: 
 * Note: 
 */
void makeRowValue(void)
{
    uint8_t pin_level;
    uint16_t ad_value;

    pin_level = ReadPin(PIN_ID_SW_L);
    if (pin_level == LEVEL_HIGH) {
        current_raw_value[SW_ID_L] = SW_VALUE_ON;
    } else {
        current_raw_value[SW_ID_L] = SW_VALUE_OFF;
    }

    pin_level = ReadPin(PIN_ID_SW_R);
    if (pin_level == LEVEL_HIGH) {
        current_raw_value[SW_ID_R] = SW_VALUE_ON;
    } else {
        current_raw_value[SW_ID_R] = SW_VALUE_OFF;
    }

    pin_level = ReadPin(PIN_ID_SW_A);
    if (pin_level == LEVEL_LOW) {
        current_raw_value[SW_ID_A] = SW_VALUE_ON;
    } else {
        current_raw_value[SW_ID_A] = SW_VALUE_OFF;
    }

    pin_level = ReadPin(PIN_ID_SW_B);
    if (pin_level == LEVEL_LOW) {
        current_raw_value[SW_ID_B] = SW_VALUE_ON;
    } else {
        current_raw_value[SW_ID_B] = SW_VALUE_OFF;
    }

    pin_level = ReadPin(PIN_ID_SW_C);
    if (pin_level == LEVEL_LOW) {
        current_raw_value[SW_ID_C] = SW_VALUE_ON;
    } else {
        current_raw_value[SW_ID_C] = SW_VALUE_OFF;
    }

    pin_level = ReadPin(PIN_ID_SW_D);
    if (pin_level == LEVEL_LOW) {
        current_raw_value[SW_ID_D] = SW_VALUE_ON;
    } else {
        current_raw_value[SW_ID_D] = SW_VALUE_OFF;
    }

    ad_value = GetAdc(ADC_ID_POS_V);
    if (ad_value > 900) {
        current_raw_value[SW_ID_POS_U] = SW_VALUE_OFF;
        current_raw_value[SW_ID_POS_D] = SW_VALUE_ON;
    } else if (ad_value < 100) {
        current_raw_value[SW_ID_POS_U] = SW_VALUE_ON;
        current_raw_value[SW_ID_POS_D] = SW_VALUE_OFF;
    } else {
        current_raw_value[SW_ID_POS_U] = SW_VALUE_OFF;
        current_raw_value[SW_ID_POS_D] = SW_VALUE_OFF;
    }
    
    ad_value = GetAdc(ADC_ID_POS_H);
    if (ad_value > 900) {
        current_raw_value[SW_ID_POS_L] = SW_VALUE_OFF;
        current_raw_value[SW_ID_POS_R] = SW_VALUE_ON;
    } else if (ad_value < 100) {
        current_raw_value[SW_ID_POS_L] = SW_VALUE_ON;
        current_raw_value[SW_ID_POS_R] = SW_VALUE_OFF;
    } else {
        current_raw_value[SW_ID_POS_L] = SW_VALUE_OFF;
        current_raw_value[SW_ID_POS_R] = SW_VALUE_OFF;
    }
}

void updateOldValue(void)
{
    uint8_t index;

    for (index=0; index< SW_ID_NUM; index++) {
        old_raw_value[index] = current_raw_value[index];
        old_fix_value[index] = current_fix_value[index];
    }
}

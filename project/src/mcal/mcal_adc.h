/*
 * File: mcal_adc.h
 * Brief: AD変換ドライバ
 * Author: KimiakiK
 * Date: 2021/08/12
 */

#ifndef MCAL_ADC_H_
#define MCAL_ADC_H_

/********** Include **********/

#include "typedef.h"

/********** Define **********/

#define ADC_ID_NUM      (2)

/********** Enum **********/

typedef enum adc_id_enum {
    ADC_ID_POS_H = 0,
    ADC_ID_POS_V
} adc_id_t;

/********** Type **********/

/********** Constant **********/

/********** Variable **********/

/********** Function Prototype **********/

void InitAdc(void);
uint16_t GetAdc(adc_id_t adc_id);

#endif /* MCAL_ADC_H_ */

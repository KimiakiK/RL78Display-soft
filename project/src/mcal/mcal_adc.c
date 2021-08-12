/*
 * File: mcal_adc.c
 * Brief: AD変換ドライバ
 * Author: KimiakiK
 * Date: 2021/08/12
 */

/********** Include **********/

#include "typedef.h"
#include "iodefine.h"
#include "mcal_adc.h"

/********** Define **********/

/********** Enum **********/

/********** Type **********/

/********** Constant **********/

/********** Variable **********/

/********** Function Prototype **********/

/********** Function **********/

/*
 * Function: 
 * Argument: 
 * Return: 
 * Note: 
 */
void InitAdc(void)
{
    ADCEN = 1;      /* クロック供給許可 */
    ADPC = 0x03;    /* ANI0, ANI1をアナログポート */
    ADM0 = 0x18;    /* AD変換時間 4.75us */
    ADM1 = 0x20;    /* ソフトウエア・トリガ・モード、ワンショット変換 */
    ADM2 = 0x00;    /* 基準電圧 VDD, VSS、10bit変換 */
    ADCE = 1;       /* AD電圧コンパレータの動作許可 */
}

/*
 * Function: 
 * Argument: 
 * Return: 
 * Note: 
 */
uint16_t GetAdc(adc_id_t adc_id)
{
    uint16_t ad_value;

    /* アナログ入力チャネル指定レジスタ */
    ADS = adc_id;
    ADIF = 0;
    /* AD変換開始 */
    ADCS = 1;
    /* AD変換完了待ち */
    while (ADIF == 0) { }
    /* AD変換結果格納 */
    ad_value = ADCR >> 6;

    return ad_value;
}

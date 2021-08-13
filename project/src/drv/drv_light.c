/*
 * File: drv_light.c
 * Brief: 照明ドライバ
 * Author: KimiakiK
 * Date: 2021/08/01
 */

/********** Include **********/

#include "typedef.h"
#include "mcal_timer.h"
#include "drv_light.h"

/********** Define **********/

/********** Enum **********/

/********** Type **********/

/********** Constant **********/

/********** Variable **********/

static uint8_t duty;

/********** Function Prototype **********/

/********** Function **********/

/*
 * Function: 照明初期化
 * Argument: 無し
 * Return: 無し
 * Note: 変数初期化
 */
void InitLight(void)
{
    duty = 50;
}


/*
 * Function: 照明点灯
 * Argument: 無し
 * Return: 無し
 * Note: 照明用のPWM出力設定
 */
void LightOn(void)
{
    SetDuty(duty * (DUTY_MAX / 100));
}

/*
 * Function: 照明消灯
 * Argument: 無し
 * Return: 無し
 * Note: 照明用のPWM出力停止
 */
void LightOff(void)
{
    SetDuty(0);
}

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
 * Function: %function%
 * Argument: %argument%
 * Return: %return%
 * Note: %note%
 */
void InitLight(void)
{
    duty = 50;
}

void LightOn(void)
{
    SetDuty(duty * (DUTY_MAX / 100));
}

void LightOff(void)
{
    SetDuty(0);
}

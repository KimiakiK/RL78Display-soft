/*
 * File: mcal_pclbuz.c
 * Brief: クロック出力／ブザー出力ドライバ
 * Author: KimiakiK
 * Date: 2021/08/12
 */

/********** Include **********/

#include "typedef.h"
#include "iodefine.h"
#include "mcal_pclbuz.h"

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
void PlayBuzzer(frequency_id_t frequency_id)
{
    /* 周波数設定 */
    switch (frequency_id) {
    case FREQUENCY_ID_256HZ:
        CKS0 = 0x0F;
        break;
    case FREQUENCY_ID_512HZ:
        CKS0 = 0x0E;
        break;
    case FREQUENCY_ID_1024HZ:
        CKS0 = 0x0D;
        break;
    case FREQUENCY_ID_2048HZ:
        CKS0 = 0x0C;
        break;
    case FREQUENCY_ID_4096HZ:
        CKS0 = 0x0B;
        break;
    default:
        /* 処理なし */
        break;
    }

    /* 出力設定 */
    if (frequency_id < FREQUENCY_ID_NUM) {
        PCLOE0 = 1;
    }
}

void StopBuzzer(void)
{
    PCLOE0 = 0;
}

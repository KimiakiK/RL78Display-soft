/*
 * File: mcal_pclbuz.h
 * Brief: クロック出力／ブザー出力ドライバ
 * Author: KimiakiK
 * Date: 2021/08/12
 */

#ifndef MCAL_PCLBUZ_H_
#define MCAL_PCLBUZ_H_

/********** Include **********/

#include "typedef.h"

/********** Define **********/

/********** Enum **********/

typedef enum frequency_id_enum {
    FREQUENCY_ID_256HZ = 0,
    FREQUENCY_ID_512HZ,
    FREQUENCY_ID_1024HZ,
    FREQUENCY_ID_2048HZ,
    FREQUENCY_ID_4096HZ,
    FREQUENCY_ID_NUM
} frequency_id_t;

/********** Type **********/

/********** Constant **********/

/********** Variable **********/

/********** Function Prototype **********/

void PlayBuzzer(frequency_id_t frequency_id);
void StopBuzzer(void);

#endif /* MCAL_PCLBUZ_H_ */

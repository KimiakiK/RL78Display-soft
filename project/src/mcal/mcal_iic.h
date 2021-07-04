/*
 * File: mcal_iic.h
 * Brief: IICドライバ
 * Author: KimiakiK
 * Date: 2021/07/04
 */

#ifndef MCAL_IIC_H_
#define MCAL_IIC_H_

/********** Include **********/

#include "typedef.h"

/********** Define **********/

#define IIC_SYNC    (0)
#define IIC_ASYNC   (1)


#define IIC_STATE_IDLE      (0)
#define IIC_STATE_ADDRESS   (1)
#define IIC_STATE_SEND      (2)
#define IIC_STATE_RECEIVE   (3)

/********** Enum **********/

/********** Type **********/

/********** Constant **********/

/********** Variable **********/

/********** Function Prototype **********/

void InitIic(void);
uint8_t GetIicState(void);
void SendIic(uint8_t address, uint8_t* data, uint8_t length, uint8_t type);
void ReceiveIic(uint8_t address, uint8_t* s_data, uint8_t s_length, uint8_t* r_data, uint8_t r_length, uint8_t type);

#endif /* MCAL_IIC_H_ */

/*
 * File: mcal_spi.c
 * Brief: SPIドライバ
 * Author: KimiakiK
 * Date: 2021/07/04
 */

/********** Include **********/

#include "typedef.h"
#include "iodefine.h"
#include "mcal_spi.h"

/********** Define **********/

/* SPI動作状態 */
#define SPI_IDLE        (0)
#define SPI_SEND        (1)

/* SMRレジスタ用定義 */
#define SMR_CSI_CK00_INTR_FINISH    (0x0020)
#define SMR_CSI_CK00_INTR_EMPTY     (0x0021)

/********** Enum **********/

/********** Type **********/

/********** Constant **********/

/********** Variable **********/

static uint8_t spi_state;
static uint32_t send_data_address;
static uint32_t send_length;

/********** Function Prototype **********/

void sendSpi(uint32_t data_address, uint32_t length);

/********** Function **********/

/*
 * Function: 
 * Argument: 
 * Return: 
 * Note: 
 */
void InitSpi(void)
{
    /* 変数初期化 */
    spi_state = SPI_IDLE;
    send_data_address = 0;
    send_length = 0;

    /* SFR初期化 */
    SAU0EN = 1;         /* クロック供給許可 */
    SPS0 = 0x0000;      /* CK00, CK01: 32MHz */
    SMR00 = SMR_CSI_CK00_INTR_FINISH;
    SCR00 = 0x8007;     /* タイプ1、MBSファースト、8ビット */
    SDR00 = 0x0000;     /* ボーレート: 32/2 MHz */
    SOL0 = 0x0000;      /* 非反転（通常）送信 */
    SO0 = 0x0F0E;       /* 初期出力レベル */
    SOE0 = 0x0001;      /* 出力許可 */
    P1 |= 0x05;         /* SCL00とSDA00端子設定 */
    PM1 &= 0xFA;        /* SCL00とSDA00端子設定 */
    CSIIF00 = 0;        /* 割り込みフラグクリア */
    CSIMK00 = 0;        /* 割り込みマスク解除 */
    SS0 = 0x0001;       /* 動作開始 */
}

/*
 * Function: 
 * Argument: 
 * Return: 
 * Note: 
 */
void SendSyncSpi(uint32_t data_address, uint32_t length)
{
    sendSpi(data_address, length);

    while (spi_state == SPI_SEND) { }
}

/*
 * Function: 
 * Argument: 
 * Return: 
 * Note: 
 */
void SendAsyncSpi(uint32_t data_address, uint32_t length)
{
    sendSpi(data_address, length);
}

#pragma interrupt IntrSpi(vect=INTCSI00)
/*
 * Function: 
 * Argument: 
 * Return: 
 * Note: 
 */
void IntrSpi(void)
{
    send_length --;
    send_data_address ++;

    if (send_length == 0) {
        /* 全データ送信完了 */
        spi_state = SPI_IDLE;
    } else {
        if (send_length == 1) {
            /* 送信データが最後の１つの場合は転送完了割り込み */
            SMR00 = SMR_CSI_CK00_INTR_FINISH;
        }
        /* 次の送信データ設定 */
         SIO00 = *(uint8_t __far *)send_data_address;
    }
}

/*
 * Function: 
 * Argument: 
 * Return: 
 * Note: 
 */
void sendSpi(uint32_t data_address, uint32_t length)
{
    spi_state = SPI_SEND;
    send_data_address = data_address;
    send_length = length;

    if (send_length > 1) {
        /* 送信データが複数ある場合はバッファ空き割り込み */
        SMR00 = SMR_CSI_CK00_INTR_EMPTY;
    } else {
        /* 送信データが１つの場合は転送完了割り込み */
        SMR00 = SMR_CSI_CK00_INTR_FINISH;
    }

    /* データ送信開始 */
    SIO00 = *(uint8_t __far *)send_data_address;
}
/*
 * File: drv_tft.c
 * Brief: TFT(ST7789)ドライバ
 * Author: KimiakiK
 * Date: 2021/07/04
 */

/********** Include **********/

#include "typedef.h"
#include "mcal_port.h"
#include "mcal_spi.h"
#include "mcal_timer.h"
#include "drv_light.h"
#include "drv_tft.h"

/********** Define **********/

#define PIN_RESET_OFF   (LEVEL_HIGH)
#define PIN_RESET_ON    (LEVEL_LOW)
#define PIN_CS_OFF      (LEVEL_HIGH)
#define PIN_CS_ON       (LEVEL_LOW)
#define PIN_DC_DATA     (LEVEL_HIGH)
#define PIN_DC_COMMAND  (LEVEL_LOW)

/********** Enum **********/

/********** Type **********/

/********** Constant **********/

// const static uint8_t command_SWRESET[] = {0x01};        /* SWRESET (01h): Software Reset */
const static uint8_t command_SLPOUT[] = {0x11};         /* SLPOUT (11h): Sleep Out */
const static uint8_t command_COLMOD[] = {0x3A};         /* COLMOD (3Ah): Interface Pixel Format */
const static uint8_t data_COLMOD[] = {0x55};            /* 65K of RGB interface, 16bit/pixel */
const static uint8_t command_MADCTL[] = {0x36};         /* MADCTL (36h): Memory Data Access Control */
const static uint8_t data_MADCTL[] = {0x00};            /* Page Address Order: Top to Bottom, Column Address Order: Left to Right */
// const static uint8_t command_INVOFF[] = {0x20};         /* INVOFF (20h): Display Inversion Off (0x0000:WHITE, 0xFFFF:BLACK */
const static uint8_t command_INVON[] = {0x21};          /* INVON (21h): Display Inversion On (0x0000:BLACK, 0xFFFF:WHITE) */
const static uint8_t command_NORON[] = {0x13};          /* NORON (13h): Normal Display Mode On */
const static uint8_t command_DISPOFF[] = {0x28};         /* DISPOFF (28h): Display Off */
const static uint8_t command_DISPON[] = {0x29};         /* DISPON (29h): Display On */
const static uint8_t command_CASET[] = {0x2A};          /* CASET (2Ah): Column Address Set */
const static uint8_t command_RASET[] = {0x2B};          /* RASET (2Bh): Row Address Set */
const static uint8_t command_RAMWR[] = {0x2C};          /* RAMWR (2Ch): Memory Write */

/********** Variable **********/

static uint8_t data_area[4];

/********** Function Prototype **********/

void sendCommand(uint32_t data_address, uint32_t length);
void sendData(uint32_t data_address, uint32_t length);
void setArea(uint8_t x, uint8_t y, uint8_t w, uint8_t h);

/********** Function **********/

/*
 * Function: TFT初期化
 * Argument: 無し
 * Return: 無し
 * Note: TFTのIC ST7789を初期化
 */
void InitTft(void)
{
    /* ハードウェアリセット */
    WritePin(PIN_ID_TFT_RESET, PIN_RESET_ON);
    WaitMs(1);
    WritePin(PIN_ID_TFT_RESET, PIN_RESET_OFF);
    WaitMs(120);

    /* 初期設定 */
    WritePin(PIN_ID_TFT_CS, PIN_CS_ON);
    sendCommand((uint32_t)((uint8_t __far *)command_SLPOUT), sizeof(command_SLPOUT));
    WaitMs(5);
    sendCommand((uint32_t)((uint8_t __far *)command_COLMOD), sizeof(command_COLMOD));
    sendData((uint32_t)((uint8_t __far *)data_COLMOD), sizeof(data_COLMOD));
    sendCommand((uint32_t)((uint8_t __far *)command_MADCTL), sizeof(command_MADCTL));
    sendData((uint32_t)((uint8_t __far *)data_MADCTL), sizeof(data_MADCTL));
    // sendCommand((uint32_t)((uint8_t __far *)command_INVOFF), sizeof(command_INVOFF));
    sendCommand((uint32_t)((uint8_t __far *)command_INVON), sizeof(command_INVON));
    sendCommand((uint32_t)((uint8_t __far *)command_NORON), sizeof(command_NORON));
}


/*
 * Function: 画像描画
 * Argument: 横位置、縦位置、横幅、縦幅、画像データの先頭アドレス
 * Return: 無し
 * Note: 指定画像をTFTに表示
 */
void DrawTft(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint32_t data)
{
    setArea(x, y, w, h);
    sendCommand((uint32_t)((uint8_t __far *)command_RAMWR), sizeof(command_RAMWR));
    sendData(data, (uint32_t)w * h * 2);
}

/*
 * Function: TFT表示クリア
 * Argument: 無し
 * Return: 無し
 * Note: TFT表示を黒色でクリア
 */
void TftClear(void)
{
    setArea(0, 0, TFT_WIDTH, TFT_HEIGHT);
    sendCommand((uint32_t)((uint8_t __far *)command_RAMWR), sizeof(command_RAMWR));
    WritePin(PIN_ID_TFT_DC, PIN_DC_DATA);
    SendFixDataSyncSpi(0x00,  (uint32_t)TFT_WIDTH * TFT_HEIGHT * 2);
}

/*
 * Function: TFT表示開始
 * Argument: 無し
 * Return: 無し
 * Note: TFT表示を開始してバックライトを点灯
 */
void TftOn(void)
{
    /* 表示開始 */
    sendCommand((uint32_t)((uint8_t __far *)command_DISPON), sizeof(command_DISPON));
    LightOn();
}

/*
 * Function: TFT表示終了
 * Argument: 無し
 * Return: 無し
 * Note: バックライトを消灯してTFT表示を終了
 */
void TftOff(void)
{
    /* 表示終了 */
    LightOff();
    sendCommand((uint32_t)((uint8_t __far *)command_DISPOFF), sizeof(command_DISPOFF));
}

/*
 * Function: TFT操作コマンド送信
 * Argument: コマンド先頭アドレス、送信コマンド長
 * Return: 無し
 * Note: 指定コマンドを送信
 */
void sendCommand(uint32_t data_address, uint32_t length)
{
    WritePin(PIN_ID_TFT_DC, PIN_DC_COMMAND);
    SendSyncSpi(data_address, length);
}

/*
 * Function: TFT操作データ送信
 * Argument: データ先頭アドレス、送信データ長
 * Return: 無し
 * Note: 指定データを送信
 */
void sendData(uint32_t data_address, uint32_t length)
{
    WritePin(PIN_ID_TFT_DC, PIN_DC_DATA);
    SendSyncSpi(data_address, length);
}

/*
 * Function: 描画領域設定
 * Argument: 横位置、縦位置、横幅、縦幅
 * Return: 無し
 * Note: 指定領域を描画領域としてTFTに指示
 */
void setArea(uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
    sendCommand((uint32_t)((uint8_t __far *)command_CASET), sizeof(command_CASET));
    data_area[0] = 0x00;
    data_area[1] = x;
    data_area[2] = 0x00;
    data_area[3] = x + w - 1;
    sendData((uint32_t)((uint8_t __far *)data_area), sizeof(data_area));

    sendCommand((uint32_t)((uint8_t __far *)command_RASET), sizeof(command_RASET));
    data_area[0] = 0x00;
    data_area[1] = y;
    data_area[2] = 0x00;
    data_area[3] = y + h - 1;
    sendData((uint32_t)((uint8_t __far *)data_area), sizeof(data_area));
}
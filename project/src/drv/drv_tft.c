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

#define DRAW_JOB_MAX    (256)

#define SPECIAL_DATA_DRAW_BLACK     (0)
#define SPECIAL_DATA_TFT_OFF        (1)
#define SPECIAL_DATA_TFT_ON         (2)

#define PIN_RESET_OFF   (LEVEL_HIGH)
#define PIN_RESET_ON    (LEVEL_LOW)
#define PIN_CS_OFF      (LEVEL_HIGH)
#define PIN_CS_ON       (LEVEL_LOW)
#define PIN_DC_DATA     (LEVEL_HIGH)
#define PIN_DC_COMMAND  (LEVEL_LOW)

/********** Enum **********/

typedef enum {
    DRAW_JOB_IDLE = 0,
    DRAW_JOB_SEND_COMMAND_CASET,
    DRAW_JOB_SEND_CASET_DATA,
    DRAW_JOB_SEND_COMMAND_RASET,
    DRAW_JOB_SEND_RASET_DATA,
    DRAW_JOB_SEND_COMMAND_RAMWR,
    DRAW_JOB_SEND_DATA,
    DRAW_JOB_SPECIAL_DATA,
    DRAW_JOB_END_WAIT
} draw_job_state_t;

/********** Type **********/

typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t w;
    uint8_t h;
    uint32_t data;
} draw_job_t;

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
static draw_job_state_t draw_job_state;
static draw_job_t draw_job_queue[DRAW_JOB_MAX];
static uint16_t draw_job_index_first;
static uint16_t draw_job_index_end;

/********** Function Prototype **********/

void sendCommand(uint32_t data_address, uint32_t length, send_mode_t mode);
void sendData(uint32_t data_address, uint32_t length, send_mode_t mode);
void setArea(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
void addDrawJob(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint32_t data);
void startDrawJob(void);
void processDrawJob(void);
void endDrawJob(void);

/********** Function **********/

/*
 * Function: TFT初期化
 * Argument: 無し
 * Return: 無し
 * Note: TFTのIC ST7789を初期化
 */
void InitTft(void)
{
    /* 変数初期化 */
    draw_job_state = DRAW_JOB_IDLE;
    draw_job_index_first = 0;
    draw_job_index_end = 0;

    /* ハードウェアリセット */
    WritePin(PIN_ID_TFT_RESET, PIN_RESET_ON);
    WaitMs(1);
    WritePin(PIN_ID_TFT_RESET, PIN_RESET_OFF);
    WaitMs(120);

    /* 初期設定 */
    WritePin(PIN_ID_TFT_CS, PIN_CS_ON);
    sendCommand((uint32_t)((uint8_t __far *)command_SLPOUT), sizeof(command_SLPOUT), SEND_SYNC);
    WaitMs(5);
    sendCommand((uint32_t)((uint8_t __far *)command_COLMOD), sizeof(command_COLMOD), SEND_SYNC);
    sendData((uint32_t)((uint8_t __far *)data_COLMOD), sizeof(data_COLMOD), SEND_SYNC);
    sendCommand((uint32_t)((uint8_t __far *)command_MADCTL), sizeof(command_MADCTL), SEND_SYNC);
    sendData((uint32_t)((uint8_t __far *)data_MADCTL), sizeof(data_MADCTL), SEND_SYNC);
    // sendCommand((uint32_t)((uint8_t __far *)command_INVOFF), sizeof(command_INVOFF), SEND_SYNC);
    sendCommand((uint32_t)((uint8_t __far *)command_INVON), sizeof(command_INVON), SEND_SYNC);
    sendCommand((uint32_t)((uint8_t __far *)command_NORON), sizeof(command_NORON), SEND_SYNC);
}


/*
 * Function: 画像描画
 * Argument: 横位置、縦位置、横幅、縦幅、画像データの先頭アドレス
 * Return: 無し
 * Note: 指定画像をTFTに表示
 */
void DrawTft(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint32_t data)
{
    addDrawJob(x, y, w, h, data);
}

/*
 * Function: TFT表示クリア
 * Argument: 無し
 * Return: 無し
 * Note: TFT表示を黒色でクリア
 */
void TftClear(void)
{
    addDrawJob(0, 0, TFT_WIDTH, TFT_HEIGHT, SPECIAL_DATA_DRAW_BLACK);
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
    addDrawJob(0, 0, 0, 0, SPECIAL_DATA_TFT_ON);
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
    addDrawJob(0, 0, 0, 0, SPECIAL_DATA_TFT_OFF);
}

/*
 * Function: TFT操作コマンド送信
 * Argument: コマンド先頭アドレス、送信コマンド長、送信モード(同期/非同期)
 * Return: 無し
 * Note: 指定コマンドを送信
 */
void sendCommand(uint32_t data_address, uint32_t length, send_mode_t mode)
{
    WritePin(PIN_ID_TFT_DC, PIN_DC_COMMAND);
    SendDataSpi(data_address, length, mode);
}

/*
 * Function: TFT操作データ送信
 * Argument: データ先頭アドレス、送信データ長、送信モード(同期/非同期)
 * Return: 無し
 * Note: 指定データを送信
 */
void sendData(uint32_t data_address, uint32_t length, send_mode_t mode)
{
    WritePin(PIN_ID_TFT_DC, PIN_DC_DATA);
    SendDataSpi(data_address, length, mode);
}

/*
 * Function: 描画領域設定
 * Argument: 横位置、縦位置、横幅、縦幅
 * Return: 無し
 * Note: 指定領域を描画領域としてTFTに指示
 */
void setArea(uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
    sendCommand((uint32_t)((uint8_t __far *)command_CASET), sizeof(command_CASET), SEND_SYNC);
    data_area[0] = 0x00;
    data_area[1] = x;
    data_area[2] = 0x00;
    data_area[3] = x + w - 1;
    sendData((uint32_t)((uint8_t __far *)data_area), sizeof(data_area), SEND_SYNC);

    sendCommand((uint32_t)((uint8_t __far *)command_RASET), sizeof(command_RASET), SEND_SYNC);
    data_area[0] = 0x00;
    data_area[1] = y;
    data_area[2] = 0x00;
    data_area[3] = y + h - 1;
    sendData((uint32_t)((uint8_t __far *)data_area), sizeof(data_area), SEND_SYNC);
}

/*
 * Function: 描画ジョブ追加
 * Argument: 横位置、縦位置、横幅、縦幅、データ
 * Return: 無し
 * Note: 描画ジョブに追加して、処理中ジョブが無い場合は処理開始
 */
void addDrawJob(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint32_t data)
{
    draw_job_queue[draw_job_index_first].x = x;
    draw_job_queue[draw_job_index_first].y = y;
    draw_job_queue[draw_job_index_first].w = w;
    draw_job_queue[draw_job_index_first].h = h;
    draw_job_queue[draw_job_index_first].data = data;
    draw_job_index_first ++;

    if (draw_job_index_first >= DRAW_JOB_MAX) {
        draw_job_index_first = 0;
    }

    if (draw_job_state == DRAW_JOB_IDLE) {
        startDrawJob();
    }
}

/*
 * Function: 描画ジョブ処理開始
 * Argument: 無し
 * Return: 無し
 * Note: 描画ジョブ処理状態を変更して処理実行
 */
void startDrawJob(void)
{
    draw_job_t* job = &draw_job_queue[draw_job_index_end];

    if ((job->data == SPECIAL_DATA_TFT_OFF)
     || (job->data == SPECIAL_DATA_TFT_ON)) {
        draw_job_state = DRAW_JOB_SPECIAL_DATA;
    } else {
        draw_job_state = DRAW_JOB_SEND_COMMAND_CASET;
    }

    processDrawJob();
}

/*
 * Function: 描画ジョブ処理実行
 * Argument: 無し
 * Return: 無し
 * Note: 描画ジョブ処理状態に応じた送信処理
 */
void processDrawJob(void)
{
    draw_job_t* job = &draw_job_queue[draw_job_index_end];
    switch (draw_job_state)
    {
    case DRAW_JOB_IDLE:
        /* 処理無し */
        break;
    case DRAW_JOB_SEND_COMMAND_CASET:
        draw_job_state = DRAW_JOB_SEND_CASET_DATA;
        sendCommand((uint32_t)((uint8_t __far *)command_CASET), sizeof(command_CASET), SEND_ASYNC);
        break;
    case DRAW_JOB_SEND_CASET_DATA:
        draw_job_state = DRAW_JOB_SEND_COMMAND_RASET;
        data_area[0] = 0x00;
        data_area[1] = job->x;
        data_area[2] = 0x00;
        data_area[3] = job->x + job->w - 1;
        sendData((uint32_t)((uint8_t __far *)data_area), sizeof(data_area), SEND_ASYNC);
        break;
    case DRAW_JOB_SEND_COMMAND_RASET:
        draw_job_state = DRAW_JOB_SEND_RASET_DATA;
        sendCommand((uint32_t)((uint8_t __far *)command_RASET), sizeof(command_RASET), SEND_ASYNC);
        break;
    case DRAW_JOB_SEND_RASET_DATA:
        draw_job_state = DRAW_JOB_SEND_COMMAND_RAMWR;
        data_area[0] = 0x00;
        data_area[1] = job->y;
        data_area[2] = 0x00;
        data_area[3] = job->y + job->h - 1;
        sendData((uint32_t)((uint8_t __far *)data_area), sizeof(data_area), SEND_ASYNC);
        break;
    case DRAW_JOB_SEND_COMMAND_RAMWR:
        draw_job_state = DRAW_JOB_SEND_DATA;
        sendCommand((uint32_t)((uint8_t __far *)command_RAMWR), sizeof(command_RAMWR), SEND_ASYNC);
        break;
    case DRAW_JOB_SEND_DATA:
        draw_job_state = DRAW_JOB_END_WAIT;
        /* 送信データが特別値の時は固定値送信、それ以外はアドレス送信 */
        if (job->data == SPECIAL_DATA_DRAW_BLACK) {
            WritePin(PIN_ID_TFT_DC, PIN_DC_DATA);
            SendFixDataSyncSpi((uint8_t)job->data, (uint32_t)job->w * job->h * 2, SEND_ASYNC);
        } else {
            sendData(job->data, (uint32_t)job->w * job->h * 2, SEND_ASYNC);
        }
        break;
    case DRAW_JOB_SPECIAL_DATA:
        draw_job_state = DRAW_JOB_END_WAIT;
        if (job->data == SPECIAL_DATA_TFT_ON) {
            /* TFT表示開始 */
            sendCommand((uint32_t)((uint8_t __far *)command_DISPON), sizeof(command_DISPON), SEND_ASYNC);
        } else {
            /* TFT表示終了 */
            sendCommand((uint32_t)((uint8_t __far *)command_DISPOFF), sizeof(command_DISPOFF), SEND_ASYNC);
        }
        break;
    case DRAW_JOB_END_WAIT:
        /* 処理無し */
        break;
    default:
        /* 処理無し */
        break;
    }
}

/*
 * Function: 描画ジョブ処理終了
 * Argument: 無し
 * Return: 無し
 * Note: 描画ジョブキューがある場合は次を送信
 */
void endDrawJob(void)
{
    draw_job_index_end ++;

    if (draw_job_index_end >= DRAW_JOB_MAX) {
        draw_job_index_end = 0;
    }

    if (draw_job_index_end == draw_job_index_first) {
        /* キュー処理完了 */
        draw_job_state = DRAW_JOB_IDLE;
    } else {
        /* キュー処理 */
        startDrawJob();
    }
}

/*
 * Function: SPIデータ送信完了コールバック
 * Argument: 無し
 * Return: 無し
 * Note: 割り込み処理内から呼び出し
 */
void EndCallbackSpi(void)
{
    if (draw_job_state == DRAW_JOB_END_WAIT) {
        endDrawJob();
    } else {
        processDrawJob();
    }
}

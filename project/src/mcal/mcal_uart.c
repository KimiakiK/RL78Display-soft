/*
 * File: mcal_uart.c
 * Brief: UARTドライバ
 * Author: KimiakiK
 * Date: 2022/12/13
 */

/********** Include **********/

#include "typedef.h"
#include "iodefine.h"
#include "mcal_uart.h"

/********** Define **********/

/* SMRレジスタ用定義 */
#define SMR_UART_CK01_INTR_FINISH    (0x8022)
#define SMR_UART_CK01_INTR_EMPTY     (0x8023)

/********** Enum **********/

/* 送信処理状態 */
typedef enum {
	TX_STATE_IDLE = 0,
	TX_STATE_BUSY
} tx_state_t;

/********** Type **********/

/********** Constant **********/

/********** Variable **********/

static tx_state_t tx_state;
static uint8_t tx_buffer[TX_LENGTH_MAX];
static uint8_t rx_buffer[RX_LENGTH_MAX];
static uint16_t tx_buffer_length;
static uint16_t tx_buffer_front;
static uint16_t rx_buffer_front;
static uint16_t rx_buffer_end;

/********** Function Prototype **********/

static void transmit(uint16_t index);

/********** Function **********/

/*
 * Function: UART初期化
 * Argument: 無し
 * Return: 無し
 * Note: UART初期化
 */
void InitUart(void)
{
	uint16_t index;

	/* 変数初期化 */
	tx_state = TX_STATE_IDLE;
	for (index=0; index<TX_LENGTH_MAX; index++) {
		tx_buffer[index] = 0;
	}
	for (index=0; index<RX_LENGTH_MAX; index++) {
		rx_buffer[index] = 0;
	}
	tx_buffer_length = 0;
	tx_buffer_front = 0;
	rx_buffer_front = 0;
	rx_buffer_end = 0;

	/* SFR初期化 */
	SAU0EN = 1;         /* クロック供給許可 */
	SPS0 = 0x0010;      /* CK00: 32MHz, CK01: 16MHz */
	/* 送信設定(TxD1) */
	SMR02 = SMR_UART_CK01_INTR_FINISH;
	SCR02 = 0x8097;     /* 送信、パリティなし、LSBファースト、ストップビット1bit、8bitデータ */
	SDR02 = 0x8800;     /* 16MHz / (68 + 1) / 2 = 115942bps */
	/* 受信設定(RxD1) */
	SIR03 = 0x0007;     /* フラグクリア */
	SMR03 = 0x8122;     /* UARTモード、CK01、非反転通信、転送完了割り込み */
	SCR03 = 0x4097;     /* 受信、パリティなし、LSBファースト、ストップビット1bit、8bitデータ */
	SDR03 = 0x8800;     /* 16MHz / (68 + 1) / 2 = 115942bps */
	NFEN0 = 0x04;       /* RxD1端子のノイズフィルタON */
	/* ポート設定 */
	PMC0 &= 0xF3;       /* TxD1端子(P02)とRxD1端子(P03)：デジタル入出力 */
	POM0 |= 0x04;       /* TxD1端子(P02)：N-chオープンドレイン出力 */
	PIM0 |= 0x08;       /* RxD1端子(P03)：TTL入力バッファ */
	P0   |= 0x04;       /* TxD1端子(P02)：HIGHレベル */
	PM0  &= 0xFB;       /* TxD1端子(P02)：出力 */
	PM0  |= 0x08;       /* RxD1端子(P03)：入力 */
	/* 通信開始 */
	SO0  |= 0x0004;     /* TxD1端子(P02)：シリアル・データ出力値 "1" */
	SOE0 |= 0x0004;     /* TxD1端子(P02)：シリアル出力許可 */
	SS0  |= 0x000C;     /* TxD1とRxD1：通信開始 */
	/* 受信割り込み許可 */
	SRIF1 = 0;
	SRMK1 = 0;
}

/*
 * Function: UART非同期送信
 * Argument: 送信データ格納先、送信データ長
 * Return: 送信処理結果
 * Note: 送信データを内部バッファにコピー後、割り込みを使用して連続送信
 */
uart_result_t SendAsyncUart(uint8_t* data, uint16_t length)
{
	uart_result_t result;
	uint16_t index;
	
	if (length < TX_LENGTH_MAX) {
		if (tx_state == TX_STATE_IDLE) {
			/* 送信データを内部バッファにコピー */
			for (index=0; index<length; index++) {
				tx_buffer[index] = data[index];
			}
			tx_buffer_length = length;
			/* 送信割り込み許可 */
			STIF1 = 0;
			STMK1 = 0;
			/* データ送信を開始 */
			tx_state = TX_STATE_BUSY;
			tx_buffer_front = 0;
			transmit(tx_buffer_front);
		} else {
			result = UART_RESULT_BUSY;
		}
	} else {
		result = UART_RESULT_ERROR;
	}

	return result;
}

/*
 * Function: UART受信データ取得
 * Argument: 受信データ格納先
 * Return: 受信データ取得結果
 * Note: 受信リングバッファの最後尾データを渡す
 */
uart_result_t GetReceiveUart(uint8_t* data)
{
	uart_result_t result;

	if (rx_buffer_end != rx_buffer_front) {
		/* 受信リングバッファの最後尾データをコピー */
		*data = rx_buffer[rx_buffer_end];
		/* リングバッファ最後尾インデックス移動 */
		rx_buffer_end ++;
		if (rx_buffer_end >= RX_LENGTH_MAX) {
			rx_buffer_end = 0;
		}
		result = UART_RESULT_OK;
	} else {
		/* 受信リングバッファが空 */
		result = UART_RESULT_EMPTY;
	}

	return result;
}

#pragma interrupt IntrUartTx(vect=INTST1)
/*
 * Function: UART送信割り込み処理
 * Argument: 無し
 * Return: 無し
 * Note: データの連続送信処理
 */
void IntrUartTx(void)
{
	if (SMR02 == SMR_UART_CK01_INTR_EMPTY) {
		/* 次データ送信 */
		transmit(tx_buffer_front);
	} else {
		/* 全データ送信完了 */
		tx_state = TX_STATE_IDLE;
		/* 送信割り込み禁止 */
		STMK1 = 1;
	}
}

#pragma interrupt IntrUartRx(vect=INTSR1)
/*
 * Function: UART受信割り込み処理
 * Argument: 無し
 * Return: 無し
 * Note: 受信データのリングバッファ格納処理
 */
void IntrUartRx(void)
{
	volatile uint8_t rx_data;

	SIR03 = 0x0007;
	/* 受信データ取得 */
	rx_data = RXD1;
	/* 受信データをバッファへ格納 */
	rx_buffer[rx_buffer_front] = rx_data;
	/* リングバッファ先頭インデックス移動 */
	rx_buffer_front ++;
	if (rx_buffer_front >= RX_LENGTH_MAX) {
		rx_buffer_front = 0;
	}
}

/*
 * Function: 送信処理
 * Argument: 送信対象の送信バッファ要素番号
 * Return: 無し
 * Note: 次の割り込み要因を設定して指定のデータを送信
 */
static void transmit(uint16_t index)
{
	tx_buffer_front = index + 1;

	if (tx_buffer_front < tx_buffer_length) {
		/* 次送信データあり：バッファ空き割り込みを設定 */
		SMR02 = SMR_UART_CK01_INTR_EMPTY;
	} else {
		/* 次送信データなし：転送完了割り込みを設定 */
		SMR02 = SMR_UART_CK01_INTR_FINISH;
	}

	/* データ送信 */
	STMK1 = 1;
	TXD1 = tx_buffer[index];
	STMK1 = 0;
}
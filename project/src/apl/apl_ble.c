/*
 * File: apl_ble.c
 * Brief: BLE通信アプリ
 * Author: KimiakiK
 * Date: 2023/01/08
 */

/********** Include **********/

#include "typedef.h"
#include "apl_ble.h"
#include "drv_bm71.h"
#include "drv_sw.h"
#include "lib_font.h"

/********** Define **********/

/********** Enum **********/

typedef enum {
	BLE_STATE_INIT = 0,
	BLE_STATE_RESET,
	BLE_STATE_IDLE,
	BLE_STATE_SCAN,
	BLE_STATE_CONNECTING,
	BLE_STATE_CONNECTED
} ble_state_t;

/********** Type **********/

/********** Constant **********/

/********** Variable **********/

static ble_state_t ble_state;

static uint8_t text_pos_x;
static uint8_t text_pos_y;

/********** Function Prototype **********/

void transitionBleState(void);

/********** Function **********/

/*
 * Function: BLE初期化
 * Argument: なし
 * Return  : なし
 * Note    : なし
 */
void InitBle(void)
{
	ble_state = BLE_STATE_INIT;

	text_pos_x = 0;
	text_pos_y = 0;
}

/*
 * Function: BLE周期処理
 * Argument: なし
 * Return  : なし
 * Note    : なし
 */
void MainBle(void)
{
	transitionBleState();
}

/*
 * Function: BLE通信表示初期化
 * Argument: なし
 * Return  : なし
 * Note    : なし
 */
void InitDrawBle(void)
{
	text_pos_x = 0;
	text_pos_y = 0;
}

/*
 * Function: BLE通信表示
 * Argument: なし
 * Return  : なし
 * Note    : なし
 */
void DrawBle(void)
{
	bm71_result_t result;
	uint8_t data;

	result = GetTransparentReceiveData(&data);

	if (result == BM71_RESULT_OK) {
		if (data == '\n') {
			text_pos_x = 0;
			text_pos_y++;
		} else {
			DrawAscii(data, text_pos_x * 10, text_pos_y * 20);
			text_pos_x++;
		}
		if (text_pos_x >= 24) {
			text_pos_x = 0;
			text_pos_y++;
		}
		if (text_pos_y >= 12) {
			text_pos_y = 0;
		}
	}

	if (ble_state == BLE_STATE_CONNECTED) {
		uint8_t text[SW_ID_NUM];
		uint8_t length = 0;
		if (GetSw(SW_ID_A) == SW_PUSH) { text[length++] = 'A'; }
		if (GetSw(SW_ID_B) == SW_PUSH) { text[length++] = 'B'; }
		if (GetSw(SW_ID_C) == SW_PUSH) { text[length++] = 'C'; }
		if (GetSw(SW_ID_D) == SW_PUSH) { text[length++] = 'D'; }
		if (GetSw(SW_ID_POS_U) == SW_PUSH) { text[length++] = 'u'; }
		if (GetSw(SW_ID_POS_D) == SW_PUSH) { text[length++] = 'd'; }
		if (GetSw(SW_ID_POS_L) == SW_PUSH) { text[length++] = 'l'; }
		if (GetSw(SW_ID_POS_R) == SW_PUSH) { text[length++] = 'r'; }
		if (length > 0) {
			SetTransparentSendData(text, length);
			SetBM71Request(BM71_REQUEST_TRANSPARENT_UART_SERVICE_SEND_DATA);
		}
	}
}

/*
 * Function: BLE状態遷移
 * Argument: なし
 * Return  : なし
 * Note    : なし
 */
void transitionBleState(void)
{
	bm71_status_t bm71_status;

	bm71_status = GetBM71Status();

	switch (ble_state) {
	case BLE_STATE_INIT:
		ble_state = BLE_STATE_RESET;
		SetBM71Request(BM71_REQUEST_RESET);
		break;
	case BLE_STATE_RESET:
		ble_state = BLE_STATE_IDLE;
		SetBM71Request(BM71_REQUEST_READ_STATUS);
		break;
	case BLE_STATE_IDLE:
		if (bm71_status == BM71_STATUS_IDLE_MODE) {
			ble_state = BLE_STATE_SCAN;
			SetBM71Request(BM71_REQUEST_SET_SCAN_ENABLE);
		} else if (bm71_status == BM71_STATUS_PENDING) {
			SetBM71Request(BM71_REQUEST_READ_STATUS);
		}
		break;
	case BLE_STATE_SCAN:
		if (bm71_status == BM71_STATUS_BLUETOOTH_LOW_ENERGY_CONNECTED_MODE) {
			ble_state = BLE_STATE_CONNECTING;
			SetBM71Request(BM71_REQUEST_ENABLE_TRANSPARENT_UART_SERVICE);
		} else {
			SetBM71Request(BM71_REQUEST_LE_CREATE_CONNECTION);
		}
		break;
	case BLE_STATE_CONNECTING:
		if (bm71_status == BM71_STATUS_TRANSPARENT_SERVICE_ENABLED_MODE) {
			ble_state = BLE_STATE_CONNECTED;
			SetTransparentSendData((uint8_t*)"CONNECTED\r\n", 11);
			SetBM71Request(BM71_REQUEST_TRANSPARENT_UART_SERVICE_SEND_DATA);
		}
		break;
	case BLE_STATE_CONNECTED:
		break;
	default:
		/* 処理なし */
		break;
	}
}

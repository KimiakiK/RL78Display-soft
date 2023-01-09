/*
 * File: drv_bm71.c
 * Brief: BM71ドライバ
 * Author: KimiakiK
 * Date: 2023/01/08
 */

/********** Include **********/

#include "typedef.h"
#include "drv_bm71.h"
#include "mcal_uart.h"

/********** Define **********/

/* 受信データバッファサイズ */
#define RECEIVE_BUFFER_SIZE		(64)
/* 送信データバッファサイズ */
#define SEND_BUFFER_SIZE		(64)

/* Transparent通信受信データバッファサイズ */
#define TRANSPARENT_RECEIVE_BUFFER_SIZE		(32)
/* Transparent通信送信データバッファサイズ */
#define TRANSPARENT_SEND_BUFFER_SIZE		(32)

/* デバイスアドレスのサイズ */
#define DEVICE_ADDRESS_SIZE		(6)

/********** Enum **********/

typedef enum {
	DEVICE_NOT_DETECT = 0,
	DEVICE_DETECTED
} device_detect_t;

/********** Type **********/

/********** Constant **********/

/********** Variable **********/

static bm71_status_t bm71_status;
static bm71_request_t bm71_request;

static uint8_t receive_buffer[RECEIVE_BUFFER_SIZE];
static uint16_t receive_buffer_front;
static uint8_t send_buffer[SEND_BUFFER_SIZE];

static uint8_t characteristic_value[CHARACTERISTIC_VALUE_LENGTH_MAX];
static uint8_t characteristic_value_length;
static update_t characteristic_value_update;

static device_detect_t device_detect_state;
static uint8_t device_address[DEVICE_ADDRESS_SIZE];

static uint8_t connection_handle;

static uint8_t transparent_send_data[TRANSPARENT_SEND_BUFFER_SIZE];
static uint16_t transparent_send_data_front;
static uint8_t transparent_receive_data[TRANSPARENT_RECEIVE_BUFFER_SIZE];
static uint16_t transparent_receive_data_front;
static uint16_t transparent_receive_data_end;

/********** Function Prototype **********/

static void processRequest(void);
static void makeMessage(uint16_t length);
static void processReceive(void);
static void parseMessage(void);
static uint8_t calculateChecksum(uint8_t* data, uint16_t length);
static void addTransparentReceiveData(uint8_t data);

/********** Function **********/

/*
 * Function: BM71初期化
 * Argument: なし
 * Return  : なし
 * Note    : なし
 */
void InitBM71(void)
{
	uint16_t index;

	bm71_status = BM71_STATUS_PENDING;
	bm71_request = BM71_REQUEST_NONE;

	for (index=0; index<RECEIVE_BUFFER_SIZE; index++) {
		receive_buffer[index] = 0;
	}
	receive_buffer_front = 0;
	for (index=0; index<SEND_BUFFER_SIZE; index++) {
		send_buffer[index] = 0;
	}

	for (index=0; index<CHARACTERISTIC_VALUE_LENGTH_MAX; index++) {
		characteristic_value[index] = 0;
	}
	characteristic_value_length = 0;
	characteristic_value_update = UPDATE_NOT;

	device_detect_state = DEVICE_NOT_DETECT;
	for (index=0; index<DEVICE_ADDRESS_SIZE; index++) {
		device_address[index] = 0;
	}

	connection_handle = 0;

	for (index=0; index<TRANSPARENT_SEND_BUFFER_SIZE; index++) {
		transparent_send_data[index] = 0;
	}
	transparent_send_data_front = 0;
	for (index=0; index<TRANSPARENT_RECEIVE_BUFFER_SIZE; index++) {
		transparent_receive_data[index] = 0;
	}
	transparent_receive_data_front = 0;
	transparent_receive_data_end = 0;
}

/*
 * Function: BM71周期処理
 * Argument: なし
 * Return  : なし
 * Note    : なし
 */
void MainBM71(void)
{
	characteristic_value_update = UPDATE_NOT;
	processRequest();
	processReceive();

}

/*
 * Function: BM71へのリクエスト設定
 * Argument: BM71へのリクエスト
 * Return  : なし
 * Note    : なし
 */
void SetBM71Request(bm71_request_t request)
{
	bm71_request = request;
}

/*
 * Function: BM71動作状態取得
 * Argument: なし
 * Return  : BM71動作状態
 * Note    : なし
 */
bm71_status_t GetBM71Status(void)
{
	return bm71_status;
}

/*
 * Function: Characteristic value取得
 * Argument: データ格納先、データ長格納先
 * Return  : データ更新有無
 * Note    : なし
 */
update_t GetCharacteristicValue(uint8_t*data, uint8_t* length)
{
	uint16_t index;

	if (characteristic_value_update == UPDATE_ON) {
		for (index=0; index<characteristic_value_length; index++) {
			data[index] = characteristic_value[index];
		}
		*length = characteristic_value_length;
	}

	return characteristic_value_update;
}

/*
 * Function: Transparent通信受信データ取得
 * Argument: 受信データ格納先
 * Return  : 受信データ取得成否
 * Note    : なし
 */
bm71_result_t GetTransparentReceiveData(uint8_t* data)
{
	bm71_result_t result;

	result = BM71_RESULT_ERROR;

	if (transparent_receive_data_end != transparent_receive_data_front) {
		*data = transparent_receive_data[transparent_receive_data_end];
		transparent_receive_data_end++;
		if (transparent_receive_data_end >= TRANSPARENT_RECEIVE_BUFFER_SIZE) {
			transparent_receive_data_end = 0;
		}
		result = BM71_RESULT_OK;
	}

	return result;
}

/*
 * Function: Transparent通信送信データ設定
 * Argument: 送信データ格納先、送信データ長
 * Return  : なし
 * Note    : なし
 */
void SetTransparentSendData(uint8_t* data, uint8_t length)
{
	uint16_t index;

	if (length < TRANSPARENT_SEND_BUFFER_SIZE) {
		for (index=0; index<length; index++) {
			transparent_send_data[index] = data[index];
		}
		transparent_send_data_front = length;
	}
}

/*
 * Function: BM71への要求処理
 * Argument: なし
 * Return  : なし
 * Note    : なし
 */
static void processRequest(void)
{
	uint16_t index;

	switch (bm71_request) {
	case BM71_REQUEST_RESET:
		send_buffer[3] = 0x02;
		makeMessage(1);
		SendAsyncUart(send_buffer, 1 + 4);
		break;
	case BM71_REQUEST_READ_STATUS:
		send_buffer[3] = 0x03;
		makeMessage(1);
		SendAsyncUart(send_buffer, 1 + 4);
		break;
	case BM71_REQUEST_SET_ADVERTISING_ENABLE:
		send_buffer[3] = 0x1C;
		send_buffer[4] = 0x01;
		makeMessage(2);
		SendAsyncUart(send_buffer, 2 + 4);
		break;
	case BM71_REQUEST_SET_SCAN_ENABLE:
		send_buffer[3] = 0x16;
		send_buffer[4] = 0x01;	/* Scanning is enabled */
		send_buffer[5] = 0x01;	/* Filtering out duplicate advertisement packets from the same Bluetooth address */
		makeMessage(3);
		SendAsyncUart(send_buffer, 3 + 4);
		break;
	case BM71_REQUEST_LE_CREATE_CONNECTION:
		if (device_detect_state == DEVICE_DETECTED) {
			device_detect_state = DEVICE_NOT_DETECT;
			send_buffer[3] = 0x17;
			send_buffer[4] = 0x00;
			send_buffer[5] = 0x00;
			send_buffer[6] = device_address[0];
			send_buffer[7] = device_address[1];
			send_buffer[8] = device_address[2];
			send_buffer[9] = device_address[3];
			send_buffer[10] = device_address[4];
			send_buffer[11] = device_address[5];
			makeMessage(9);
			SendAsyncUart(send_buffer, 9 + 4);
		}
		break;
	case BM71_REQUEST_ENABLE_TRANSPARENT_UART_SERVICE:
		send_buffer[3] = 0x35;
		send_buffer[4] = connection_handle;
		send_buffer[5] = 0x01;
		send_buffer[6] = 0x00;
		makeMessage(4);
		SendAsyncUart(send_buffer, 4 + 4);
		break;
	case BM71_REQUEST_TRANSPARENT_UART_SERVICE_SEND_DATA:
		if (transparent_send_data_front != 0) {
			send_buffer[3] = 0x3F;
			send_buffer[4] = connection_handle;
			for (index=0; index<transparent_send_data_front; index++) {
				send_buffer[5 + index] = transparent_send_data[index];
			}
			makeMessage(2 + transparent_send_data_front);
			SendAsyncUart(send_buffer, 2 + transparent_send_data_front + 4);
		}
		break;
	case BM71_REQUEST_NONE:	/* Fall through */
	default:
		/* 処理なし */
		break;
	}
	bm71_request = BM71_REQUEST_NONE;
}

/*
 * Function: BM71への要求メッセージ作成
 * Argument: なし
 * Return  : なし
 * Note    : なし
 */
static void makeMessage(uint16_t length)
{
	send_buffer[0] = 0xAA;
	send_buffer[1] = (length >> 8) & 0xFF;
	send_buffer[2] = (length >> 0) & 0xFF;
	send_buffer[3 + length] = calculateChecksum(&send_buffer[1], length + 2);
}

/*
 * Function: 受信データ確認処理
 * Argument: なし
 * Return  : なし
 * Note    : なし
 */
static void processReceive(void)
{
	uint8_t receive_data;
	uart_result_t uart_result;
	uint16_t message_length;

	/* 受信データがなくなるまでループ */
	do {
		uart_result = GetReceiveUart(&receive_data);
		if (uart_result == UART_RESULT_OK) {
			if (receive_buffer_front == 0) {
				/* 先頭データは0xAAのみ受付 */
				if (receive_data == 0xAA) {
					receive_buffer[receive_buffer_front] = receive_data;
					receive_buffer_front++;
				}
			} else if (receive_buffer_front <= 2) {
				/* Lengthデータは無条件で格納 */
				receive_buffer[receive_buffer_front] = receive_data;
				receive_buffer_front++;
			} else {
				/* Lengthで指定されたデータまで格納 */
				message_length = (receive_buffer[1] << 8) + receive_buffer[2];
				message_length += 4; /* HEAD(Start + Length)の3byteとChecksumの1byte分を加算 */
				receive_buffer[receive_buffer_front] = receive_data;
				receive_buffer_front++;
				if (receive_buffer_front >= message_length) {
					/* 1メッセージ分受信できたのでメッセージを解析 */
					parseMessage();
					/* 次のメッセージを受信するためにバッファクリア */
					receive_buffer_front = 0;
				}
			}
		}
	} while (uart_result == UART_RESULT_OK);
}

/*
 * Function: 受信メッセージ解析
 * Argument: なし
 * Return  : なし
 * Note    : なし
 */
static void parseMessage(void)
{
	uint8_t checksum;
	uint16_t message_length;
	uint16_t index;

	/* 最初にチェックサムを確認 */
	message_length = (receive_buffer[1] << 8) + receive_buffer[2];
	checksum = calculateChecksum(&receive_buffer[1], message_length + 2);
	if (checksum == receive_buffer[message_length + 3]) {
		/* チェックサムが正常なのでOP Codeを解析 */
		switch (receive_buffer[3]) {
		case 0x70:
			/* ADVERTISING REPORT EVENT */
			/* デバイス名が BM71_BLE のアドバタイズを探して、デバイスアドレスを取得 */
			if ((receive_buffer[15] == 0x42)
			 && (receive_buffer[16] == 0x4D)
			 && (receive_buffer[17] == 0x37)
			 && (receive_buffer[18] == 0x31)
			 && (receive_buffer[19] == 0x5F)
			 && (receive_buffer[20] == 0x42)
			 && (receive_buffer[21] == 0x4C)
			 && (receive_buffer[22] == 0x45)) {
				device_detect_state = DEVICE_DETECTED;
				device_address[0] = receive_buffer[6];
				device_address[1] = receive_buffer[7];
				device_address[2] = receive_buffer[8];
				device_address[3] = receive_buffer[9];
				device_address[4] = receive_buffer[10];
				device_address[5] = receive_buffer[11];
			}
			break;
		case 0x71:
			/* LE CONNECTION COMPLETE EVENT */
			/* 接続成功の場合はConnection Handleを取得 */
			if (receive_buffer[4] == 0x00) {
				connection_handle = receive_buffer[5];
			}
			break;
		case 0x81:
			/* STATUS REPORT EVENT */
			/* 受信したステータスを取得 */
			switch (receive_buffer[5]) {
			case 0x01:
				bm71_status = BM71_STATUS_SCANNING_MODE;
				break;
			case 0x02:
				bm71_status = BM71_STATUS_CONNECTING_MODE;
				break;
			case 0x03:
				bm71_status = BM71_STATUS_STANDBY_MODE;
				break;
			case 0x05:
				bm71_status = BM71_STATUS_BROADCAST_MODE;
				break;
			case 0x08:
				bm71_status = BM71_STATUS_TRANSPARENT_SERVICE_ENABLED_MODE;
				break;
			case 0x09:
				bm71_status = BM71_STATUS_IDLE_MODE;
				break;
			case 0x0A:
				bm71_status = BM71_STATUS_SHUTDOWN_MODE;
				break;
			case 0x0B:
				bm71_status = BM71_STATUS_CONFIGURE_MODE;
				break;
			case 0x0C:
				bm71_status = BM71_STATUS_BLUETOOTH_LOW_ENERGY_CONNECTED_MODE;
				break;
			default:
				bm71_status = BM71_STATUS_UNKNOWN;
				break;
			}
			break;
		case 0x98:
			/* CLIENT WRITE CHARACTERISTIC VALUE EVENT */
			/* 受信したCharacteristic valueを取得 */
			characteristic_value_length = message_length - 4;
			for (index=0; index<characteristic_value_length; index++) {
				characteristic_value[index] = receive_buffer[index + 7];
			}
			characteristic_value_update = UPDATE_ON;
			break;
		case 0x9A:
			/* RECEIVED TRANSPARENT DATA EVENT RESPONSE */
			/* Transparent通信の受信データを取得 */
			for (index=0; index<(message_length-2); index++) {
				addTransparentReceiveData(receive_buffer[index + 5]);
			}
			break;
		default:
			/* 不要なメッセージは破棄 */
			break;
		}
	} else {
		/* チェックサム異常は破棄 */
	}
}

/*
 * Function: チェックサム計算
 * Argument: 対象データ、対象データ長
 * Return  : チェックサム値
 * Note    : なし
 */
static uint8_t calculateChecksum(uint8_t* data, uint16_t length)
{
	uint8_t checksum;
	uint16_t index;

	checksum = 0;

	/* 合計値計算 */
	for (index=0; index<length; index++) {
		checksum += data[index];
	}

	/* 足して0になる値を計算 */
	checksum = ~checksum + 1;

	return checksum;
}

/*
 * Function: Transparent通信受信データ格納
 * Argument: 受信データ
 * Return  : なし
 * Note    : なし
 */
static void addTransparentReceiveData(uint8_t data)
{
	transparent_receive_data[transparent_receive_data_front] = data;
	transparent_receive_data_front++;
	if (transparent_receive_data_front >= TRANSPARENT_RECEIVE_BUFFER_SIZE) {
		transparent_receive_data_front = 0;
	}
}

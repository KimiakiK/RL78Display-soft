/*
 * File: drv_bm71.h
 * Brief: BM71ドライバ
 * Author: KimiakiK
 * Date: 2023/01/08
 */

#ifndef DRV_BM71_H_
#define DRV_BM71_H_

/********** Include **********/

/********** Define **********/

/* Characteristic valueの最大データ長[byte] */
#define CHARACTERISTIC_VALUE_LENGTH_MAX	(20)

/********** Enum **********/

typedef enum {
	BM71_STATUS_PENDING = 0,
	BM71_STATUS_SCANNING_MODE,
	BM71_STATUS_CONNECTING_MODE,
	BM71_STATUS_STANDBY_MODE,
	BM71_STATUS_BROADCAST_MODE,
	BM71_STATUS_TRANSPARENT_SERVICE_ENABLED_MODE,
	BM71_STATUS_IDLE_MODE,
	BM71_STATUS_SHUTDOWN_MODE,
	BM71_STATUS_CONFIGURE_MODE,
	BM71_STATUS_BLUETOOTH_LOW_ENERGY_CONNECTED_MODE,
	BM71_STATUS_UNKNOWN
} bm71_status_t;

typedef enum {
	BM71_REQUEST_NONE = 0,
	BM71_REQUEST_RESET,
	BM71_REQUEST_READ_STATUS,
	BM71_REQUEST_SET_ADVERTISING_ENABLE,
	BM71_REQUEST_SET_SCAN_ENABLE,
	BM71_REQUEST_LE_CREATE_CONNECTION,
	BM71_REQUEST_ENABLE_TRANSPARENT_UART_SERVICE,
	BM71_REQUEST_TRANSPARENT_UART_SERVICE_SEND_DATA
} bm71_request_t;

typedef enum {
	UPDATE_NOT = 0,
	UPDATE_ON
} update_t;

typedef enum {
	BM71_RESULT_OK = 0,
	BM71_RESULT_ERROR
} bm71_result_t;

/********** Type **********/

/********** Constant **********/

/********** Variable **********/

/********** Function Prototype **********/

void InitBM71(void);
void MainBM71(void);
void SetBM71Request(bm71_request_t request);
bm71_status_t GetBM71Status(void);
update_t GetCharacteristicValue(uint8_t*data, uint8_t* length);
bm71_result_t GetTransparentReceiveData(uint8_t* data);
void SetTransparentSendData(uint8_t* data, uint8_t length);

#endif /* DRV_BM71_H_ */

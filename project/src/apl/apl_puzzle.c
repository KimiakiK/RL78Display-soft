/*
 * File: apl_puzzle.c
 * Brief: パズルアプリ
 * Author: KimiakiK
 * Date: 2023/01/29
 */

/********** Include **********/

#include "typedef.h"
#include "apl_puzzle.h"
#include "drv_tft.h"
#include "drv_sw.h"
#include "mcal_adc.h"
#include "lib_graphic.h"
#include "stdlib.h"
#include "apl_display.h"

/********** Define **********/

#define TIMER_INCREMENT		(10)	/* 1周期の時間[ms] */

#define DOWN_INTERVAL_INIT	1000
#define DOWN_INTERVAL_DEC	30
#define DOWN_INTERVAL_LOWER	200

#define INPUT_INTERVAL_FIRST	200
#define INPUT_INTERVAL_CONTINUE	(INPUT_INTERVAL_FIRST - 50)

#define STAGE_H		24
#define	STAGE_W		14

#define STAGE_TOP		1
#define STAGE_BOTTOM	22
#define STAGE_LEFT		1
#define STAGE_RIGHT		12

#define PLAYER_POS_X_INIT	(STAGE_LEFT + 4)
#define PLAYER_POS_Y_INIT	(STAGE_TOP)

#define MOVE_LEFT	-1
#define MOVE_RIGHT	1
#define MOVE_UP		-1
#define MOVE_DOWN	1

#define ROTATE_CW	1
#define ROTATE_CCW	-1

#define MINOS_BLOCK_NUM	4
#define ANGLE_NUM	4
#define LINE_BLOCK_NUM	10

#define NO_REQUEST	0
#define REQUEST		1

#define CURRENT_MINOS	(minos_next_table[minos_table_index])

/********** Enum **********/

enum sceneType {
	SCENE_RESET = 0,
	SCENE_START,
	SCENE_PLAY,
	SCENE_GAMEOVER
};

enum inputStateType {
	INPUT_OFF = 0,
	INPUT_ON,
	INPUT_INIT
};

enum inputType {
	INPUT_MOVE_LEFT = 0,
	INPUT_MOVE_RIGHT,
	INPUT_MOVE_DOWN,
	INPUT_MOVE_BOTTOM,
	INPUT_HOLD,
	INPUT_ROTATE_CW,
	INPUT_ROTATE_CCW,
	INPUT_NUM
};

enum minosType {
	MINOS_I = 0,
	MINOS_J,
	MINOS_L,
	MINOS_S,
	MINOS_Z,
	MINOS_O,
	MINOS_T,
	MINOS_NUM
};

enum blockType {
	BLOCK_EMPTY = 0,
	BLOCK_HARD,
	BLOCK_CYAN,
	BLOCK_BLUE,
	BLOCK_ORAGE,
	BLOCK_GREEN,
	BLOCK_RED,
	BLOCK_YELLOW,
	BLOCK_PURPLE,
};

enum timerIdType {
	TIMER_DOWN = 0,
	TIMER_INPUT_LEFT,
	TIMER_INPUT_RIGHT,
	TIMER_INPUT_DOWN,
	TIMER_NUM
};

enum holdStateType {
	HOLD_STATE_POSSIBLE = 0,
	HOLD_STATE_DONE
};

enum checkResultType {
	CHECK_OK = 0,
	CHECK_NG
};

/********** Type **********/

typedef struct pointType {
	uint8_t x;
	uint8_t y;
}point_t;

/********** Constant **********/

const point_t minos_pos[MINOS_NUM][ANGLE_NUM][MINOS_BLOCK_NUM] =
{
		{ /* I */
				{ {0, 1}, {1, 1}, {2, 1}, {3, 1} },
				{ {2, 0}, {2, 1}, {2, 2}, {2, 3} },
				{ {0, 2}, {1, 2}, {2, 2}, {3, 2} },
				{ {1, 0}, {1, 1}, {1, 2}, {1, 3} }
		},
		{ /* J */
				{ {0, 0}, {0, 1}, {1, 1}, {2, 1} },
				{ {1, 0}, {1, 1}, {1, 2}, {2, 0} },
				{ {0, 1}, {1, 1}, {2, 1}, {2, 2} },
				{ {0, 2}, {1, 0}, {1, 1}, {1, 2} }
		},
		{ /* L */
				{ {0, 1}, {1, 1}, {2, 0}, {2, 1} },
				{ {1, 0}, {1, 1}, {1, 2}, {2, 2} },
				{ {0, 1}, {0, 2}, {1, 1}, {2, 1} },
				{ {0, 0}, {1, 0}, {1, 1}, {1, 2} }
		},
		{ /* S */
				{ {0, 1}, {1, 0}, {1, 1}, {2, 0} },
				{ {1, 0}, {1, 1}, {2, 1}, {2, 2} },
				{ {0, 2}, {1, 1}, {1, 2}, {2, 1} },
				{ {0, 0}, {0, 1}, {1, 1}, {1, 2} }
		},
		{ /* Z */
				{ {0, 0}, {1, 0}, {1, 1}, {2, 1} },
				{ {1, 1}, {1, 2}, {2, 0}, {2, 1} },
				{ {0, 1}, {1, 1}, {1, 2}, {2, 2} },
				{ {0, 1}, {0, 2}, {1, 0}, {1, 1} }
		},
		{ /* O */
				{ {1, 0}, {1, 1}, {2, 0}, {2, 1} },
				{ {1, 0}, {1, 1}, {2, 0}, {2, 1} },
				{ {1, 0}, {1, 1}, {2, 0}, {2, 1} },
				{ {1, 0}, {1, 1}, {2, 0}, {2, 1} }
		},
		{ /* T */
				{ {0, 1}, {1, 0}, {1, 1}, {2, 1} },
				{ {1, 0}, {1, 1}, {1, 2}, {2, 1} },
				{ {0, 1}, {1, 1}, {1, 2}, {2, 1} },
				{ {0, 1}, {1, 0}, {1, 1}, {1, 2} }
		}
};

/********** Variable **********/

static uint8_t scene;
static uint8_t stage[STAGE_H][STAGE_W];
static point_t player_pos;
static point_t player_pos_old;
static uint8_t palyer_down_prediction_old;
static uint8_t minos_next_table[MINOS_NUM * 2];
static uint8_t minos_table_index;
static uint8_t minos_angle;
static uint8_t minos_angle_old;
static point_t rotate_prev_pos;
static int8_t rotate_prev_value;
static uint8_t hold_minos;
static uint8_t hold_state;
static uint16_t down_interval;
static uint16_t timer[TIMER_NUM];
static uint8_t input[INPUT_NUM];
static uint8_t display_player_request;
static uint8_t display_block_request;
static uint8_t display_next_request;

/********** Function Prototype **********/

static void playLoop(void);

static void initStage(void);
static void initPlayerPos(void);
static void initMinosNextTable(uint8_t table_index_offset);
static void initMinosTableIndex(void);
static void initMinosAngle(void);
static void initHold(void);
static void initRotatePrev(void);
static void initDownInterval(void);
static void initTimerAll(void);
static void initTimer(uint8_t timer_id);
static void initInput(void);
static void initDisplayRequest(void);

static void updateTimer(void);

static void inputSW(void);
static uint8_t checkPushSW(void);

static void nextMinos(void);
static void rotateMinos(int8_t rotate);
static void downMinos(void);
static void moveMinos(int8_t direction);
static uint8_t checkPosition(uint8_t minos, uint8_t x, uint8_t y, uint8_t angle);
static void checkLine(void);
static void downBlock(uint8_t line);
static void setDwonInterval(void);
static void putMinos(void);
static void holdMinos(void);

static void updateDisplay(void);
static void drawBackground(void);
static void drawBlocks(void);
static void drawPlayer(void);
static void drawPlayerDownPrediction(void);
static void drawImage(int8_t x, int8_t y, block_id_t color);
static void drawNextMinos(void);
static void drawHoldMinos(void);
static void drawText(void);

/********** Function **********/

/*
 * Function: パズル画面遷移許可取得
 * Argument: なし
 * Return  : 画面遷移許可
 * Note    : なし
 */
display_transition_enable_t GetPuzzleDisplayTransitonEnable(void)
{
	display_transition_enable_t display_transition;

	if (scene == SCENE_PLAY) {
		display_transition = DISPLAY_TRANSITION_DISABLE;
	} else {
		display_transition = DISPLAY_TRANSITION_ENABLE;
	}

	return display_transition;
}

/*
 * Function: パズル表示初期化
 * Argument: なし
 * Return  : なし
 * Note    : なし
 */
void DrawPuzzleInit(void)
{
	scene = SCENE_RESET;
	srand(GetAdc(ADC_ID_POS_H) + GetAdc(ADC_ID_POS_V) << 8);
	drawBackground();
}

/*
 * Function: パズル表示
 * Argument: なし
 * Return  : なし
 * Note    : なし
 */
void DrawPuzzle(void)
{
	switch (scene) {
	case SCENE_RESET:
		initStage();
		initPlayerPos();
		initMinosNextTable(0);
		initMinosNextTable(7);
		initMinosTableIndex();
		initMinosAngle();
		initHold();
		initRotatePrev();
		initDownInterval();
		initTimerAll();
		initInput();
		initDisplayRequest();
		display_block_request = REQUEST;
		updateDisplay();
		scene = SCENE_START;
		break;
	case SCENE_START:
		if (checkPushSW() == INPUT_ON) {
			scene = SCENE_PLAY;
			initInput();
			initTimerAll();
			display_player_request = REQUEST;
			display_next_request = REQUEST;
			updateDisplay();
		}
		break;
	case SCENE_PLAY:
		playLoop();
		break;
	case SCENE_GAMEOVER:
		if (checkPushSW() == INPUT_ON) {
			scene = SCENE_RESET;
			initInput();
		}
		break;
	default:
		break;
	}
}

static void playLoop(void)
{
	/* 画面更新要求初期化 */
	initDisplayRequest();

	/* タイマー更新 */
	updateTimer();

	/* 入力判定・処理 */
	inputSW();

	/* 落下判定・処理 */
	if (timer[TIMER_DOWN] > down_interval) {
		downMinos();
	}

	/* 画面更新処理 */
	updateDisplay();

	/* 前回値更新 */
	player_pos_old.x = player_pos.x;
	player_pos_old.y = player_pos.y;
	minos_angle_old = minos_angle;
}

static void initStage(void)
{
	uint8_t x, y;

	for (y=0; y<STAGE_H; y++) {
		for (x=0; x<STAGE_W; x++) {
			if (x <= STAGE_LEFT) {
				stage[y][x] = BLOCK_HARD;
			} else if (x >= STAGE_RIGHT) {
				stage[y][x] = BLOCK_HARD;
			} else if (y >= STAGE_BOTTOM) {
				stage[y][x] = BLOCK_HARD;
			} else {
				stage[y][x] = BLOCK_EMPTY;
			}
		}
	}
}

static void initPlayerPos(void)
{
	player_pos.x = PLAYER_POS_X_INIT;
	player_pos.y = PLAYER_POS_Y_INIT;
	player_pos_old.x = player_pos.x;
	player_pos_old.y = player_pos.y;
	palyer_down_prediction_old = 0;
	display_player_request = REQUEST;
	display_next_request = REQUEST;
	display_block_request = REQUEST;
}

static void initMinosNextTable(uint8_t table_index_offset)
{
	uint16_t random_value;
	int8_t table_index, loop;

	for (table_index=table_index_offset; table_index<(MINOS_NUM+table_index_offset); table_index++) {
		do {
			random_value = rand();
			random_value %= MINOS_NUM;
			for (loop=table_index_offset; loop<(table_index+table_index_offset); loop++) {
				if (random_value == minos_next_table[loop]) {
					break;
				}
			}
		} while (loop < table_index);
		minos_next_table[table_index] = random_value;
	}
}

static void initMinosTableIndex(void)
{
	minos_table_index = 0;
}

static void initMinosAngle(void)
{
	minos_angle = 0;
	minos_angle_old = minos_angle;
}

static void initHold(void)
{
	hold_minos = MINOS_NUM;
	hold_state = HOLD_STATE_POSSIBLE;
}

static void initRotatePrev(void)
{
	rotate_prev_pos.x = 0;
	rotate_prev_pos.y = 0;
	rotate_prev_value = 0;
}

static void initDownInterval(void)
{
	down_interval = DOWN_INTERVAL_INIT;
}

static void initTimerAll(void)
{
	uint8_t timer_id;

	for (timer_id=0; timer_id<TIMER_NUM; timer_id++) {
		initTimer(timer_id);
	}
}

static void initTimer(uint8_t timer_id)
{
	timer[timer_id] = 0;
}

static void initInput(void)
{
	uint8_t index;

	for (index=0; index<INPUT_NUM; index++) {
		input[index] = INPUT_INIT;
	}
}

static void initDisplayRequest(void)
{
	display_player_request = NO_REQUEST;
	display_block_request = NO_REQUEST;
	display_next_request = NO_REQUEST;
}

static void updateTimer(void)
{
	uint8_t timer_id;

	for (timer_id=0; timer_id<TIMER_NUM; timer_id++) {
		timer[timer_id] += TIMER_INCREMENT;
	}
}

static void inputSW(void)
{
	sw_state_t sw, sw2;

	/* 左移動判定・処理 */
	sw = GetSw(SW_ID_POS_L);
	if ((sw==SW_PUSH) || (sw==SW_ON)) {
		if (input[INPUT_MOVE_LEFT] == INPUT_OFF) {
			moveMinos(MOVE_LEFT);
			initTimer(TIMER_INPUT_LEFT);
			input[INPUT_MOVE_LEFT] = INPUT_ON;
		} else if (input[INPUT_MOVE_LEFT] == INPUT_ON) {
			if (timer[TIMER_INPUT_LEFT] >= INPUT_INTERVAL_FIRST) {
				moveMinos(MOVE_LEFT);
				timer[TIMER_INPUT_LEFT] = INPUT_INTERVAL_CONTINUE;
			}
		}
	} else {
		input[INPUT_MOVE_LEFT] = INPUT_OFF;
	}
	/* 右移動判定・処理 */
	sw = GetSw(SW_ID_POS_R);
	if ((sw==SW_PUSH) || (sw==SW_ON)) {
		if (input[INPUT_MOVE_RIGHT] == INPUT_OFF) {
			moveMinos(MOVE_RIGHT);
			initTimer(TIMER_INPUT_RIGHT);
			input[INPUT_MOVE_RIGHT] = INPUT_ON;
		} else if (input[INPUT_MOVE_RIGHT] == INPUT_ON) {
			if (timer[TIMER_INPUT_RIGHT] >= INPUT_INTERVAL_FIRST) {
				moveMinos(MOVE_RIGHT);
				timer[TIMER_INPUT_RIGHT] = INPUT_INTERVAL_CONTINUE;
			}
		}
	} else {
		input[INPUT_MOVE_RIGHT] = INPUT_OFF;
	}
	/* 下移動判定・処理 */
	sw = GetSw(SW_ID_POS_D);
	if ((sw==SW_PUSH) || (sw==SW_ON)) {
		if (input[INPUT_MOVE_DOWN] == INPUT_OFF) {
			downMinos();
			initTimer(TIMER_INPUT_DOWN);
			input[INPUT_MOVE_DOWN] = INPUT_ON;
		} else if (input[INPUT_MOVE_DOWN] == INPUT_ON) {
			if (timer[TIMER_INPUT_DOWN] >= INPUT_INTERVAL_FIRST) {
				downMinos();
				timer[TIMER_INPUT_DOWN] = INPUT_INTERVAL_CONTINUE;
			}
		}
	} else {
		input[INPUT_MOVE_DOWN] = INPUT_OFF;
	}
	/* 落下移動判定・処理 */
	sw = GetSw(SW_ID_POS_U);
	if ((sw==SW_PUSH) || (sw==SW_ON)) {
		if (input[INPUT_MOVE_BOTTOM] == INPUT_OFF) {
			uint8_t current_pos;
			do {
				current_pos = player_pos.y;
				downMinos();
				/* 下移動可能な間ループ */
			} while (current_pos < player_pos.y);
			input[INPUT_MOVE_BOTTOM] = INPUT_ON;
		}
	} else {
		input[INPUT_MOVE_BOTTOM] = INPUT_OFF;
	}
	/* 回転(時計回り)判定・処理 */
	sw = GetSw(SW_ID_A);
	if ((sw==SW_PUSH) || (sw==SW_ON)) {
		if (input[INPUT_ROTATE_CW] == INPUT_OFF) {
			rotateMinos(ROTATE_CW);
			input[INPUT_ROTATE_CW] = INPUT_ON;
		}
	} else {
		input[INPUT_ROTATE_CW] = INPUT_OFF;
	}
	/* 回転(反時計回り)判定・処理 */
	sw = GetSw(SW_ID_B);
	if ((sw==SW_PUSH) || (sw==SW_ON)) {
		if (input[INPUT_ROTATE_CCW] == INPUT_OFF) {
			rotateMinos(ROTATE_CCW);
			input[INPUT_ROTATE_CCW] = INPUT_ON;
		}
	} else {
		input[INPUT_ROTATE_CCW] = INPUT_OFF;
	}
	/* ホールド判定・処理 */
	sw = GetSw(SW_ID_C);
	sw2  = GetSw(SW_ID_D);
	if ((sw==SW_PUSH) || (sw==SW_ON) || (sw2==SW_PUSH) || (sw2==SW_ON)) {
		if (input[INPUT_HOLD] == INPUT_OFF) {
			holdMinos();
			input[INPUT_HOLD] = INPUT_ON;
		}
	} else {
		input[INPUT_HOLD] = INPUT_OFF;
	}
}

static uint8_t checkPushSW(void)
{
	sw_state_t sw;

	/* ゲーム開始時とゲームオーバー時のSW入力判定 */
	sw = GetSw(SW_ID_A);
	if ((sw==SW_PUSH) || (sw==SW_ON)) {
		if (input[INPUT_ROTATE_CW] == INPUT_OFF) {
			input[INPUT_ROTATE_CW] = INPUT_ON;
		}
	} else {
		input[INPUT_ROTATE_CW] = INPUT_OFF;
	}

	return input[INPUT_ROTATE_CW];
}

static void nextMinos(void)
{
	minos_table_index ++;
	if (minos_table_index >= MINOS_NUM*2) {
		minos_table_index = 0;
	}
	if (minos_table_index == 0) {

		initMinosNextTable(7);
	}

	if (minos_table_index == 7) {

		initMinosNextTable(0);
	}
	initMinosAngle();
}

static void rotateMinos(int8_t rotate)
{
	uint8_t next_angle, temp_x, temp_y, result;

	next_angle = (uint8_t)(minos_angle + ANGLE_NUM + rotate) % ANGLE_NUM;
	temp_x = player_pos.x;
	temp_y = player_pos.y;

	result = CHECK_NG;

	/* 下に移動して回転判定 */
	if (result == CHECK_NG) {
		uint8_t x, y;
		if (input[INPUT_MOVE_LEFT] == INPUT_ON) {
			x = player_pos.x + minos_pos[CURRENT_MINOS][minos_angle][0].x;
			y = player_pos.y + minos_pos[CURRENT_MINOS][minos_angle][0].y;
			if (stage[y-1][x] != BLOCK_EMPTY) {
				temp_x = player_pos.x + MOVE_LEFT;
				temp_y = player_pos.y + MOVE_DOWN * 2;
				result = checkPosition(CURRENT_MINOS, temp_x, temp_y, next_angle);
			}
		}
		if (input[INPUT_MOVE_RIGHT] == INPUT_ON) {
			x = player_pos.x + minos_pos[CURRENT_MINOS][minos_angle][3].x;
			y = player_pos.y + minos_pos[CURRENT_MINOS][minos_angle][3].y;
			if (stage[y-1][x] != BLOCK_EMPTY) {
				temp_x = player_pos.x + MOVE_RIGHT;
				temp_y = player_pos.y + MOVE_DOWN * 2;
				result = checkPosition(CURRENT_MINOS, temp_x, temp_y, next_angle);
			}
		}
		if (result == CHECK_NG) {
			temp_x = player_pos.x;
			temp_y = player_pos.y;
		}
	}

	/* その場で90度回転判定 */
	if (result == CHECK_NG) {
		result = checkPosition(CURRENT_MINOS, player_pos.x, player_pos.y, next_angle);
	}

	/* 上に移動して回転判定 */
	if (result == CHECK_NG) {
		if (player_pos.y > 0) {
			temp_y = player_pos.y + MOVE_UP;
			result = checkPosition(CURRENT_MINOS, player_pos.x, temp_y, next_angle);
			if ((result == CHECK_NG) && (CURRENT_MINOS == MINOS_I)) {
				/* Iはさらに横移動して回転判定 */
				temp_y = temp_y + MOVE_UP;
				result = checkPosition(CURRENT_MINOS, player_pos.x, temp_y, next_angle);
			}
			if (result == CHECK_NG) {
				temp_y = player_pos.y;
			}
		}
	}

	/* その場で180度回転判定 */
	if ((result == CHECK_NG) && (rotate_prev_pos.x == player_pos.x) && (rotate_prev_value == rotate) && (CURRENT_MINOS != MINOS_I)) {
		next_angle = (uint8_t)(next_angle + ANGLE_NUM + rotate) % ANGLE_NUM;
		result = checkPosition(CURRENT_MINOS, player_pos.x, player_pos.y, next_angle);
	}

	/* 左に移動して回転判定 */
	if (result == CHECK_NG) {
		temp_x = player_pos.x + MOVE_LEFT;
		result = checkPosition(CURRENT_MINOS, temp_x, player_pos.y, next_angle);
		if (result == CHECK_NG) {
			/* 下に移動しながら回転範囲 */
			temp_y = player_pos.y + MOVE_DOWN;
			result = checkPosition(CURRENT_MINOS, temp_x, temp_y, next_angle);
		}
		if (result == CHECK_NG) {
			temp_y = player_pos.y;
		}
		if ((result == CHECK_NG) && (CURRENT_MINOS == MINOS_I)) {
			/* Iはさらに横移動して回転判定 */
			temp_x = temp_x + MOVE_LEFT;
			result = checkPosition(CURRENT_MINOS, temp_x, player_pos.y, next_angle);
		}
	}

	/* 右に移動して回転判定 */
	if (result == CHECK_NG) {
		temp_x = player_pos.x + MOVE_RIGHT;
		result = checkPosition(CURRENT_MINOS, temp_x, player_pos.y, next_angle);
		if (result == CHECK_NG) {
			/* 下に移動しながら回転範囲 */
			temp_y = player_pos.y + MOVE_DOWN;
			result = checkPosition(CURRENT_MINOS, temp_x, temp_y, next_angle);
		}
		if (result == CHECK_NG) {
			temp_y = player_pos.y;
		}
		if ((result == CHECK_NG) && (CURRENT_MINOS == MINOS_I)) {
			/* Iはさらに横移動して回転判定 */
			temp_x = temp_x + MOVE_RIGHT;
			result = checkPosition(CURRENT_MINOS, temp_x, player_pos.y, next_angle);
		}
	}

	if (result == CHECK_OK) {
		/* 回転成功 */
		minos_angle = next_angle;
		player_pos.x = temp_x;
		player_pos.y = temp_y;
		initRotatePrev();
		display_player_request = REQUEST;
	} else {
		rotate_prev_pos.x = player_pos.x;
		rotate_prev_pos.y = player_pos.y;
		rotate_prev_value = rotate;
	}
}

static void downMinos(void)
{
	uint8_t next_y, x, result;

	next_y = player_pos.y + MOVE_DOWN;

	result = checkPosition(CURRENT_MINOS, player_pos.x, next_y, minos_angle);

	if (result == CHECK_OK) {
		/* 下移動成功 */
		player_pos.y = next_y;
		/* 下移動でそのまま下接着を回避 */
		next_y = player_pos.y + MOVE_DOWN;
		result = checkPosition(CURRENT_MINOS, player_pos.x, next_y, minos_angle);
		if (result == CHECK_NG) {
			initTimer(TIMER_INPUT_DOWN);
		}
	} else {
		/* 下接着 */
		hold_state = HOLD_STATE_POSSIBLE;
		display_block_request = REQUEST;
		putMinos();
		checkLine();
		/* ゲームオーバー判定 */
		for (x=STAGE_LEFT; x<STAGE_RIGHT; x++) {
			if (stage[STAGE_TOP][x] > BLOCK_HARD) {
				break;
			}
		}
		if (x<STAGE_RIGHT) {
			/* ゲームオーバー */
			scene = SCENE_GAMEOVER;
		} else {
			/* ゲーム継続 */
			initPlayerPos();
			nextMinos();
			initInput();
		}
	}

	display_player_request = REQUEST;
	initTimer(TIMER_DOWN);
}

static void moveMinos(int8_t direction)
{
	uint8_t next_x, result;

	next_x = player_pos.x + direction;

	result = checkPosition(CURRENT_MINOS, next_x, player_pos.y, minos_angle);

	if (result == CHECK_OK) {
		/* 左右移動成功 */
		player_pos.x = next_x;
		display_player_request = REQUEST;
	}
}

static uint8_t checkPosition(uint8_t minos, uint8_t offset_x, uint8_t offset_y, uint8_t angle)
{
	uint8_t index, x, y, result;

	result = CHECK_OK;

	for (index=0; index<MINOS_BLOCK_NUM; index++) {
		x = offset_x + minos_pos[minos][angle][index].x;
		y = offset_y + minos_pos[minos][angle][index].y;
		if (stage[y][x] != BLOCK_EMPTY) {
			result = CHECK_NG;
			break;
		}
	}

	return result;
}

static void checkLine(void)
{
	uint8_t x, y;
	uint8_t block_num;

	for (y=STAGE_TOP; y<STAGE_BOTTOM; y++) {
		block_num = 0;
		for (x=STAGE_LEFT+1; x<STAGE_RIGHT; x++) {
			if (stage[y][x] != BLOCK_EMPTY) {
				block_num++;
			}
		}
		if (block_num >= LINE_BLOCK_NUM) {
			/* 横一列成立 */
			downBlock(y);
			setDwonInterval();
		}
	}
}

static void downBlock(uint8_t line)
{
	uint8_t x, y;

	for (y=line; y>STAGE_TOP; y--) {
		for (x=STAGE_LEFT+1; x<STAGE_RIGHT; x++) {
			stage[y][x] = stage[y-1][x];
			stage[y-1][x] = BLOCK_EMPTY;
		}
	}
}

static void setDwonInterval(void)
{
	if (down_interval > DOWN_INTERVAL_LOWER) {
		down_interval -= DOWN_INTERVAL_DEC;
	}
}

static void putMinos(void)
{
	uint8_t index, x, y;

	for (index=0; index<MINOS_BLOCK_NUM; index++) {
		x = player_pos.x + minos_pos[CURRENT_MINOS][minos_angle][index].x;
		y = player_pos.y + minos_pos[CURRENT_MINOS][minos_angle][index].y;
		stage[y][x] = CURRENT_MINOS + BLOCK_CYAN;
	}
}

static void holdMinos(void)
{
	if (hold_state == HOLD_STATE_POSSIBLE) {
		hold_state = HOLD_STATE_DONE;
		if (hold_minos >= MINOS_NUM) {
			/* 初回(交換なし) */
			hold_minos = CURRENT_MINOS;
			nextMinos();
		} else {
			/* 交換 */
			uint8_t temp_minos;
			temp_minos = CURRENT_MINOS;
			CURRENT_MINOS = hold_minos;
			hold_minos = temp_minos;
			initMinosAngle();
		}
		initPlayerPos();
		initInput();
	}
}

/****************************************/
/**********   Draw Functions   **********/
/****************************************/


static void updateDisplay(void)
{
	drawText();
	if (display_block_request == REQUEST) {
		drawBlocks();
	}
	if (scene==SCENE_PLAY) {
		if (display_player_request == REQUEST) {
			drawPlayerDownPrediction();
			drawPlayer();
		}
		if (display_next_request == REQUEST) {
			drawNextMinos();
			drawHoldMinos();
		}
	}
}

#define BLOCK_SIZE				(10)
#define BACKGROUND_OFFSET_X		(60)
#define BACKGROUND_OFFSET_Y		(30)
#define BACKGROUND_NUM_X		(12)
#define BACKGROUND_NUM_Y		(20)
#define STAGE_OFFSET_X			(BACKGROUND_OFFSET_X + BLOCK_SIZE)
#define STAGE_OFFSET_Y			(BACKGROUND_OFFSET_Y)
#define STAGE_NUM_X				(10)
#define STAGE_NUM_Y				(20)


static void drawBackground(void)
{
	uint8_t x, y;

	/* 両側 */
	for (y=0; y<BACKGROUND_NUM_Y; y++) {
		drawImage(-1, y, BLOCK_ID_HARD);
		drawImage(BACKGROUND_NUM_X - 2, y, BLOCK_ID_HARD);
	}

	/* 下 */
	for (x=0; x<BACKGROUND_NUM_X; x++) {
		drawImage(x - 1, BACKGROUND_NUM_Y, BLOCK_ID_HARD);
	}

	/* next/hold 垂直 */
	for (y=0; y<BACKGROUND_NUM_Y; y++) {
		drawImage(BACKGROUND_NUM_X + 4, y, BLOCK_ID_HARD);
	}

	/* next/hold 水平 */
	for (x=0; x<6; x++) {
		drawImage(x + BACKGROUND_NUM_X - 1, 0, BLOCK_ID_HARD);
		drawImage(x + BACKGROUND_NUM_X - 1, BACKGROUND_NUM_Y - 4, BLOCK_ID_HARD);
		drawImage(x + BACKGROUND_NUM_X - 1, BACKGROUND_NUM_Y, BLOCK_ID_HARD);
	}
}

static void drawBlocks(void)
{
	uint8_t x, y;

	for (x=0; x<10; x++) {
		for (y=0; y<20; y++) {
			drawImage(x, y, (block_id_t)stage[y + STAGE_TOP + 1][x + STAGE_LEFT + 1]);
		}
	}
}

static void drawPlayer(void)
{
	uint8_t index;
	int8_t  x, y;

	for (index=0; index<MINOS_BLOCK_NUM; index++) {
		x = player_pos_old.x + minos_pos[CURRENT_MINOS][minos_angle_old][index].x - STAGE_LEFT - 1;
		y = player_pos_old.y + minos_pos[CURRENT_MINOS][minos_angle_old][index].y - STAGE_TOP - 1;
		if (y >= 0) {
			drawImage(x, y, BLOCK_ID_BACK);
		}
	}

	for (index=0; index<MINOS_BLOCK_NUM; index++) {
		x = player_pos.x + minos_pos[CURRENT_MINOS][minos_angle][index].x - STAGE_LEFT - 1;
		y = player_pos.y + minos_pos[CURRENT_MINOS][minos_angle][index].y - STAGE_TOP - 1;
		if (y >= 0) {
			drawImage(x, y, (block_id_t)((uint8_t)CURRENT_MINOS + 2));
		}
	}
}

static void drawPlayerDownPrediction(void)
{
	uint8_t index, down_pos, result;
	int8_t  x, y;

	down_pos = player_pos.y;
	do {
		down_pos ++;
		result = checkPosition(CURRENT_MINOS, player_pos.x, down_pos, minos_angle);
	} while (result == CHECK_OK);
	down_pos --;

	if (palyer_down_prediction_old != 0) {
		for (index=0; index<MINOS_BLOCK_NUM; index++) {
			x = player_pos_old.x + minos_pos[CURRENT_MINOS][minos_angle_old][index].x - STAGE_LEFT - 1;
			y = palyer_down_prediction_old + minos_pos[CURRENT_MINOS][minos_angle_old][index].y - STAGE_TOP - 1;
			if (y >= 0) {
				drawImage(x, y, BLOCK_ID_BACK);
			}
		}
	}
	
	for (index=0; index<MINOS_BLOCK_NUM; index++) {
		x = player_pos.x + minos_pos[CURRENT_MINOS][minos_angle][index].x - STAGE_LEFT - 1;
		y = down_pos + minos_pos[CURRENT_MINOS][minos_angle][index].y - STAGE_TOP - 1;
		if (y >= 0) {
			drawImage(x, y, (block_id_t)((uint8_t)CURRENT_MINOS + 2 + 7));
		}
	}

	palyer_down_prediction_old = down_pos;
}

static void drawImage(int8_t x, int8_t y, block_id_t color)
{
		DrawTft(STAGE_OFFSET_X + x * BLOCK_SIZE, STAGE_OFFSET_Y + y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, image_block[color].address);
}

#define NEXT_BLOCK_NUM			(5)
#define NEXT_BLOCK_NUM_OFFSET_X	(STAGE_NUM_X + 2.5)
#define NEXT_BLOCK_NUM_OFFSET_Y	(1.5)
#define NEXT_BLOCK_INTERVAL		(3)

static void drawNextMinos(void)
{
	uint8_t index, table_index, block_index;
	int8_t  x, y, offset_x, offset_y;

	for (index=0; index<NEXT_BLOCK_NUM; index++) {
		TftClearRect(BACKGROUND_OFFSET_X + NEXT_BLOCK_NUM_OFFSET_X * BLOCK_SIZE, BACKGROUND_OFFSET_Y + (NEXT_BLOCK_NUM_OFFSET_Y + index * NEXT_BLOCK_INTERVAL) * BLOCK_SIZE, BLOCK_SIZE * 4, BLOCK_SIZE * 2);
		table_index = (minos_table_index + index + 1) % (MINOS_NUM * 2);
		for (block_index=0; block_index<MINOS_BLOCK_NUM; block_index++) {
			x = minos_pos[minos_next_table[table_index]][0][block_index].x + NEXT_BLOCK_NUM_OFFSET_X;
			y = minos_pos[minos_next_table[table_index]][0][block_index].y + index * NEXT_BLOCK_INTERVAL + NEXT_BLOCK_NUM_OFFSET_Y;
			if (minos_next_table[table_index] == MINOS_I) {
				offset_x = -BLOCK_SIZE / 2;
				offset_y = -BLOCK_SIZE / 2;
			} else if (minos_next_table[table_index] == MINOS_O) {
				offset_x = -BLOCK_SIZE / 2;
				offset_y = 0;
			} else {
				offset_x = 0;
				offset_y = 0;
			}
			DrawTft(STAGE_OFFSET_X + x * BLOCK_SIZE + offset_x, STAGE_OFFSET_Y + y * BLOCK_SIZE + BLOCK_SIZE / 2 + offset_y, BLOCK_SIZE, BLOCK_SIZE, image_block[minos_next_table[table_index] + 2].address);
		}
	}
}

#define HOLD_BLOCK_OFFSET_X		(NEXT_BLOCK_NUM_OFFSET_X)
#define HOLD_BLOCK_OFFSET_Y		(17.5)

static void drawHoldMinos(void)
{
	uint8_t block_index;
	int8_t  x, y, offset_x, offset_y;

	if (hold_minos < MINOS_NUM) {
		TftClearRect(BACKGROUND_OFFSET_X + NEXT_BLOCK_NUM_OFFSET_X * BLOCK_SIZE, BACKGROUND_OFFSET_Y + HOLD_BLOCK_OFFSET_Y * BLOCK_SIZE, BLOCK_SIZE * 4, BLOCK_SIZE * 2);
		for (block_index=0; block_index<MINOS_BLOCK_NUM; block_index++) {
			x = minos_pos[hold_minos][0][block_index].x + HOLD_BLOCK_OFFSET_X;
			y = minos_pos[hold_minos][0][block_index].y + HOLD_BLOCK_OFFSET_Y;
			if (hold_minos == MINOS_I) {
				offset_x = -BLOCK_SIZE / 2;
				offset_y = -BLOCK_SIZE / 2;
			} else if (hold_minos == MINOS_O) {
				offset_x = -BLOCK_SIZE / 2;
				offset_y = 0;
			} else {
				offset_x = 0;
				offset_y = 0;
			}
			DrawTft(STAGE_OFFSET_X + x * BLOCK_SIZE + offset_x, STAGE_OFFSET_Y + y * BLOCK_SIZE + BLOCK_SIZE / 2 + offset_y, BLOCK_SIZE, BLOCK_SIZE, image_block[hold_minos + 2].address);
		}
	}
}

static void drawText(void)
{

}

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
#include "lib_graphic.h"

/********** Define **********/

/********** Enum **********/

/********** Type **********/

/********** Constant **********/

/********** Variable **********/

/********** Function Prototype **********/

/********** Function **********/

/*
 * Function: パズル表示初期化
 * Argument: なし
 * Return  : なし
 * Note    : なし
 */
void DrawPuzzleInit(void)
{
	uint8_t x, y;
	uint8_t block_id;
	uint8_t offset_x = 60;
	uint8_t offset_y = 30;
	uint8_t size = 10;
	for (y=0; y<21; y++) {
		DrawTft(offset_x + 0, offset_y + y * size, size, size, image_block[BLOCK_ID_HARD].address);
		DrawTft(offset_x + 11 * size, offset_y + y * size, size, size, image_block[BLOCK_ID_HARD].address);
	}
	for (x=1; x<11; x++) {
		DrawTft(offset_x + x * size, offset_y + 20 * size, size, size, image_block[BLOCK_ID_HARD].address);
	}
	for (y=0; y<20; y++) {
		for (x=1; x<11; x++) {
			block_id = BLOCK_ID_BACK;
			if (y==18) {
				if (x<8) {
					block_id = BLOCK_ID_HARD + x;
				}
			}
			if (y==19) {
				if (x<8) {
					block_id = BLOCK_ID_T + x;
				}
			}
			DrawTft(offset_x + x * size, offset_y + y * size, size, size, image_block[block_id].address);
		}
	}
}

/*
 * Function: パズル表示
 * Argument: なし
 * Return  : なし
 * Note    : なし
 */
void DrawPuzzle(void)
{
}

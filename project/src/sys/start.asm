; Startup code

; Set reset vector
_start	.VECTOR 0

.SECTION	.text, text
_start:
	; Set stack pointer
	MOVW	SP, #LOWW(__STACK_ADDR_START)

	; Set Clock
	MOV		CMC, #0x12	; X1未使用, XT1通常発振
	MOV		CSC, #0x80	; X1停止, XT1発振, オンチップオシレータ動作

	; Clear RAM
	MOVW	HL, #LOWW(__RAM_ADDR_START)
	MOVW	AX, #LOWW(__RAM_ADDR_END)

ram_clear_loop:
	MOV		[HL+0], #0
	INCW	HL
	CMPW	AX, HL
	BNZ		$ram_clear_loop

	; Start main function
	CALL	!!_main

_exit:
	BR		$_exit

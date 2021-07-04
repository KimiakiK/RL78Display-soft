; Setting option bytes

.SECTION	.option_byte, OPT_BYTE
	.DB	0xEF	; WDT disable
	.DB	0xFF	; LVD off
	.DB	0xE8	; internal 32MHz
	.DB	0x85	; Debugger enable

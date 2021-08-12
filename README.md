# RL78Display-soft

## メモリマップ
ROM 512KB 0x00000～0x7FFFF
- debugger 256byte(0x100) 0x7FF00～0x7FFFF
- background_title.bin 115200byte(0x1C200) 0x63D00～0x7FEFF
- background_clock.bin 115200byte(0x1C200) 0x47B00～0x63CFF
- background_controller.bin 115200byte (0x1C200) 0x2B900～0x47AFF
- font_30px.bin 27000byte (0x6978) 0x24F00～0x2B877
- font_50px.bin 33000byte (0x80E8) 0x1CE00～0x24EE7
- sw_off.bin 43264byte (0xA900) 0x12500～0x1CDFF
- sw_on.bin  43264byte (0xA900) 0x7C00～0x124FF

## フォント
- font_30px
 縦: 30px、横: 数字 18px、記号 15px、曜日 30px
- font_50px
 縦: 50px、横: 30
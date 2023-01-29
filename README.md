# RL78Display-soft

## メモリマップ
ROM 512KB 0x00000～0x7FFFF
- debugger 256byte(0x100) 0x7FF00～0x7FFFF
- background_title.bin 115200byte(0x1C200) 0x63D00 ～ 0x7FEFF
- background_clock.bin 115200byte(0x1C200) 0x47B00 ～ 0x63CFF
- font_20px.bin 38000byte (0x9470) 0x3E600 ～ 0x47AFF
- font_30px.bin 27000byte (0x6978) 0x37C00 ～ 0x3E5FF
- font_50px.bin 33000byte (0x80E8) 0x2FB00 ～ 0x37BFF
- sw_off.bin 43264byte (0xA900) 0x25200 ～ 0x2FAFF
- sw_on.bin  43264byte (0xA900) 0x1A900 ～ 0x251FF
- block.bin 3200byte (0xC80) 0x19C00 ～ 0x1A8FF

## フォント
- font_30px
 縦: 30px、横: 数字 18px、記号 15px、曜日 30px
- font_50px
 縦: 50px、横: 30
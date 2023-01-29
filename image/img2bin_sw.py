from PIL import Image
import glob

files = ['controller_inactive.png', 'controller_active.png']
outputs = ['sw_off.bin', 'sw_on.bin']

pos = [(184, 184), (4, 184), (4, 4), (184, 4), (94, 24), (94, 164), (24, 94), (164, 94)]

size = 52

# 指定座標の色取得
def getColor(im, x, y):
  color = im.getpixel((x, y))
  r = color[0]
  g = color[1]
  b = color[2]
  r = int(r / 8) & 0x1F # 赤は5bit
  g = int(g / 4) & 0x3F # 緑は6bit
  b = int(b / 8) & 0x1F # 
  
  return r<<3 | (g&0b111)<<13 | g>>3 | b<<8

# メイン
if __name__ == '__main__':
  for index in range(2):
    with open(outputs[index], 'wb') as bin:
      with Image.open(files[index]) as im:
        for i in range(8):
          for y in range(size):
            for x in range(size):
                color = getColor(im, x + pos[i][0], y+pos[i][1])
                bin.write(color.to_bytes(2, byteorder='little'))
  
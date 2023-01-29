from PIL import Image
import glob

target_file = 'font_20px.png'
bin_file = 'font_0px.bin'

font_width = 10

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
  with open(target_file.replace('png', 'bin'), 'wb') as bin:
    with Image.open(target_file) as im:
      width, height = im.size
      for i in range(int(width / font_width)):
        for y in range(height):
          for x in range(font_width):
            color = getColor(im, x + i * font_width, y)
            bin.write(color.to_bytes(2, byteorder='little'))
  exit()
  

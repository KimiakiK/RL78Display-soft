from PIL import Image
import glob

ignore = ['controller_active.png', 'controller_inactive.png', 'font_20px.png']

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
  files = glob.glob("*.png")
  for filename in files:
    if filename not in ignore:
      with open(filename.replace('png', 'bin'), 'wb') as bin:
        with Image.open(filename) as im:
          width, height = im.size
          for y in range(height):
            for x in range(width):
              color = getColor(im, x, y)
              bin.write(color.to_bytes(2, byteorder='little'))

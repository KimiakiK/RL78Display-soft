from PIL import Image, ImageDraw, ImageFont

# フォントサイズ
font_size = 20
# 画像生成対象文字列
text = ' !"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~'

im = Image.new("RGB", (len(text)*int(font_size/2), font_size))

draw = ImageDraw.Draw(im)

font = ImageFont.truetype("C:/Windows/Fonts/BIZ-UDMinchoM.ttc", font_size)

draw.text((0, 0), text, fill=(255, 255, 255), font=font)

im.save("image.png")

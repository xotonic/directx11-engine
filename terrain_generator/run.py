import os,sys
from PIL import Image, ImageFilter

im = Image.open("picture.png")

#print(im.bits, im.size, im.format)

im = im.convert("F")
w , h = im.size

file = open("out.terrain",'w')
file.write('%d %d\n' % (w,h))

for x in range(w):
    for y in range(h):
        c = im.getpixel((x,y))
        file.write('%d ' % c)
    file.write('\n')
#col = im.getpixel((130,40))
#print(col)
print('converting complete!')
#im.show()
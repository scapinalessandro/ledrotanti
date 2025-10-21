import cv2
import numpy as np
import math
import socket
import struct

img = cv2.imread('./rotondo2.png', cv2.IMREAD_GRAYSCALE)

# img = cv2.blur(img, (5,5))
ret,img = cv2.threshold(img,190,255,cv2.THRESH_BINARY)
cv2.imwrite('./output/blurred.jpg', img)

blank = np.ones((700, 700), dtype = np.uint8)

h, w = img.shape

p = 1
nleds = 40
distance = 4.5

outArray = []

xInitOut = int(w / 2)
yInitOut = int(h / 2)

cd = 19

for i in range(0, int(360/p)):
    csin = math.sin(math.radians(i * p))
    ccos = math.cos(math.radians(i * p))

    buffer : int = 0
    print("{", end="")
    for l in range(nleds, 0, -1):
        x = int(ccos * l * distance) + int(ccos * cd)
        y = int(csin * l * distance) + int(csin * cd)
        
        bit : int = img[y + yInitOut][x + xInitOut] > 128

        buffer = buffer << 1
        buffer = buffer | (bit & 0x1)

        if (l % 8) == 7:
            outArray.append(buffer)
            print(hex(buffer), end="")
            if(l > 7):
                print(",", end="")
            buffer = 0

        blank[y + yInitOut][x + xInitOut] = img[y + yInitOut][x + xInitOut]
    print("},")

cv2.imwrite('./output/final.jpg',blank)

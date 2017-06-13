import cv2
import numpy as np
import matplotlib.pyplot as plt

img =cv2.imread('sandeep.jpg',cv2.IMREAD_GRAYSCALE)
#_COLOR =1
#_GRAYSCALE =0
#_UNCHANGED=-1
'''
cv2.imshow('image',img)
cv2.waitKey(0)
cv2.destroyAllWindows()
'''
plt.imshow(img,cmap='gray',interpolation='bicubic')

plt.show()

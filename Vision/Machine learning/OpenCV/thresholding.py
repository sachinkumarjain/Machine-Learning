# http://docs.opencv.org/trunk/d7/d4d/tutorial_py_thresholding.html
import cv2
import numpy as np
img =cv2.imread('book.jpg')

retvsal,threshold = cv2.threshold(img,120,255,cv2.THRESH_BINARY)

grayscaled = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
retval2, threshold2 = cv2.threshold(grayscaled, 10, 255, cv2.THRESH_BINARY)
th = cv2.adaptiveThreshold(grayscaled, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 115, 1)

cv2.imshow('original',img)
cv2.imshow('threshold',threshold)
cv2.imshow('Adaptive threshold',th)
cv2.waitKey(0)
cv2.destroyAllWindows()

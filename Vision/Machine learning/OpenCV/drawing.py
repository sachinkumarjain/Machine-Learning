import numpy as np
import cv2

img=cv2.imread('sandeep.jpg',cv2.IMREAD_COLOR)
'''
cv2.line(img,(0,0),(150,150),(255,0,0), 15) #bgr
cv2.rectangle(img,(15,25),(200,150),(255,255,255),5)
'''
font=cv2.FONT_HERSHEY_SIMPLEX
cv2.putText(img,'Im Sandeep',(130,130),font,4,(200,255,255),2,cv2.LINE_AA)
cv2.imshow('image',img)
cv2.waitKey(0)
cv2.destoryAllWindows()

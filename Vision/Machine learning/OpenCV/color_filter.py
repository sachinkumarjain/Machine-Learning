'''
http://docs.opencv.org/3.1.0/d4/d13/tutorial_py_filtering.html
bluring and smoothing removing the noise from video
 we are removing other color expect red 

As in one-dimensional signals, images also can be filtered with various low-pass filters(LPF), high-pass filters(HPF) etc. LPF helps in removing noises, blurring the images etc
HPF filters helps in finding edges in the image
'''
import cv2
import numpy as np

cap = cv2.VideoCapture(0)
fourcc=cv2.VideoWriter_fourcc(*'XVID')
out=cv2.VideoWriter('color_filter.avi', fourcc,20.0,(640,480))

while(1):
    _, frame = cap.read()  #_ output we dont want
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV) #hsv color
    
    
    lower_red = np.array([10,0,0])
    upper_red = np.array([155,255,180])
    
    mask = cv2.inRange(hsv, lower_red, upper_red)
    res = cv2.bitwise_and(frame,frame, mask= mask)
    out.write(res)
    kernel=np.ones((15,15),np.float32)/225
    smooethed=cv2.filter2D(res,-1,kernel)
    blur = cv2.GaussianBlur(res,(15,15),0) 

    median = cv2.medianBlur(res,15) # 15 is kernel size 
    bilateral = cv2.bilateralFilter(res,15,75,75)
    cv2.imshow('bilateral Blur',bilateral)
    cv2.imshow('Median Blur',median)
    cv2.imshow('Gaussian Blurring',blur)
    cv2.imshow('frame',frame)
    cv2.imshow('mask',mask)
    cv2.imshow('smooethed',res)
    
    k = cv2.waitKey(5) & 0xFF
    if k == 27:
        break

out.release()
cap.release()
cv2.destroyAllWindows()

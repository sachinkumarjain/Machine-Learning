'''

opening is removes false postive from background 
closing is remove false postive within the object 

'''
import cv2
import numpy as np

cap = cv2.VideoCapture(0)
fourcc=cv2.VideoWriter_fourcc(*'XVID')
out=cv2.VideoWriter('color_.avi', fourcc,20.0,(640,480))


while(1):

    _, frame = cap.read()
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    
    lower_red = np.array([150,150,50])
    upper_red = np.array([180,255,180])
    
    mask = cv2.inRange(hsv, lower_red, upper_red)
    res = cv2.bitwise_and(frame,frame, mask= mask)

    kernel = np.ones((5,5),np.uint8)
    erosion = cv2.erode(mask,kernel,iterations = 1)
    dilation = cv2.dilate(mask,kernel,iterations = 1)
    opening = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernel)
    closing = cv2.morphologyEx(mask, cv2.MORPH_CLOSE, kernel)

    out.write(closing)
    cv2.imshow('Original',frame)
    cv2.imshow('Mask',mask)
    #cv2.imshow('Erosion',erosion)
    #cv2.imshow('Dilation',dilation)
    cv2.imshow('Opening',opening)
    cv2.imshow('Closing',closing)

    k = cv2.waitKey(5) & 0xFF
    if k == 27:
        break

out.release()
cap.release()
cv2.destroyAllWindows()

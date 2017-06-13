import cv2
import numpy as np

cap = cv2.VideoCapture(0)
fourcc=cv2.VideoWriter_fourcc(*'XVID')
out=cv2.VideoWriter('edge.avi', fourcc,20.0,(640,480))

while(1):

    _, frame = cap.read()
    
    laplacian = cv2.Laplacian(frame, cv2.CV_64F)
    sobelx = cv2.Sobel(frame,cv2.CV_64F,1,0,ksize=5)
    sobely = cv2.Sobel(frame,cv2.CV_64F,0,1,ksize=5)
    edges=cv2.Canny(frame,200,200)
    #cv2.imshow('Original',frame)
    cv2.imshow('edgess',edges)
    out.write(edges)
    #cv2.imshow('laplacian',laplacian) # see edge really well but can we use canny
    #cv2.imshow('sobelx',sobelx)
    #cv2.imshow('sobely',sobely)
    
    k = cv2.waitKey(5) & 0xFF
    if k == 27:
        break

cv2.destroyAllWindows()
cap.release()
out.release()

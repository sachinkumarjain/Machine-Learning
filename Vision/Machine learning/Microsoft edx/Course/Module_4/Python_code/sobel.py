def edge_sobel(im_list):
    from scipy import ndimage
    import numpy as np
    out=[]
    for images in im_list:
        dx=ndimage.sobel(image,1)
        dy=ndimage.sobel(image,0)
        mag=np.hypot(dx,dy)
        mag*= 255.0/np.amax(mag)
        mag=mag.astype(np.uint8)
        out.append(mag)
    return out

pictures=edge_sobel(carrot_filter)


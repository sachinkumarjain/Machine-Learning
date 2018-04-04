
import cv2
#import os module for reading training data directories and paths
import os
#import numpy to convert python lists to numpy arrays as 
#it is needed by OpenCV face recognizers
import numpy as np

#there is no label 0 in our training data so subject name for index/label 0 is empty

# list of people names who we what to find 
subjects = ["", "sam", "kamal","gokul"]


# Preparing data step can be further divided into following sub-steps.

# 1. Read all the folder names of subjects/persons provided in training data folder. 
# 2. For each subject, extract label number
# 3. Read all the images of the subject, detect face from each image.
# 4. Add each face to faces vector with corresponding subject label (extracted in above step) added to labels vector. 


def detect_face(img):
    #convert the test image to gray image as opencv face detector expects gray images
    # i found that using color image gave me more accurate results do check with your data 
    #gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    
    #load OpenCV face detector, I am using Haar which is more accurate than lbp but training takes lot of time you can use lbpcascade_frontalface.xml for lbp 
    face_cascade = cv2.CascadeClassifier('opencv-files/haarcascade_frontalface_alt.xml')

    #let's detect multiscale (some images may be closer to camera than others) images result is a list of faces
   
    faces = face_cascade.detectMultiScale(img, scaleFactor=1.2, minNeighbors=6);
    #faces = face_cascade.detectMultiScale(gray, scaleFactor=1.2, minNeighbors=6);    

    #if no faces are detected then return original img
    if (len(faces) == 0):
        return None, None
    
    #under the assumption that there will be only one face,
    #extract the face area
    (x, y, w, h) = faces[0]
    
    #return only the face part of the image
    return img[y:y+w, x:x+h], faces[0]


#this function will read all persons' training images, detect face from each image and will return two lists of exactly same size, one list of faces and another list of labels for each face


def prepare_training_data(data_folder_path):
    
    #------STEP-1--------
    #get the directories (one directory for each subject) in data folder
    dirs = os.listdir(data_folder_path)
    
    #list to hold all subject faces
    faces = []
    #list to hold labels for all subjects
    labels = []
    
    #read images within each directories
    for dir_name in dirs:
        
        #our subject directories start with letter 's' so
        #ignore any non-relevant directories if any
        if not dir_name.startswith("s"):
            continue;
            
        label = int(dir_name.replace("s", ""))
        subject_dir_path = data_folder_path + "/" + dir_name
        subject_images_names = os.listdir(subject_dir_path)
        
       
        for image_name in subject_images_names:
            
            
            if image_name.startswith("."):
                continue;
            
            image_path = subject_dir_path + "/" + image_name

            #read image
            image = cv2.imread(image_path)
            
            #display an image window to show the image 
            #cv2.imshow("Training on image...", cv2.resize(image, (400, 500)))
            #cv2.waitKey(100)
            
            #detect face
            face, rect = detect_face(image)
                        
            if face is not None:
                #add face to list of faces
                faces.append(face)
                #add label for this face
                labels.append(label)
            
    cv2.destroyAllWindows()
    cv2.waitKey(1)
    cv2.destroyAllWindows()
    
    return faces, labels


print("Preparing data...")
faces, labels = prepare_training_data("training-data")
print("Data prepared")

#print total faces and labels
print("Total faces: ", len(faces))
print("Total labels: ", len(labels))


#create our LBPH face recognizer 
#face_recognizer = cv2.createFisherFaceRecognizer()

# for EigenFaceRecognizer  
#face_recognizer = cv2.face.EigenFaceRecognizer_create()

#for FisherFaceRecognizer  
face_recognizer = cv2.face.FisherFaceRecognizer_create()

#train our face recognizer of our training faces with face_vector and label_vector

face_recognizer.train(faces, np.array(labels))


#function to draw rectangle on image according to given (x, y) coordinates and 

def draw_rectangle(img, rect):
    (x, y, w, h) = rect
    cv2.rectangle(img, (x, y), (x+w, y+h), (0, 255, 0), 2)
    
#function to draw text on give image starting from in given x,y coordinate
 
def draw_text(img, text, x, y):
    cv2.putText(img, text, (x, y), cv2.FONT_HERSHEY_PLAIN, 1.5, (0, 255, 0), 2)


#this function recognizes the person in image passed and draws a rectangle around detected face with name of the subject

def predict(test_img):
    #make a copy of the image as we don't want to chang original image
    img = test_img.copy()
    #detect face from the image
    face, rect = detect_face(img)

    #predict the image using our face recognizer 
    label, confidence = face_recognizer.predict(face)
    #get name of respective label returned by face recognizer
    label_text = subjects[label]
    print(label_text)    
    #draw a rectangle around face detected
    draw_rectangle(img, rect)
    #draw name of predicted person
    draw_text(img, label_text, rect[0], rect[1]-5)
    
    return img

print("Predicting images...")

#load test images
test_img1 = cv2.imread("test-data/test9.jpg")
test_img2 = cv2.imread("test-data/test10.jpg")

#perform a prediction
predicted_img1 = predict(test_img1)
predicted_img2 = predict(test_img2)
print("Prediction complete")
print(predicted_img1)
print(predicted_img2)

#display both images
cv2.imshow(subjects[1], cv2.resize(predicted_img1, (400, 500)))
cv2.imshow(subjects[2], cv2.resize(predicted_img1, (400, 500)))
cv2.waitKey(0)
cv2.destroyAllWindows()
cv2.waitKey(1)
cv2.destroyAllWindows()






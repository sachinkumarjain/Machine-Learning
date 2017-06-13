import matplotlib.pyplot as plt
from sklearn import datasets
from sklearn import svm # svm is a machine learning ,it divide the group

digits=datasets.load_digits()

clf=svm.SVC(gamma=0.0001,C=100)   #gamma is used to get correct answer (to get down the moutain example)
print(len(digits.data))
#train the set
x,y=digits.data[:-10], digits.target[:-10]
clf.fit(x,y)
#predict 
print('Prediction:',clf.predict(digits.data[-5]))
#show the image 
plt.imshow(digits.images[-5],cmap=plt.cm.gray_r, interpolation ="nearest")

plt.show()

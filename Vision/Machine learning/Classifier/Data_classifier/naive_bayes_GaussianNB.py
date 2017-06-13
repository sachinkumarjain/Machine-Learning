import numpy as np
from sklearn import  cross_validation
from sklearn.naive_bayes import GaussianNB
import pandas as pd

df = pd.read_csv('breast-cancer-wisconsin.data.txt')
df.replace('?',-99999, inplace=True)
df.drop(['id'], 1, inplace=True)

X = np.array(df.drop(['class'], 1)) 
y = np.array(df['class'])

X_train, X_test, y_train, y_test = cross_validation.train_test_split(X, y, test_size=0.2)

clf = GaussianNB()
clf.fit(X_train, y_train)
accuracy = clf.score(X_test, y_test)
print(accuracy)

'''
This is especially useful when the whole dataset is too big to fit in memory at once.
This method has some performance and numerical stability overhead,
hence it is better to call partial_fit on chunks of data that are as large as possible
(as long as fitting in the memory budget) to hide the overhead.
'''


clf_pf=GaussianNB()
clf_pf.partial_fit(X_train, y_train,np.unique(y_train))
accuracy1 = clf_pf.score(X_test, y_test)
print(accuracy1)

example_measures = np.array([[4,2,1,1,1,2,3,2,1],[4,2,1,1,1,2,3,2,1]])
example_measures = example_measures.reshape(len(example_measures), -1)
prediction = clf.predict(example_measures)
prediction1 = clf_pf.predict(example_measures)
                                
print(prediction)
print(prediction1)

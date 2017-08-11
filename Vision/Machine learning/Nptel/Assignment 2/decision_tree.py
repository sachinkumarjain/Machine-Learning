import numpy as np
from sklearn import tree
from sklearn.model_selection import cross_val_score

f=open('decision_tree_data.txt','r')
X=[]
Y =[]

for line in f:
    line =np.asarray(line.split(),dtype=np.float32)
    X.append(line[:-1])
    Y.append(line[-1])


X =np.asmatrix(X)
Y =np.reshape(Y,(len(Y),1))

clf = tree.DecisionTreeClassifier(random_state=0)

clf.fit(X,Y);
print(clf.decision_path(X,check_input=True))

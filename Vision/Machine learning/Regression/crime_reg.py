import pandas as pd
import numpy as np
from sklearn import linear_model
from sklearn.cross_validation import train_test_split
import matplotlib.pyplot as plt

df =pd.read_csv('communities.data.txt', header=None)

df.replace('?',-99999, inplace=True)

df_x = np.array(df.drop(df.columns[[df.shape[1]-1]], axis=1))
df_y=np.array(df[[df.shape[1]-1]])
#print(df_y)



reg=linear_model.LinearRegression()
x_train,x_test,y_train,y_test=train_test_split(df_x,df_y,test_size=0.2,random_state=4)
reg.fit(x_train,y_train)
#print(reg.coef_)
#a=reg.predict(x_test)
print(reg.intercept_)
print(reg.coef_.shape)
#print(a)
print("R2 score",reg.score(x_test,y_test))
#plt.scatter(x_train,y_train)
#plt.show()


import numpy as np
from sklearn import preprocessing,svm,cross_validation
import pandas as pd
from sklearn.ensemble import RandomForestClassifier

 
df_train = pd.read_csv('train.csv')
df_test = pd.read_csv('test.csv')
		 
df_train.drop(['Cabin'], 1, inplace=True)
df_train.convert_objects(convert_numeric=True)
df_train['Age'].fillna(df_train['Age'].median(), inplace=True)

df_test.drop(['Cabin'], 1, inplace=True)
df_test.convert_objects(convert_numeric=True)
df_test['Age'].fillna(df_test['Age'].median(), inplace=True)


def handle_non_numerical_data(df):
    columns = df.columns.values
    for column in columns:
        text_digit_vals = {}
        def convert_to_int(val):
            return text_digit_vals[val]
         # checking column wise if it has a number or not 
        if df[column].dtype != np.int64 and df[column].dtype != np.float64:
            column_contents = df[column].values.tolist()
            unique_elements = set(column_contents)
            x = 0
            for unique in unique_elements:
                if unique not in text_digit_vals:
                    text_digit_vals[unique] = x
                    x+=1

            df[column] = list(map(convert_to_int, df[column]))

    return df


df_ntrain = handle_non_numerical_data(df_train)
df_ntest = handle_non_numerical_data(df_test)
#print(df_ntrain)

X = np.array(df_ntrain.drop(['Survived'],1).astype(float))
X = preprocessing.scale(X)
y = np.array(df_ntrain['Survived'])

X_train, X_test, y_train, y_test = cross_validation.train_test_split(X, y, test_size=0.2)

clf=RandomForestClassifier(n_estimators=33,)
clf.fit(X_train, y_train)
accuracy = clf.score(X_test, y_test)
print(accuracy)

'''
X_test = np.array(df_ntest.astype(float))
X_test = preprocessing.scale(X_test)
print(clf.predict(X_test))
'''

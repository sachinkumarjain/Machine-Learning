import numpy as np
from sklearn import preprocessing,cross_validation
import pandas as pd
from sklearn.neural_network import MLPClassifier

# Reading csv dataset 
df_train = pd.read_csv('train.csv')
df_test = pd.read_csv('test.csv')

# Dropping and filling null with 0 in train set		 
df_train.drop(['Cabin'], 1, inplace=True)
x = df_train["Age"].mean()
#print(x)
df_train.convert_objects(convert_numeric=True)
df_train.fillna(x, inplace=True)

# Dropping and filling null with 0 in test set
df_test.drop(['Cabin'], 1, inplace=True)
df_test.convert_objects(convert_numeric=True)
df_test.fillna(x, inplace=True)

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

# Has datset has non-numerical data we are connecting it to numeric	
df_ntrain = handle_non_numerical_data(df_train)
df_ntest = handle_non_numerical_data(df_test)
#print(df_ntrain)

#df_ntrain.drop(['Sex'], 1, inplace=True)
#print(df_ntrain.head())
X = np.array(df_ntrain.drop(['Survived'],1).astype(float))
X = preprocessing.scale(X)
y = np.array(df_ntrain['Survived'])
X_train, X_test, y_train, y_test = cross_validation.train_test_split(X, y, test_size=0.2)
# 5,2 ->85% output remains same , 40, ->99% output varies

clf = MLPClassifier(solver='sgd', alpha=1e-5,hidden_layer_sizes=(),
                    random_state=1)
clf.fit(X_train, y_train)
accuracy = clf.score(X_test, y_test)
print(accuracy)

X_test = np.array(df_ntest.astype(float))
X_test = preprocessing.scale(X_test)
print(clf.predict(X_test))


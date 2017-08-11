import pandas as pd
import numpy as np
from sklearn import linear_model
from sklearn.cross_validation import train_test_split
from sklearn.decomposition import PCA

df_train = pd.read_csv('train.csv')
df_test = pd.read_csv('test.csv')

df_train.drop(df_train.columns[0], axis =1,inplace =True)
df_test.drop(df_test.columns[0], axis =1,inplace = True)


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
	
X = np.array(df_train.drop(['y'], 1)) 
y = np.array(df_train['y'])  

X_test = np.array(df_test)

#reg=linear_model.RidgeCV(alphas=[0.1, 1.0, 10.0,100.0,0.001,0.01])
#reg=linear_model.ElasticNet(alpha=0.01)
reg=linear_model.Lasso(alpha = 0.01)
#reg=RandomForestRegressor(n_estimators=20,max_features=100)

x_train,x_test,y_train,y_test=train_test_split(X,y,test_size=0.2,random_state=1)

reg.fit(x_train,y_train)
#print(reg.coef_)
print("R2 score",reg.score(x_test,y_test))


output = reg.predict(X_test)
df_output = pd.DataFrame()
#df_output['ID'] = X_test['ID']
df_output['y'] = output
df_output[['y']].to_csv('output_pca.csv',index=False)

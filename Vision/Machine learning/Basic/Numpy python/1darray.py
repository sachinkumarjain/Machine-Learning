import numpy as np

my=np.array([0,1,2,3])
#print(my)

#converting int to string using astype
'''
str_array=my.astype(str)
print(str_array)
'''
#numpy is used for muilt-dimensional array

dim2=np.array([[0,1,2,3],[1,2,3,4],[9,8,7,6]])
'''print(dim2.ndim)
print(dim2.shape)
print(dim2.size)
'''

#mathmatical
#print(np.mean(dim2))
# avg by col axis =0 axis=1 row wise
#print(np.mean(dim2,axis=0))

'''
#indices row and column
print(dim2[:]) # print entire row
print(dim2[:2,:]) # first two row and all column
print(dim2[0][::-1]) # reverse the matrix first row        
'''
#save the array in excel for easy visibility

np.savetxt('dim2array.csv',dim2,delimiter=',')
#open dim2array.csv to open file from cmd
print(np.genfromtxt('dim2array.csv',delimiter=','))



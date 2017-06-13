import matplotlib.pyplot as plt
import numpy as np

names=np.array([1,2,3])
y=np.array([9,8,7])
x1=[1,2,3]
y2=[9,11,10]

#lines

plt.plot(names,y,label='First line')
plt.plot(x1,y2,label='Second line')
plt.legend()

#bar

plt.bar(names,y,label='Barsl',color='c')
plt.bar(x1,y2,label='Bars2',color='b')

#points by default point look like circle
plt.scatter(x1,y2,label='hi',color='r',marker='*',s=100)  # label are used to display the variable name in the plot inside the box

#pie
slices = [2,3,8]
activities = ['y','x1','y2']
cols=['c','m','k','b']
plt.pie(slices,
        labels=activities,
        colors=cols,
        startangle=90,
        shadow=True,
        explode=(0,0.1,0),
        autopct='%1.lf%%')
        
plt.xlabel('x')
plt.ylabel('y')
plt.title('super')
plt.show()

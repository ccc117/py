'''xlist=[]
for i in range(5):
    x=input("값을 입력: ")
    xlist.append(x)

print(xlist)

for i in range(len(xlist)):
    xlist[i]=int(xlist[i])

print(xlist)

xlist=[int(i) for i in xlist]
print(xlist)
xeven=[]
for i in xlist:
    if i%2==0:
        xeven.append(i)
print(xeven)
xodd=[i for i in xlist if i%2==1]
print(xodd)
'''
'''xeven=[]

while True:
    x=int(input('값을 입력: '))
    if x<0:break
    if x%2==1:continue
    xeven.append(x)

print(xeven)'''
dt={'a':1,'b':2,'c':3}
for i in dt:
    print(i)

for key, value in dt.items():
    print(key, value)
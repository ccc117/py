lst=[10,40,30]
tp=('a','b','c')
dt=dict(zip(tp,lst))
print(dt)
for i in lst :
    print(i,end=' ')
print()
for i in tp :
    print(i,end=' ')
print()
for i in dt :
    print(i, "=>", dt[i])
print(len(dt))
print(max(lst))
print(max(tp))
print(max(dt))
print(min(lst))
print(min(tp))
print(min(dt))
print(sum(lst))
#print(sum(tp))안됨
print(sorted(lst))
print(lst)
lst.sort()
print(lst)







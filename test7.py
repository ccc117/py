'''f=open('test.txt','a',encoding='utf 8')
indata=input('입력하세요.')
f.write(indata+'\n')
f.close'''
f=open('test.txt','r',encoding='utf 8')
'''datal=f.read()
print(datal)'''
'''data2=f.readline()
print(data2)'''
data3=f.readlines()
#print(data3)
'''for line in data3:
    line=line.replace('\n',' ')
    print(line)'''
for i in range(len(data3)):
    data3[i]=data3[i].replace('\n',' ')
for line in data3:
    print(line)
print(data3)
f.close
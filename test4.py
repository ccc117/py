'''def gugu(dan):
    for i in range(1,10):
        print(dan,"x",i,"=",(dan*i))
dan=int(input('단을 입력하세요: '))
gugu(dan)
'''
def oneInput(msg):
    num=float(input(msg))
    return num
def twoInput():
    width=float(input('가로 입력: '))
    height=float(input('세로 입력: '))
    return width, height
'''def rectArea():
    rlist.append(width*height)
rlist=[]
width,height=twoInput()
rectArea()
print(rlist)'''
def rectArea():
    rect=[]
    '''global cnt
    cnt+=1
    rect.append(cnt)'''
    if width==height:rect.append('정사각형')
    else:rect.append('직사각형')
    rect.append(width)
    rect.append(height)
    rect.append(width*height)
    return rect
rlist=[]
cnt=0
width,height=twoInput()
rlist.append(rectArea())
width,height=twoInput()
rlist.append(rectArea())
print(rlist)
while(True):
    width,height=twoInput()
    if width<=0 or height<=0:break
    rlist.append(rectArea())
print(rlist)

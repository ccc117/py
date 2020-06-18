xlist=[]

while True:
    x=int(input('값을 입력: '))
    if x<0:break
    if 2<=x<=9:
        print(x,'단')
        for i in range(9):
            print(f'{x} X {i+1}={x*(i+1)}')
    else:
        print('입력오류')
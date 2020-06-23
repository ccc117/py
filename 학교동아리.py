#20619 최찬웅
x=int(input('알고 싶은 구구단 입력: '))
for i in range(9):
    print(f'{x} X {i+1} = {x*(i+1)}')





y=int(input('가지고 있는 돈 입력: '))
if y>=50000:
    print('저스트 댄스 사야지')
elif 50000>y>=30000:
    print('치밥 사먹어야지')
elif 30000>y>=10000:
    print('롤에 질러야지')
else:
    print('걍 집에서 잠이나 자야지')
    
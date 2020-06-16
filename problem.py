y='코로나. 코로나. 코로나.'
x=str(input('찾고자하는 단어를 입력하세요.: '))
print(f'{x}는 ',y.count(x),'번 언급되었습니다.')
print('이 문장은 ',y.count('.'),'개의 문장으로 이루어져있습니다.')
print(y.split('.'))
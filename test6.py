#홀수
def is_odd(n):
    if n %2==1 : return True
    else : return False

#짝수
def is_even(n):
    if n %2==0 : return True
    else : return False

#소수

def is_prime(n):
    for i in range(2,n):
        if n%i==0: return False 
    return True

#완전수
def is_perfect(n):
    sum=0
    for i in range(1,n):
        if n %i==0 : sum=sum+i
    if sum==n: return True
    else: return False

if __name__=='__main__':
    n= int(input('숫자입력: '))
    if is_odd(n) :
        print('홀수')
    if is_even(n) :
        print('짝수')
    if is_prime(n) :
        print('소수')
    if is_perfect(n) :
        print('완전수')

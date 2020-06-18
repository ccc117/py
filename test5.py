def STKG(m,gender):
    if gender==1:
        stkg=m*m*22
    else:
        stkg=m**2*21
    return stkg
def BMI(kg,m):
    bmi=kg/m**2
    return bmi
def BMICHECK(bmi):
    if bmi<18.5: bmicheck='저체중'
    elif bmi<25 : bmicheck='정상'
    else : bmicheck='비만'
    return bmicheck
while True:
    kg=float(input('체중(kg)입력: '))
    cm=float(input('키(cm)입력: '))
    gender=int(input('성별입력(남1,여2): '))
    m=cm/100
    stkg=STKG(m,gender)
    bmi=BMI(kg,m)
    bmicheck = BMICHECK(bmi)

    print(cm, kg)
    if gender==1:
        print('남자')
    else: print('여자')
    print(bmi, bmicheck)
    ans=print('계속하시겠습니까?: y or n')
    if ans.lower() !='y':break
from cs50 import get_int

def judge(n, l):
    n1 = str(n)
    s = 0

    for i in range(l):
        if i % 2 == 0:
            s += n % 10
        else:
            temp = 2 * (n % 10)
            s += temp // 10 + temp % 10
        n = n // 10
#    print(s)
    return s % 10 == 0
'''
    for i in range(1, l, 2):
        print(n1[i])
        temp = 2 * int(n1[i])
        s += temp // 10 + temp % 10

    for i in range(0, l, 2):
        print(n1[i])
        s += int(n1[i])
'''

while(1):
    n = get_int("Number: ")
    if(n >= 0):
        break

n1 = str(n)
l = len(n1)

if (l == 13 or l == 16) and n1[0] == '4' and judge(n, l):
    print("VISA")
elif (l == 15) and (n1[:2] == "34" or n1[:2] == "37") and judge(n, l):
    print("AMEX")
elif (l == 16) and (int(n1[:2]) >= 51 and int(n1[:2]) <= 55) and judge(n, l):
    print("MASTERCARD")
else:
    print("INVALID")



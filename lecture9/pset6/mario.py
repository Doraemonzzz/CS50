from cs50 import get_int

def printmario(n):
    for i in range(n):
        s = ""
        for j in range(n-i-1):
            s += " "
        for j in range(i+1):
            s += "#"
        s += "  "
        for j in range(i+1):
            s += "#"
        print(s)

while(True):
    n = get_int("Height: ")
    if (n >= 0 and n <= 23):
        break
printmario(n)


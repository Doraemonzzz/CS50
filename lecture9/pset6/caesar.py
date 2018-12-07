import sys
from cs50 import get_string

def caesar(word, k):
    n = len(word)
    result = ""
    for i in range(n):
        if word[i].islower():
            num = ord('a') + (ord(word[i]) - ord('a') + k) % 26
            result += chr(num)
        elif word[i].isupper():
            num = ord('A') + (ord(word[i]) - ord('A') + k) % 26
#            print(num)
            result += chr(num)
        else:
            result += word[i]
    return result

s= len(sys.argv)
k = -1
if(s != 2):
    print("Usage: ./caesar k");
else:
    k = int(sys.argv[1])

if(k > 0):
    word1 = get_string("plaintext:  ")
    #print(word1)
    word2 = caesar(word1, k)
    print(f"ciphertext: {word2}")
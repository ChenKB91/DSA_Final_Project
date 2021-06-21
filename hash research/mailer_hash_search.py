from string import ascii_lowercase
import numpy as np
import time

start = time.time()
primes = list()
all_mailer = list()

with open(f"mailer_unique.txt", "r") as file:
    mailer_data = file.read()
    all_mailer = mailer_data.split("\n")

with open("primes.txt", "r") as file:
    lines = file.read()
    primes = [int(i) for i in lines.split()]
print("done reading!")

mult_list = primes[:20]
rem = 1000007
rem = 100003

def myHash(mult, rem, s):
    h = 0
    for c in s:
        h = (mult*h+ord(c))%rem
    return h

def evalParameter(mult, rem):
    table = [None]*rem
    for m in all_mailer:
        idx = myHash(mult, rem, m)
        if table[idx] and m != table[idx]:
            return
        else:
            table[idx] = m
    print(f"{mult=} {rem=} doesn't have any collision!")

for mult in mult_list:
    evalParameter(mult, rem)

end = time.time()
print(f"{end-start:.3f}s")

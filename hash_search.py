from string import ascii_lowercase
import numpy as np
import time

start = time.time()
chars = list(ascii_lowercase)
chars = chars + [str(i) for i in range(10)]
primes = list()
all_token = set()
for c in chars:
    with open(f"token_split_into_prefix/{c}.txt", "r") as file:
        tks = file.readlines()
        for t in tks:
            all_token.add(t[:-1])

with open("primes.txt", "r") as file:
    lines = file.read()
    primes = [int(i) for i in lines.split()]
print("done reading!")

all_token = sorted(list(all_token))

# print(primes[10000])
# exit()

mult_list = primes[:10000]
# mult_list = [8923]
rem = 10000019
threshold = 400

def myHash(mult, rem, s):
    h = 0
    for c in s:
        h = (mult*h+ord(c))%rem
    return h

def evalParameter(mult, rem):
    trans = [
        -1, -1, -1, -1, -1, -1, -1, -1,  -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1,  -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1,  -1, -1, -1, -1, -1, -1, -1, -1,
        0,  4,  1,  1,  1,  1,  0,  0,   0,  0, -1, -1, -1, -1, -1, -1,
        -1, 12, 11, 15,  9,  5,  5,  7,   7,  4,  1,  6,  6, 14,  3,  2,
        13,  0,  8, 16, 10,  2,  2,  3,   0,  0,  0, -1, -1, -1, -1, -1,
        -1, 12, 11, 15,  9,  5,  5,  7,   7,  4,  1,  6,  6, 14,  3,  2,
        13,  0,  8, 16, 10,  2,  2,  3,   0,  0,  0, -1, -1, -1, -1, -1
    ]
    # table = np.zeros((17, rem), dtype=np.int8)
    table = list()
    for _ in range(17):
        table.append([None]*rem)
    cnt = 0
    for t in all_token:
        idx = myHash(mult, rem, t)
        table_no = trans[ord(t[0])]
        if table[table_no][idx] and t != table[table_no][idx]:
            cnt += 1
            if cnt > threshold:
                return
            # print(t, table[table_no][idx], table_no)
        else:
            table[table_no][idx] = t
    
    print(f"{mult=} {rem=} {cnt=}")

paras = list()
for mult in mult_list:
    # for rem in rem_list:
    evalParameter(mult, rem)

end = time.time()
print(f"{end-start:.3f}s")

col_pairs = set()
while True:
    try:
        s = input()
        s1 = s.split()[0]
        s2 = s.split()[1]
        col_pairs.add(s1)
        col_pairs.add(s2)
    except:
        break
print(len(col_pairs))
# print(col_pairs)
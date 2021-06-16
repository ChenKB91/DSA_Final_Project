from string import ascii_lowercase

chars = list(ascii_lowercase)
chars = chars + [str(i) for i in range(10)]
# print(c)

count = dict()

for c in chars:
    with open(f"{c}.txt", "r") as file:
        tokens = file.readlines()
        unique = set(tokens)
        count[c] = len(unique)

for k, v in count.items():
    print(f"{k}, {v}")

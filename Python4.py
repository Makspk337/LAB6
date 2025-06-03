from itertools import product


parts = input().strip().split()
while len(parts) != 2 or not all(part.isdigit() for part in parts):
    print("Введите два целых числа через пробел.")
    parts = input().strip().split()

N, K = map(int, parts)
while N < 2 or N > 10 or K < 2 or K > 6:
    print("Число букв должно быть от 2 до 10, а длина слов — от 2 до 6.")
    parts = input().strip().split()
    N, K = map(int, parts)

codes = []
for i in range(N):

    code = input().strip()
    while len(code) < 1 or len(code) > 5:
        print("Шифр должен содержать от 1 до 5 цифр.")
        code = input().strip()

    for c in code:
        while c not in '123456789':
            print("Допустимы только цифры от 1 до 9.")
        code = input().strip()

    codes.append(code)

encrypted_words = {}

for indices in product(range(N), repeat=K):
    encrypted = ''.join(codes[i] for i in indices)
    encrypted_words[encrypted] = encrypted_words.get(encrypted, 0) + 1

unique_count = sum(1 for count in encrypted_words.values() if count == 1)
print(unique_count)

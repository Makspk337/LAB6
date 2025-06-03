import random

def count_mismatches(grid, pattern):
    mismatches = 0
    for i in range(len(grid)):
        for j in range(len(grid[0])):
            if grid[i][j] != pattern[i][j]:
                mismatches += 1
    return mismatches

def generate_random_grid(n, m):
    return [[random.randint(0, 1) for _ in range(m)] for _ in range(n)]

def print_grid(grid):
    for row in grid:
        print(' '.join(str(cell) for cell in row))

def min_moves_to_chessboard(grid):
    n, m = len(grid), len(grid[0])

    pattern1 = [[(i + j) % 2 for j in range(m)] for i in range(n)]
    pattern2 = [[1 - (i + j) % 2 for j in range(m)] for i in range(n)]

    mismatch1 = count_mismatches(grid, pattern1)
    mismatch2 = count_mismatches(grid, pattern2)

    result = float('inf')
    if mismatch1 % 2 == 0:
        result = mismatch1 // 2
    if mismatch2 % 2 == 0:
        result = min(result, mismatch2 // 2)

    return result if result != float('inf') else -1

size = input("Введите размеры щита: ").split()
while len(size) != 2 or not all(x.isdigit() for x in size):
    print("Введите два положительных целых числа.")
    size = input("Введите размеры щита: ").split()

N, M = map(int, size)
while N <= 0 or M <= 0:
    print("Размеры должны быть положительными целыми числами.")
    size = input("Введите размеры щита: ").split()
    N, M = map(int, size)

grid = generate_random_grid(N, M)

print("\nИсходная матрица:")
print_grid(grid)

moves = min_moves_to_chessboard(grid)
if moves == -1:
    print("\nНевозможно привести к шахматному виду.")
else:
    print(f"\nМинимальное число ходов: {moves}")

import random
from typing import List, Tuple, Dict


def matrix_sequence(M: int, N: int) -> None:
    print("\nМатрица с возрастающей последовательностью:")
    matrix = [[random.randint(0, 100) for _ in range(N)] for _ in range(M)]

    for row in matrix:
        print("\t".join(map(str, row)))

    best_seq = []
    min_start = 101

    for row in matrix:
        curr_seq = [row[0]]
        for num in row[1:]:
            if num > curr_seq[-1]:
                curr_seq.append(num)
            else:
                if curr_seq[0] < min_start or (curr_seq[0] == min_start and len(curr_seq) > len(best_seq)):
                    min_start = curr_seq[0]
                    best_seq = curr_seq.copy()
                curr_seq = [num]

        if curr_seq[0] < min_start or (curr_seq[0] == min_start and len(curr_seq) > len(best_seq)):
            min_start = curr_seq[0]
            best_seq = curr_seq.copy()

    print("\nНаилучшая возрастающая последовательность:", " ".join(map(str, best_seq)))


def matrix_unique(M: int, N: int) -> None:
    print("\nМатрица с уникальными элементами:")
    matrix = [[random.randint(-50, 50) for _ in range(N)] for _ in range(M)]
    freq = {}

    for row in matrix:
        print("\t".join(map(str, row)))
        for num in row:
            freq[num] = freq.get(num, 0) + 1

    print("\nКоличество повторений элементов:")
    for num, count in sorted(freq.items()):
        print(f"{num}: {count}")


SIZE = 8


def is_queen_attacking(queen_pos: Tuple[int, int], king_pos: Tuple[int, int]) -> bool:
    queen_x, queen_y = queen_pos
    king_x, king_y = king_pos
    return (queen_x == king_x or queen_y == king_y or
            abs(queen_x - king_x) == abs(queen_y - king_y))


def chess_board() -> None:
    board = [['.' for _ in range(SIZE)] for _ in range(SIZE)]
    queens = []
    random.seed()

    king_x, king_y = random.randint(0, SIZE - 1), random.randint(0, SIZE - 1)
    board[king_x][king_y] = 'K'
    print(f"Король расположен на: ({king_x}, {king_y})")

    num_queens = random.randint(2, 5)
    for _ in range(num_queens):
        while True:
            x, y = random.randint(0, SIZE - 1), random.randint(0, SIZE - 1)
            if board[x][y] == '.':
                board[x][y] = 'Q'
                queens.append((x, y))
                break

    print("\nШахматная доска:")
    for row in board:
        print(" ".join(row))

    print("\nФерзи, атакующие короля:")
    attacking_queens = [q for q in queens if is_queen_attacking(q, (king_x, king_y))]

    if attacking_queens:
        for q in attacking_queens:
            print(f"Ферзь на ({q[0]}, {q[1]})")
    else:
        print("Нет ферзей, атакующих короля.")


try:
    M = int(input("Введите количество строк: "))
    N = int(input("Введите количество столбцов: "))

    if M < 2 or M > 10 or N < 2 or N > 10:
        print("Размеры матрицы должны быть от 2 до 10")
    else:
        random.seed()
        matrix_sequence(M, N)
        matrix_unique(M, N)
        chess_board()
except ValueError:
    print("Ошибка: введите целые числа")

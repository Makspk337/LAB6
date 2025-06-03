import random
import time
from collections import Counter

def matrix_sequence(M, N):
    print("\nМатрица с возрастающей последовательностью:")
    matrix = [[random.randint(0, 100) for _ in range(N)] for _ in range(M)]
    for row in matrix:
        print("\t".join(map(str, row)))

    best_seq = []
    min_start = 101

    for row in matrix:
        curr_seq = [row[0]]
        for j in range(1, N):
            if row[j] > curr_seq[-1]:
                curr_seq.append(row[j])
            else:
                if curr_seq[0] < min_start or (curr_seq[0] == min_start and len(curr_seq) > len(best_seq)):
                    best_seq = curr_seq[:]
                    min_start = curr_seq[0]
                curr_seq = [row[j]]
        if curr_seq[0] < min_start or (curr_seq[0] == min_start and len(curr_seq) > len(best_seq)):
            best_seq = curr_seq[:]
            min_start = curr_seq[0]

    print("\nНаилучшая возрастающая последовательность:", " ".join(map(str, best_seq)))

def matrix_unuque(M, N):
    print("\nМатрица с уникальными элементами:")
    matrix = [[random.randint(-50, 50) for _ in range(N)] for _ in range(M)]
    for row in matrix:
        print("\t".join(map(str, row)))

    flat = [elem for row in matrix for elem in row]
    freq = Counter(flat)

    print("\nКоличество повторений элементов:")
    for value, count in sorted(freq.items()):
        print(f"{value}: {count}")


SIZE = 8

def is_attacking_king(piece, king_x, king_y, board):
    x, y = piece['x'], piece['y']
    t = piece['type']
    if t == 'Q':
        directions = [(0,1), (1,0), (1,1), (-1,1), (-1,0), (0,-1), (1,-1), (-1,-1)]
        for dx, dy in directions:
            nx, ny = x + dx, y + dy
            while 0 <= nx < SIZE and 0 <= ny < SIZE:
                if board[nx][ny] != '.' and (nx != king_x or ny != king_y):
                    break
                if nx == king_x and ny == king_y:
                    return True
                nx += dx
                ny += dy
    elif t == 'N':
        moves = [(-2,1), (-1,2), (1,2), (2,1), (2,-1), (1,-2), (-1,-2), (-2,-1)]
        for dx, dy in moves:
            nx, ny = x + dx, y + dy
            if 0 <= nx < SIZE and 0 <= ny < SIZE and nx == king_x and ny == king_y:
                return True
    return False

def chess_board():
    board = [['.' for _ in range(SIZE)] for _ in range(SIZE)]
    pieces = []

    king_x = random.randint(0, SIZE-1)
    king_y = random.randint(0, SIZE-1)
    board[king_x][king_y] = 'K'

    num_queens = 4 + random.randint(0, 4)
    num_knights = 2 + random.randint(0, 4)

    total_pieces = num_queens + num_knights
    types = ['Q'] * num_queens + ['N'] * num_knights

    for t in types:
        while True:
            x, y = random.randint(0, SIZE-1), random.randint(0, SIZE-1)
            if board[x][y] == '.':
                board[x][y] = t
                pieces.append({'type': t, 'x': x, 'y': y})
                break

    print("\nШахматная доска:")
    for row in board:
        print(" ".join(row))

    print("\nФигуры, атакующие короля:")
    for piece in pieces:
        if is_attacking_king(piece, king_x, king_y, board):
            name = "Ферзь" if piece['type'] == 'Q' else "Конь"
            print(f"{name} на ({piece['x']},{piece['y']})")


M = int(input("Введите количество строк: "))
N = int(input("Введите количество столбцов: "))

random.seed(time.time())
matrix_sequence(M, N)
matrix_unuque(M, N)
chess_board()

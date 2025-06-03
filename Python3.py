n = 4

def lu_decompose(A):
    L = [[0.0] * n for _ in range(n)]
    U = [[0.0] * n for _ in range(n)]

    for i in range(n):
        for j in range(i, n):
            sum_ = 0
            for k in range(i):
                sum_ += L[i][k] * U[k][j]
            U[i][j] = A[i][j] - sum_

        if abs(U[i][i]) < 1e-15:
            print("Нулевой элемент на диагонали U в позиции", i)
            return None, None

        for j in range(i, n):
            if i == j:
                L[i][i] = 1.0
            else:
                sum_ = 0
                for k in range(i):
                    sum_ += L[j][k] * U[k][i]
                L[j][i] = (A[j][i] - sum_) / U[i][i]
    return L, U


def forward_substitution(L, b):
    y = [0.0] * n
    for i in range(n):
        sum_ = 0
        for j in range(i):
            sum_ += L[i][j] * y[j]
        y[i] = b[i] - sum_
    return y


def backward_substitution(U, y):
    x = [0.0] * n
    for i in reversed(range(n)):
        sum_ = 0
        for j in range(i + 1, n):
            sum_ += U[i][j] * x[j]
        x[i] = (y[i] - sum_) / U[i][i]
    return x


M = -0.77
N = 0.16
P = 1.12

A = [
    [M, -0.04, 0.21, -18],
    [0.25, -1.23, N, -0.09],
    [-0.21, N, 0.8, -0.13],
    [0.15, -1.31, 0.06, P]
]

b = [-1.24, P, 2.56, M]

L, U = lu_decompose(A)
while L is None or U is None:
    print("LU-разложение не удалось.")
    L, U = lu_decompose(A)


print("Матрица L:")
for row in L:
    print(" ".join(f"{v:10.6f}" for v in row))

print("\nМатрица U:")
for row in U:
    print(" ".join(f"{v:10.6f}" for v in row))

y = forward_substitution(L, b)
print("\nРезультат прямого хода (вектор y):")
for i, val in enumerate(y, 1):
    print(f"y[{i}] = {val:.6f}")

x = backward_substitution(U, y)
print("\nРезультат обратного хода (решение x):")
for i, val in enumerate(x, 1):
    print(f"x[{i}] = {val:.6f}")

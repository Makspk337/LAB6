#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>
using namespace std;

void matrix_sequence(int M, int N) {
    cout << "\nМатрица с возрастающей последовательностью:\n";
    vector<vector<int>> matrix(M, vector<int>(N));
    srand(time(0));

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = rand() % 101;
            cout << matrix[i][j] << "\t";
        }
        cout << endl;
    }

    vector<int> bestSeq;
    int minStart = 101;

    for (int i = 0; i < M; i++) {
        vector<int> currSeq = {matrix[i][0]};
        for (int j = 1; j < N; j++) {
            if (matrix[i][j] > currSeq.back()) {
                currSeq.push_back(matrix[i][j]);
            } else {
                if (currSeq[0] < minStart || (currSeq[0] == minStart && currSeq.size() > bestSeq.size())) {
                    minStart = currSeq[0];
                    bestSeq = currSeq;
                }
                currSeq = {matrix[i][j]};
            }
        }
        if (currSeq[0] < minStart || (currSeq[0] == minStart && currSeq.size() > bestSeq.size())) {
            minStart = currSeq[0];
            bestSeq = currSeq;
        }
    }

    cout << "\nНаилучшая возрастающая последовательность: ";
    for (int x : bestSeq) cout << x << " ";
    cout << endl;
}

void matrix_unuque(int M, int N) {
    cout << "\nМатрица с уникальными элементами:\n";
    vector<vector<int>> matrix(M, vector<int>(N));
    map<int, int> freq;
    srand(time(0) + 1);

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = rand() % 101 - 50;
            freq[matrix[i][j]]++;
            cout << matrix[i][j] << "\t";
        }
        cout << endl;
    }

    cout << "\nКоличество повторений элементов:\n";
    for (auto& pair : freq) {
        cout << pair.first << ": " << pair.second << endl;
    }
}

const int SIZE = 8;

bool isQueenAttacking(int queenX, int queenY, int kingX, int kingY) {
    return (queenX == kingX || queenY == kingY ||
           abs(queenX - kingX) == abs(queenY - kingY));
}

void chess_board() {
    vector<vector<char>> board(SIZE, vector<char>(SIZE, '.'));
    vector<pair<int, int>> queens;
    srand(time(0) + 2);

    int kingX = rand() % SIZE;
    int kingY = rand() % SIZE;
    board[kingX][kingY] = 'K';
    cout << "Король расположен на: (" << kingX << ", " << kingY << ")\n";

    int numQueens = 2 + rand() % 4;
    for (int i = 0; i < numQueens; i++) {
        int x, y;
        do {
            x = rand() % SIZE;
            y = rand() % SIZE;
        } while (board[x][y] != '.');
        board[x][y] = 'Q';
        queens.emplace_back(x, y);
    }

    cout << "\nШахматная доска:\n";
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j)
            cout << board[i][j] << " ";
        cout << endl;
    }

    cout << "\nФерзи, атакующие короля:\n";
    bool found = false;
    for (auto &queen : queens) {
        if (isQueenAttacking(queen.first, queen.second, kingX, kingY)) {
            cout << "Ферзь на (" << queen.first << "," << queen.second << ")\n";
            found = true;
        }
    }
    
    if (!found) {
        cout << "Нет ферзей, атакующих короля.\n";
    }
}

int main() {
    int M, N;
    cout << "Введите количество строк: ";
    cin >> M;
    cout << "Введите количество столбцов: ";
    cin >> N;
    
    matrix_sequence(M, N);
    matrix_unuque(M, N);
    chess_board();

    return 0;
}


#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>
using namespace std;

struct Piece {
    char type;
    int x, y;
};

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

int SIZE = 8;

bool isAttackingKing(const Piece& piece, int kingX, int kingY, const vector<vector<char>>& board) {
    if (piece.type == 'Q') {
        int dx[] = {0, 1, 1, -1, -1, 0, 1, -1};
        int dy[] = {1, 0, 1, 1, 0, -1, -1, -1};
        for (int dir = 0; dir < 8; ++dir) {
            int nx = piece.x + dx[dir], ny = piece.y + dy[dir];
            while (nx >= 0 && nx < SIZE && ny >= 0 && ny < SIZE) {
                if (board[nx][ny] != '.' && !(nx == kingX && ny == kingY)) break;
                if (nx == kingX && ny == kingY) return true;
                nx += dx[dir];
                ny += dy[dir];
            }
        }
    } else if (piece.type == 'N') {
        int dx[] = {-2, -1, 1, 2, 2, 1, -1, -2};
        int dy[] = {1, 2, 2, 1, -1, -2, -2, -1};
        for (int i = 0; i < 8; ++i) {
            int nx = piece.x + dx[i], ny = piece.y + dy[i];
            if (nx == kingX && ny == kingY)
                return true;
        }
    }
    return false;
}

void chess_board() {
    vector<vector<char>> board(SIZE, vector<char>(SIZE, '.'));
    vector<Piece> pieces;
    srand(time(0) + 2);

    int kingX = rand() % SIZE;
    int kingY = rand() % SIZE;
    board[kingX][kingY] = 'K';

    int numQueens = 4 + rand() % 5;
    int numKnights = 2 + rand() % 5;

    for (int i = 0; i < numQueens + numKnights; i++) {
        char type = (i < numQueens) ? 'Q' : 'N';
        int x, y;
        do {
            x = rand() % SIZE;
            y = rand() % SIZE;
        } while (board[x][y] != '.');
        board[x][y] = type;
        pieces.push_back({type, x, y});
    }

    cout << "\nШахматная доска:\n";
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j)
            cout << board[i][j] << " ";
        cout << endl;
    }

    cout << "\nФигуры, атакующие короля:\n";
    for (auto& piece : pieces) {
        if (isAttackingKing(piece, kingX, kingY, board)) {
            string name;
            if (piece.type == 'Q') name = "Ферзь";
            else if (piece.type == 'N') name = "Конь";
            else name = "Фигура";
            cout << name << " на (" << piece.x << "," << piece.y << ")\n";
        }
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


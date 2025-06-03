#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

int countMismatches(const vector<vector<int>>& grid, const vector<vector<int>>& pattern) {
    int mismatches = 0;
    int n = grid.size(), m = grid[0].size();
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            if (grid[i][j] != pattern[i][j])
                ++mismatches;
    return mismatches;
}

vector<vector<int>> generateRandomGrid(int n, int m) {
    vector<vector<int>> grid(n, vector<int>(m));
    srand(time(0));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            grid[i][j] = rand() % 2;
    return grid;
}

void printGrid(const vector<vector<int>>& grid) {
    for (const auto& row : grid) {
        for (int cell : row)
            cout << cell << ' ';
        cout << '\n';
    }
}

int minMovesToChessboard(const vector<vector<int>>& grid) {
    int n = grid.size(), m = grid[0].size();

    vector<vector<int>> pattern1(n, vector<int>(m));
    vector<vector<int>> pattern2(n, vector<int>(m));

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j) {
            pattern1[i][j] = (i + j) % 2;
            pattern2[i][j] = 1 - pattern1[i][j];
        }

    int mismatch1 = countMismatches(grid, pattern1);
    int mismatch2 = countMismatches(grid, pattern2);

    int result = -1;
    if (mismatch1 % 2 == 0)
        result = mismatch1 / 2;
    if (mismatch2 % 2 == 0) {
        int alt = mismatch2 / 2;
        if (result == -1 || alt < result)
            result = alt;
    }

    return result;
}

int main() {
    int N, M;
    cout << "Введите размеры щита: ";
    cin >> N >> M;

    if (N <= 0 || M <= 0) {
        cout << "Размеры должны быть положительными целыми числами.\n";
        return 1;
    }

    vector<vector<int>> grid = generateRandomGrid(N, M);

    cout << "\nИсходная матрица:\n";
    printGrid(grid);

    int moves = minMovesToChessboard(grid);
    if (moves == -1)
        cout << "\nНевозможно привести к шахматному виду.\n";
    else
        cout << "\nМинимальное число ходов: " << moves << endl;

    return 0;
}

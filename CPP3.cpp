#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

const int N = 4;
const double EPS = 1e-3;

bool LU_Decompose(const vector<vector<double>>& A, vector<vector<double>>& L, vector<vector<double>>& U) {
    for (int i = 0; i < N; i++) {
        // U[i][j]
        for (int j = i; j < N; j++) {
            double sum = 0;
            for (int k = 0; k < i; k++)
                sum += L[i][k] * U[k][j];
            U[i][j] = A[i][j] - sum;
        }

        if (fabs(U[i][i]) < 1e-15) {
            cout << "Ошибка: нулевой элемент на диагонали U в позиции " << i << endl;
            return false;
        }

        for (int j = i; j < N; j++) {
            if (i == j)
                L[i][i] = 1;
            else {
                double sum = 0;
                for (int k = 0; k < i; k++)
                    sum += L[j][k] * U[k][i];
                L[j][i] = (A[j][i] - sum) / U[i][i];
            }
        }
    }
    return true;
}

vector<double> ForwardSubstitution(const vector<vector<double>>& L, const vector<double>& b) {
    vector<double> y(N);
    for (int i = 0; i < N; i++) {
        double sum = 0;
        for (int j = 0; j < i; j++)
            sum += L[i][j] * y[j];
        y[i] = b[i] - sum;
    }
    return y;
}

vector<double> BackwardSubstitution(const vector<vector<double>>& U, const vector<double>& y) {
    vector<double> x(N);
    for (int i = N - 1; i >= 0; i--) {
        double sum = 0;
        for (int j = i + 1; j < N; j++)
            sum += U[i][j] * x[j];
        x[i] = (y[i] - sum) / U[i][i];
    }
    return x;
}

int main() {
    double M = -0.77, N_param = 0.16, P = 1.12;

    vector<vector<double>> A = {
        {M,     -0.04, 0.21,   -18},
        {0.25,  -1.23, N_param, -0.09},
        {-0.21, N_param, 0.8,  -0.13},
        {0.15,  -1.31, 0.06,   P}
    };

    vector<double> b = {-1.24, P, 2.56, M};

    vector<vector<double>> L(N, vector<double>(N, 0));
    vector<vector<double>> U(N, vector<double>(N, 0));

    bool ok = LU_Decompose(A, L, U);
    if (!ok) {
        cout << "LU-разложение не удалось." << endl;
        return 1;
    }

    cout << fixed << setprecision(6);

    cout << "Матрица L:" << endl;
    for (auto& row : L) {
        for (double val : row)
            cout << setw(12) << val << " ";
        cout << endl;
    }

    cout << "\nМатрица U:" << endl;
    for (auto& row : U) {
        for (double val : row)
            cout << setw(12) << val << " ";
        cout << endl;
    }

    vector<double> y = ForwardSubstitution(L, b);

    cout << "\nРезультат прямого хода (вектор y):" << endl;
    for (int i = 0; i < N; i++)
        cout << "y[" << i+1 << "] = " << y[i] << endl;

    vector<double> x = BackwardSubstitution(U, y);

    cout << "\nРезультат обратного хода (решение x):" << endl;
    for (int i = 0; i < N; i++)
        cout << "x[" << i+1 << "] = " << x[i] << endl;

    return 0;
}


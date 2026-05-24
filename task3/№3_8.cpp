#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

const int n = 4;
const double eps = 1e-3;

double A[n][n] = {
    {0.91, -0.04, 0.21, -1.16},
    {0.25, -1.23, -0.23, -0.09},
    {-0.21, -0.23, 0.8, -0.13},
    {0.15, -1.31, 0.06, -1.04}
};

double b[n] = {-1.24, -1.04, 2.56, 0.91};

void luDecomposition(double L[n][n], double U[n][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            L[i][j] = 0;
            U[i][j] = 0;
        }
        L[i][i] = 1;
    }
    
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            double sum = 0;
            for (int k = 0; k < i; k++) {
                sum += L[i][k] * U[k][j];
            }
            U[i][j] = A[i][j] - sum;
        }
        
        for (int j = i + 1; j < n; j++) {
            double sum = 0;
            for (int k = 0; k < i; k++) {
                sum += L[j][k] * U[k][i];
            }
            L[j][i] = (A[j][i] - sum) / U[i][i];
        }
    }
}

void solveLy(double L[n][n], double b[n], double y[n]) {
    for (int i = 0; i < n; i++) {
        double sum = 0;
        for (int j = 0; j < i; j++) {
            sum += L[i][j] * y[j];
        }
        y[i] = b[i] - sum;
    }
}

void solveUx(double U[n][n], double y[n], double x[n]) {
    for (int i = n - 1; i >= 0; i--) {
        double sum = 0;
        for (int j = i + 1; j < n; j++) {
            sum += U[i][j] * x[j];
        }
        x[i] = (y[i] - sum) / U[i][i];
    }
}

void directMethod(double x[n]) {
    cout << "\nПРЯМОЙ МЕТОД: LU-разложение\n" << endl;
    
    double L[n][n], U[n][n];
    luDecomposition(L, U);
    
    cout << "Матрица L:" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << setw(10) << setprecision(4) << L[i][j] << " ";
        }
        cout << endl;
    }
    
    cout << "\nМатрица U:" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << setw(10) << setprecision(4) << U[i][j] << " ";
        }
        cout << endl;
    }
    
    double y[n];
    solveLy(L, b, y);
    
    cout << "\nВектор y (решение Ly = b):" << endl;
    for (int i = 0; i < n; i++) {
        cout << "y[" << i << "] = " << setw(10) << setprecision(6) << y[i] << endl;
    }
    
    solveUx(U, y, x);
    
    cout << "\nРешение системы (прямой метод):" << endl;
    for (int i = 0; i < n; i++) {
        cout << "x[" << i << "] = " << setw(12) << setprecision(6) << x[i] << endl;
    }
}

void seidelMethod(double x_start[n], double eps) {
    cout << "\nПРИБЛИЖЁННЫЙ МЕТОД: Зейделя\n" << endl;
    
    double x[n];
    double x_prev[n];
    for (int i = 0; i < n; i++) {
        x[i] = x_start[i];
        x_prev[i] = x_start[i];
    }
    
    // Проверка диагонального преобладания
    bool diagDom = true;
    for (int i = 0; i < n; i++) {
        double sum = 0;
        for (int j = 0; j < n; j++) {
            if (i != j) sum += fabs(A[i][j]);
        }
        if (fabs(A[i][i]) <= sum) {
            diagDom = false;
        }
    }
    if (diagDom) {
        cout << "Диагональное преобладание есть -> сходимость гарантирована" << endl;
    } else {
        cout << "Диагонального преобладания нет, но метод может сойтись" << endl;
    }
    
    cout << "\nN   x1         x2         x3         x4         epsilon" << endl;
    
    int iter = 0;
    double maxDiff;
    
    do {
        for (int i = 0; i < n; i++) {
            x_prev[i] = x[i];
        }
        
        for (int i = 0; i < n; i++) {
            double sum = b[i];
            for (int j = 0; j < n; j++) {
                if (j != i) {
                    sum -= A[i][j] * x[j];
                }
            }
            x[i] = sum / A[i][i];
        }
        
        maxDiff = 0;
        for (int i = 0; i < n; i++) {
            double diff = fabs(x[i] - x_prev[i]);
            if (diff > maxDiff) maxDiff = diff;
        }
        
        cout << iter << "   ";
        for (int i = 0; i < n; i++) {
            cout << setw(10) << setprecision(6) << x[i] << " ";
        }
        cout << setw(10) << setprecision(6) << maxDiff << endl;
        
        iter++;
        
    } while (maxDiff > eps && iter < 1000);
    
    cout << "\nРешение системы (метод Зейделя, eps = " << eps << "):" << endl;
    for (int i = 0; i < n; i++) {
        cout << "x[" << i << "] = " << setw(12) << setprecision(6) << x[i] << endl;
    }
    cout << "Количество итераций: " << iter << endl;
}

int main() {
    cout << "\nВариант 8: M = 0.91, N = -0.23, P = -1.04" << endl;
    cout << "Прямой метод: LU-разложение" << endl;
    cout << "Приближённый метод: Зейделя" << endl;
    cout << "Точность eps = " << eps << endl;
    
    cout << "\nИсходная матрица A:" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << setw(10) << setprecision(4) << A[i][j] << " ";
        }
        cout << "| " << setw(10) << b[i] << endl;
    }
    
    double x_direct[n];
    directMethod(x_direct);
    
    double x_start[n] = {0, 0, 0, 0};
    seidelMethod(x_start, eps);
    
    return 0;
}
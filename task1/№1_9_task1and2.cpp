#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>

using namespace std;

int main() {
    srand(time(0));
    
    // ПУНКТ 1
    cout << " ПУНКТ 1 " << endl;
    
    const int M = 8;  // M > 7
    const int N = 6;  // N > 5
    
    int matrix1[M][N];
    
    // Заполнение матрицы
    cout << "Матрица " << M << "x" << N << ":" << endl;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            matrix1[i][j] = rand() % 61 - 40;
            cout << matrix1[i][j] << "\t";
        }
        cout << endl;
    }
    
    // Поиск строк с 3+ отрицательными
    vector<int> rowsWithThree;
    int negCount[M];
    
    for (int i = 0; i < M; i++) {
        negCount[i] = 0;
        for (int j = 0; j < N; j++) {
            if (matrix1[i][j] < 0) negCount[i]++;
        }
        if (negCount[i] >= 3) rowsWithThree.push_back(i);
    }
    
    cout << "\nСтроки с >=3 отрицательными: ";
    if (rowsWithThree.empty()) cout << "нет";
    else for (int row : rowsWithThree) cout << row << " ";
    cout << endl;
    
    // Сбор отрицательных элементов
    vector<int> negativeElements;
    for (int row : rowsWithThree) {
        for (int j = 0; j < N; j++) {
            if (matrix1[row][j] < 0) negativeElements.push_back(matrix1[row][j]);
        }
    }
    
    cout << "Найдено строк: " << rowsWithThree.size() << endl;
    cout << "Отрицательных элементов: " << negativeElements.size() << endl;
    if (!negativeElements.empty()) {
        cout << "Массив: ";
        for (int val : negativeElements) cout << val << " ";
        cout << endl;
    }
    
    // ПУНКТ 2
    cout << "\n ПУНКТ 2 " << endl;
    
    const int N2 = 6;  // чётное, >3
    int matrix2[N2][N2];
    
    // Заполнение матрицы [10,30]
    cout << "Квадратная матрица " << N2 << "x" << N2 << ":" << endl;
    for (int i = 0; i < N2; i++) {
        for (int j = 0; j < N2; j++) {
            matrix2[i][j] = rand() % 21 + 10;
            cout << matrix2[i][j] << "\t";
        }
        cout << endl;
    }
    
    int half = N2 / 2;
    
    // 1. Левая нижняя четверть -> 0
    for (int i = half; i < N2; i++) {
        for (int j = 0; j < half; j++) {
            matrix2[i][j] = 0;
        }
    }
    
    // 2. Правая нижняя четверть -> 10
    for (int i = half; i < N2; i++) {
        for (int j = half; j < N2; j++) {
            matrix2[i][j] = 10;
        }
    }
    
    // 3. Меняем местами верхнюю левую и верхнюю правую четверти
    for (int i = 0; i < half; i++) {
        for (int j = 0; j < half; j++) {
            swap(matrix2[i][j], matrix2[i][j + half]);
        }
    }
    
    // Вывод результата
    cout << "\nМатрица после преобразований:" << endl;
    for (int i = 0; i < N2; i++) {
        for (int j = 0; j < N2; j++) {
            cout << matrix2[i][j] << "\t";
        }
        cout << endl;
    }
    
    return 0;
}
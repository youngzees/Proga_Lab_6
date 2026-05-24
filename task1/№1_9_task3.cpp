#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>  // вместо windows.h (для usleep)

using namespace std;

const int WIDTH = 40;   // ширина поля
const int HEIGHT = 25;  // высота поля

// Копирование поля
void copyGrid(int source[HEIGHT][WIDTH], int dest[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            dest[i][j] = source[i][j];
        }
    }
}

// Подсчёт живых соседей (с учётом замкнутой плоскости)
int countNeighbors(int grid[HEIGHT][WIDTH], int x, int y) {
    int count = 0;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue;
            int nx = (x + dx + HEIGHT) % HEIGHT;
            int ny = (y + dy + WIDTH) % WIDTH;
            count += grid[nx][ny];
        }
    }
    return count;
}

// Следующее поколение
void nextGeneration(int grid[HEIGHT][WIDTH], int newGrid[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            int neighbors = countNeighbors(grid, i, j);
            if (grid[i][j] == 1) {
                // Живая клетка
                if (neighbors < 2 || neighbors > 3) newGrid[i][j] = 0;
                else newGrid[i][j] = 1;
            } else {
                // Мёртвая клетка
                if (neighbors == 3) newGrid[i][j] = 1;
                else newGrid[i][j] = 0;
            }
        }
    }
}

// Вывод поля
void printGrid(int grid[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (grid[i][j] == 1) cout << "█";  // живая
            else cout << " ";                  // мёртвая (пробел)
        }
        cout << endl;
    }
}

// Установка начальной конфигурации (Gosper Glider Gun)
void setupGliderGun(int grid[HEIGHT][WIDTH]) {
    // Очищаем всё
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            grid[i][j] = 0;
        }
    }
    
    // Координаты ружья (сместим, чтобы не улетело сразу)
    int offsetX = 10, offsetY = 5;
    
    // Gosper Glider Gun (стандартные координаты)
    int gun[36][2] = {
        {0, 24}, {1, 22}, {1, 24}, {2, 12}, {2, 13}, {2, 20}, {2, 21}, 
        {2, 34}, {2, 35}, {3, 11}, {3, 15}, {3, 20}, {3, 21}, {3, 34}, 
        {3, 35}, {4, 0}, {4, 1}, {4, 10}, {4, 16}, {4, 20}, {4, 21}, 
        {5, 0}, {5, 1}, {5, 10}, {5, 14}, {5, 16}, {5, 17}, {5, 22}, 
        {5, 24}, {6, 10}, {6, 16}, {6, 24}, {7, 11}, {7, 15}, {8, 12}, 
        {8, 13}
    };
    
    for (int i = 0; i < 36; i++) {
        int x = gun[i][0] + offsetX;
        int y = gun[i][1] + offsetY;
        if (x >= 0 && x < HEIGHT && y >= 0 && y < WIDTH) {
            grid[x][y] = 1;
        }
    }
}

// Случайная генерация (вероятность жизни ~20%)
void setupRandom(int grid[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            grid[i][j] = (rand() % 100 < 20) ? 1 : 0;
        }
    }
}

int main() {
    srand(time(0));
    
    int grid[HEIGHT][WIDTH];
    int newGrid[HEIGHT][WIDTH];
    
    cout << "Выберите начальную конфигурацию:" << endl;
    cout << "1 - Gosper Glider Gun (ружьё)" << endl;
    cout << "2 - Случайная генерация" << endl;
    cout << "Ваш выбор: ";
    int choice;
    cin >> choice;
    
    if (choice == 1) {
        setupGliderGun(grid);
    } else {
        setupRandom(grid);
    }
    
    int generation = 0;
    
    // Анимация (100 поколений)
    for (int step = 0; step < 100; step++) {
        system("clear");  // для Linux (вместо cls)
        
        cout << "Поколение: " << generation << endl;
        cout << "Живые клетки: ";
        int alive = 0;
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                alive += grid[i][j];
            }
        }
        cout << alive << endl << endl;
        
        printGrid(grid);
        
        nextGeneration(grid, newGrid);
        copyGrid(newGrid, grid);
        generation++;
        
        usleep(100000);  // 100 000 микросекунд = 0.1 секунды
    }
    
    return 0;
}
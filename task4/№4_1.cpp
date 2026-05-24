#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <string>

using namespace std;

int N, M;

string toKey(const vector<vector<int>>& a) {
    string s;
    for (auto& row : a)
        for (int x : row) s += to_string(x);
    return s;
}

vector<vector<int>> copyBoard(const vector<vector<int>>& a) {
    return a;
}

bool isChess(const vector<vector<int>>& a) {
    int v = a[0][0];
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            if (a[i][j] != ((i + j) % 2 == 0 ? v : 1 - v)) return false;
    return true;
}

void printBoard(const vector<vector<int>>& a) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) cout << a[i][j];
        cout << endl;
    }
}

int bfs(vector<vector<int>> start, vector<vector<int>>& finalBoard) {
    queue<vector<vector<int>>> q;
    map<string, int> dist;

    q.push(start);
    dist[toKey(start)] = 0;

    while (!q.empty()) {
        auto cur = q.front(); q.pop();
        int d = dist[toKey(cur)];

        if (isChess(cur)) {
            finalBoard = cur;
            return d;
        }

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                if (j + 1 < M) {
                    auto nb = copyBoard(cur);
                    nb[i][j] = 1 - nb[i][j];
                    nb[i][j+1] = 1 - nb[i][j+1];
                    string key = toKey(nb);
                    if (dist.find(key) == dist.end()) {
                        dist[key] = d + 1;
                        q.push(nb);
                    }
                }
                if (i + 1 < N) {
                    auto nb = copyBoard(cur);
                    nb[i][j] = 1 - nb[i][j];
                    nb[i+1][j] = 1 - nb[i+1][j];
                    string key = toKey(nb);
                    if (dist.find(key) == dist.end()) {
                        dist[key] = d + 1;
                        q.push(nb);
                    }
                }
            }
        }
    }
    return -1;
}

int main() {
    cout << "\nВведите размеры доски N и M: ";
    cin >> N >> M;

    vector<vector<int>> start(N, vector<int>(M));

    cout << "Введите доску (0 - орёл, 1 - решка):" << endl;
    for (int i = 0; i < N; i++) {
        string s;
        cin >> s;
        for (int j = 0; j < M; j++) start[i][j] = s[j] - '0';
    }

    vector<vector<int>> finalBoard;
    int moves = bfs(start, finalBoard);

    cout << "\nМинимальное количество ходов: " << moves << endl;
    cout << "Финальная доска (шахматный паттерн):" << endl;
    printBoard(finalBoard);

    return 0;
}
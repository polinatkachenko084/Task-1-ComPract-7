#include <iostream>
#include <cmath>
#include <windows.h>
using namespace std;

const int MAX_N = 10; // розмір статичної матриці

// Створення динамічної матриці n x n
int** createMatrix(int n) {
    int** m = new int* [n];
    for (int i = 0; i < n; i++) {
        m[i] = new int[n];
    }
    return m;
}

// Звільнення пам'яті, виділеної під матрицю
void freeMatrix(int** m, int n) {
    for (int i = 0; i < n; i++) {
        delete[] m[i];
    }
    delete[] m;
}

// Введення матриці з клавіатури
void inputMatrix(int** m, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << "A[" << i + 1 << "][" << j + 1 << "] = ";
            cin >> m[i][j];
        }
    }
}

// Копіювання статичної матриці в динамічну
void copyFromStatic(const int src[][MAX_N], int** dst, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            dst[i][j] = src[i][j];
        }
    }
}

// Виведення матриці
void printMatrix(int** m, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout.width(4);
            cout << m[i][j];
        }
        cout << endl;
    }
}

// Кількість нульових елементів на головній діагоналі
int countZeroDiagonal(int** m, int n) {
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (m[i][i] == 0) count++;
    }
    return count;
}

// Пошук рядка для позиції pos з можливим перепризначенням
bool findRow(int** src, int n, int pos, bool* used, int* owner) {
    for (int r = 0; r < n; r++) {
        if (src[r][pos] != 0 && !used[r]) {
            used[r] = true;
            if (owner[r] == -1 || findRow(src, n, owner[r], used, owner)) {
                owner[r] = pos;
                return true;
            }
        }
    }
    return false;
}

// Переставляє рядки матриці
bool fixDiagonal(int** src, int** res, int n) {
    int* owner = new int[n];
    bool* used = new bool[n];
    for (int r = 0; r < n; r++) owner[r] = -1;

    bool ok = true;
    for (int pos = 0; pos < n && ok; pos++) {
        for (int r = 0; r < n; r++) used[r] = false;
        if (!findRow(src, n, pos, used, owner)) {
            ok = false;
        }
    }

    if (ok) {
        for (int r = 0; r < n; r++) {
            for (int j = 0; j < n; j++) {
                res[owner[r]][j] = src[r][j];
            }
        }
    }

    delete[] owner;
    delete[] used;
    return ok;
}

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    // Статична матриця з готовим прикладом
    int example[MAX_N][MAX_N] = {
        {0, 3, 0, 0},
        {1, 0, 0, 2},
        {0, 0, 4, 0},
        {5, 0, 0, 0}
    };

    const int exampleN = 4;

    int choice;

    cout << "1 - ввести матрицю з клавіатури" << endl;
    cout << "2 - взяти готовий приклад (статична матриця 4x4)" << endl;
    cout << "Ваш вибір: ";
    cin >> choice;

    int n;
    int** a;

    if (choice == 2) {
        n = exampleN;
        a = createMatrix(n);
        copyFromStatic(example, a, n);
    }
    else {
        cout << "Введіть розмір квадратної матриці n: ";
        cin >> n;
        while (n <= 0) {
            cout << "n має бути додатним. Спробуйте ще раз: ";
            cin >> n;
        }
        a = createMatrix(n);
        inputMatrix(a, n);
    }

    cout << endl << "Початкова матриця:" << endl;
    printMatrix(a, n);

    if (countZeroDiagonal(a, n) == 0) {
        cout << "На головній діагоналі немає нулів, переставляти нічого не треба." << endl;
    }
    else {
        int** b = createMatrix(n);

        if (fixDiagonal(a, b, n)) {
            cout << endl << "Матриця після переставлення рядків:" << endl;
            printMatrix(b, n);
        }
        else {
            cout << "Зробити діагональ ненульовою переставленням неможливо." << endl;
        }

        freeMatrix(b, n);
    }

    freeMatrix(a, n);

    return 0;
}
// zm_col.cpp : алгоритм зміщення по колу 
//

#include <iostream>
#include <vector>

using namespace std;

// Функція для здійснення зміщення по колу сторінок
void circularShift(vector<int>& arr, int k) {
    int n = arr.size();
    k = k % n;  // Забезпечуємо, щоб k було в межах розміру масиву

    // Крок 1: Обертання всього масиву
    reverse(arr.begin(), arr.end());

    // Крок 2: Обертання перших k елементів масиву
    reverse(arr.begin(), arr.begin() + k);

    // Крок 3: Обертання залишку елементів масиву (від k до n-1)
    reverse(arr.begin() + k, arr.end());
}

int main() {
    // Приклад використання алгоритму зміщення по колу сторінок у векторі
    vector<int> memoryPages = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int shiftAmount = 3;

    // Друк вектора перед зміщенням
    cout << "Before circular shift: ";
    for (int page : memoryPages) {
        cout << page << " ";
    }
    cout << endl;

    // Здійснення зміщення по колу сторінок
    circularShift(memoryPages, shiftAmount);

    // Друк вектора після зміщення
    cout << "After circular shift: ";
    for (int page : memoryPages) {
        cout << page << " ";
    }
    cout << endl;

    return 0;
}

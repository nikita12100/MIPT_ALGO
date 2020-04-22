/*
 * Даны два массива целых чисел одинаковой длины A[0..n-1] и B[0..n-1].
 * Необходимо найти первую пару индексов i0 и j0, ,
 * такую что A[i0] + B[j0] = max {A[i] + B[j], где 0 <= i < n, 0 <= j < n, i <= j}.
 * Время работы - O(n). n ≤ 100000.
 * 
 */
#include <iostream>

using namespace std;

void max(int* a, int* b, int l) {
    int max_j = l - 1;
    int max_i = l - 1;
    int buff_j = l - 1;
    int max_par = a[l - 1] + b[l - 1];

    for (int i = l - 1; i >= 0; i--) {
        if (b[i] >= b[buff_j]) buff_j = i;

        if((a[i] + b[buff_j] >= max_par) && (buff_j >= i)){
            max_par = a[i] + b[buff_j];
            max_i = i;
            max_j = buff_j;
        }
    }

    cout << max_i << " " << max_j;
}

int main() {
    int lenght = 0;
    cin >> lenght;

    int* a = (int*)malloc(lenght * sizeof(int));
    for (int i = 0; i < lenght; i++) cin >> a[i];
    
    int* b = (int*)malloc(lenght * sizeof(int));
    for (int i = 0; i < lenght; i++) cin >> b[i];

    max(a, b, lenght);

    free(a);
    free(b);

    return 0;
}
/*
 * Даны два массива неповторяющихся целых чисел, упорядоченные по возрастанию. A[0..n-1] и B[0..m-1]. n » m.
 * Найдите их пересечение. Требуемое время работы: O(m * log k), где k - позиция элементта B[m-1] в массиве A..
 * В процессе поиска очередного элемента B[i]
 * в массиве A пользуйтесь результатом поиска элемента B[i-1]. n, k ≤ 10000.
 *
 */

#include <iostream>
#include <cmath>

using namespace std;

int bin_search(int* m , int x, int n){ //бин поиск
    int begin = 0;
    int end = n;
    int middle  = 0;
    int i = 0;

    int q = pow(2,i); // по степеням двойки
    while(q < n){
        if(m[q] < x){
           	 q = pow(2, ++i);
        } else break;
    }
    if(q > n) q = n;
    begin = pow(2, i-1); // зажимаем 
    end = q;

    while(begin < end){  //бинарный поиск
        middle = (begin + end) / 2;
        if(m[middle] < x) begin = middle + 1;
        else end = middle;
    }

    if ( begin == n || m[begin] != x )  return -1;
    else return begin;
}

int main() {
    int l1, l2 = 0;
    cin >> l1 >> l2;

    int *a = new int[l1];
    for (int i = 0; i < l1; i++) cin >> a[i];
    int *b = new int[l2];
    for (int i = 0; i < l2; i++) cin >> b[i];

    int first_result = -1;
    int buff = 0;
    for (int i = 0; i < l2; i++) {
        buff = bin_search(a, b[i], l1);
        if (buff > -1) {
            first_result = buff;
            cout << a[first_result] << " ";
        }
    }


    delete[] (a);
    delete[] (b);
    return 0;
}
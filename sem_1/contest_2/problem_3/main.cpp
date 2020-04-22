/*
Даны неотрицательные целые числа n, k и массив целых чисел из диапазона [0..109] размера n.
Требуется найти k-ю порядковую статистику. т.е. напечатать число, которое бы стояло на позиции с индексом k ∈[0..n-1] в отсортированном массиве.
Напишите нерекурсивный алгоритм.
Требования к дополнительной памяти: O(n).
Требуемое среднее время работы: O(n).
Функцию Partition следует реализовывать методом прохода двумя итераторами в одном направлении.
Описание для случая прохода от начала массива к концу:
Выбирается опорный элемент.
Опорный элемент меняется с последним элементом массива.
Во время работы Partition в начале массива содержатся элементы, не бОльшие опорного. Затем располагаются элементы, строго бОльшие опорного.
В конце массива лежат нерассмотренные элементы. Последним элементом лежит опорный.
Итератор (индекс) i указывает на начало группы элементов, строго бОльших опорного.
Итератор j больше i, итератор j указывает на первый нерассмотренный элемент.
Шаг алгоритма. Рассматривается элемент, на который указывает j. Если он больше опорного, то сдвигаем j.
Если он не больше опорного, то меняем a[i] и a[j] местами, сдвигаем i и сдвигаем j.
В конце работы алгоритма меняем опорный и элемент, на который указывает итератор i.

3_1. Реализуйте стратегию выбора опорного элемента “медиана трёх”.
Функцию Partition реализуйте методом прохода двумя итераторами от начала массива к концу.

*/
#include <iostream>
#include <assert.h>

using namespace std;

bool myCpm(const int& a, const int& b){
    return a > b;
}

template <class T>
int Mediana(int* arr, int a, int c, bool (*myCmp)(const T&, const T&)){
    int b = (a + (c+1)) / 2;
    if(((c+1) - a) <= 2) return c;
    else {
        if (myCmp(arr[a] , arr[b])) {
            if (myCmp(arr[c] , arr[a]))
                return a;
            else if (myCmp(arr[b] , arr[c])) return b;
            else return c;
        }
        else if (myCmp(arr[c] , arr[b]))
            return b;
        else if (myCmp(arr[a] , arr[c])) return a;
        else return c;
    }
}

template <class T>
int Partition( int* a, int end, int beg, bool (*myCmp)(const T&, const T&) ) {
    int med = Mediana(a, beg, end-1, myCmp) ; //ищем медиану

    if(med != (end-1)) swap(a[med] , a[end - 1]);
    const int& pivot = a[end - 1];
    int i = beg;
    int j = beg;
    while(j < end - 1) {
        for( ; myCmp(a[j], pivot); j++ ) {}
        if( !myCmp(a[j] , pivot) && j != (end-1)) {
            swap( a[i++], a[j++]);
        }
    }
    swap(a[i], a[end - 1]);

    return i;
}

template <class T>
int K_Stat( int* a, int n, int k, bool (*myCmp)(const T&, const T&)) {
    assert(k < n);
    int begin = 0, end =  n;
    int p = -1;
    while (1){ // пока не попадем пивотом
        p = Partition(a, end, begin, myCmp);
        if(k < p){
            end = p;
        }
        else if(k > p){
            begin = p + 1;
        }
        else return a[k];
    }
}

int main(){
    int n = 0;
    int k = 0;
    cin >> n >> k;

    int* a = new int[n];
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    cout << K_Stat(a, n, k, myCpm) << endl;

    delete[] a;
    return 0;
}
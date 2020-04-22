#include <iostream>
#include <cstring>

using namespace std;

bool isLess(const int &a, const int &b){
    return a <= b;
}

template <class T>
void Merge(const T* a, int a_Len, const T* b, int b_Len, T* c , bool (*myCmp)(const T&, const T&)){
    int i = 0;
    int j = 0;
    while (i < a_Len && j < b_Len){
        if( myCmp(a[i] , b[j]) ){
            c[i + j] = a[i];
            ++i;
        } else{
            c[i + j] = b[j];
            ++j;
        }
    }

    if (i == a_Len){
        for ( ; j < b_Len; ++j)
            c[i + j] = b[j];
    } else{
        for ( ; i < a_Len; ++i)
            c[i + j] = a[i];
    }
}

template <class T>
void mergeSort(T* a, int a_Len, bool (*myCmp)(const T&, const T&)) {
    if (a_Len <= 1) {
        return;
    }
    int firstLen = a_Len / 2;
    int secondLen = a_Len - firstLen;
    mergeSort(a, firstLen, myCmp);
    mergeSort(a + firstLen, secondLen, myCmp);
    T* c = new T[a_Len];
    Merge(a, firstLen, a + firstLen, secondLen, c , myCmp);
    memcpy(a, c, sizeof(T) * a_Len);
    delete[] c;
}

template <class T>
void Merging(T* a, int n, T* buff, int k) {
    for (int i = 0; i < k; i++){
        buff[i] = a[i];
    }
    if (3*k > n)
        for (int i = 2*k; i < n; i++) {
            buff[k] = a[i];
        }
    else
        for (int i = 2*k; i < 3*k; i++) {
            buff[k] = a[i];
        }
}

template <class T>
void mySort(T *a, int n, int k , bool (*myCmp)(const T&, const T&)) {
    T *midd = new T[3 * k];
    T *buff = new T[2 * k];
    int len = 0;

    for (int i = k; i < n; i = i + k) {
        if(i + k < n){
            Merging(a + i - k, n, buff, k);
            len = k;
            Merge(a + i, k, a + i - k, k, midd, myCmp);
        } else {
            len = n - i;
            Merge(a + i - k, k, a + i, len, midd, myCmp);
        }
        int j = i - k;
        for (int m = 0; m < k + len; m++) {
            a[j] = midd[m];
            j++;
        }
    }
    delete[] midd;
    delete[] buff;
}

int main() {
    int n = 0;
    int k = 0;
    cin >> n;
    cin >> k;

    int* a = new int[n];
    for (int i = 0; i < n; i++) cin >> a[i];

    for (int i = 0; i < n; i = i + k) { // разбиваем массив на k-подмассивы
        if (i + k >= n) {
            mergeSort(a + i, n - i, isLess); // остаток
            break;
        }
        mergeSort(a + i, k, isLess); //// сортируем каждый подмассив
    }

    mySort(a, n, k, isLess);  // cливаем i нечетный массив с соседними массивами , т.к. его элементы попадут туда
    for (int i = 0; i < n; i++) {
        cout << a[i] << " ";
    }

    delete[] a;

    return 0;
}
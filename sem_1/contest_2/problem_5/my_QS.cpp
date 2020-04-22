#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

template <class T>
void my_swap(T &a, T &b){
    T buff = a;
    a = b;
    b = buff;
}

void insertion_Sort(int *a, int n) { // оптимизация partition
    for( int i = 1; i < n; ++i ) {
        int tmp = a[i]; // запомним, может перезаписаться.
        int j = i - 1;
        for( ; j >= 0 && tmp < a[j]; --j ){
            a[j + 1] = a[j];
        }
        a[j + 1] = tmp;
    }
}

int Mediana(int* arr, int a, int c){ //оптимизация пивота
    int b = (a + (c+1)) / 2;
    if(((c+1) - a) <= 2) return c;
    else {
        if (arr[a] > arr[b]) {
            if (arr[c] > arr[a])
                return a;
            else if (arr[b] > arr[c]) return b;
            else return c;
        }
        else if (arr[c] > arr[b])
            return b;
        else if (arr[a] > arr[c]) return a;
        else return c;
    }
}

int Partition(int* a, int n ) {
    if( n <= 20 ) {
        insertion_Sort(a, n);
        return -1;
    }
    int med = Mediana(a, 0, n-1) ; //ищем медиану

    if(med != (n-1)) my_swap(a[med], a[n - 1]);
    const int& pivot = a[n - 1];

    int i = 0;
    int j = n - 2;
    while( i <= j ) {
        for( ; a[i] < pivot; ++i ) {}
        for( ; j >= 0 && !( a[j] < pivot ); --j ) {}
        if( i < j ) {
            my_swap(a[i++], a[j--]);
        }
    }
    my_swap(a[i], a[n - 1]);
    return i;
}

void MY_quick_Sort(int *a, int n) {
    int part = Partition(a, n);
    if( part == -1 ) return;
    if( part > 0 ) MY_quick_Sort(a, part);
    if( part + 1 < n ) MY_quick_Sort(a + part + 1, n - (part + 1));
}

bool MY_read_int(int& input) // оптимизаия ввода
{
    int ch = getchar();
    for (; !('0'<=ch && ch<='9'); ch = getchar())
    {
        if (ch == EOF || ch < '0' || ch > '9')
            return false;
    }
    if (ch == EOF)
        return false;

    int number = 0;
    for (; '0' <= ch && ch <= '9' ; ch = getchar())
    {
        number = number*10 + ch - '0';
    }
    input = number;
    return true;
}

int main() {
    int n = 10;
    int* a = new int[n];
    int i = 0;
    while(MY_read_int(a[i])){
        if(i + 1 >= n){
            n *= 2;
            int* buff = new int[n];
            for (int j = 0; j < n/2; ++j) {
                buff[j] = a[j];
            }
            delete[] a;
            a = buff;
        }
        i++;
    }

    MY_quick_Sort(a, i);

    for (int j = 9; j < i; j += 10) cout << a[j] << " ";

    delete[] a;
    return 0;
}
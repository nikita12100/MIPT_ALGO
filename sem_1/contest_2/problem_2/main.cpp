/**
 * В супермаркете решили оптимизировать показ рекламы.
 * Известно расписание прихода и ухода покупателей (два целых числа).
 * Каждому покупателю необходимо показать минимум 2 рекламы.
 * Рекламу можно транслировать только в целочисленные моменты времени.
 * Покупатель может видеть рекламу от момента прихода до момента ухода из магазина.
 * В каждый момент времени может показываться только одна реклама. Считается, что реклама показывается мгновенно.
 * Если реклама показывается в момент ухода или прихода, то считается, что посетитель успел её посмотреть.
Требуется определить минимальное число показов рекламы.
 */

#include <iostream>

using namespace std;

struct People{
    int in = 0;
    int out = 0;

    People() = default;
    People( int _in, int _out ) : in(_in), out(_out) {}

};
istream& operator >> (istream& istream , People& p1){
    return istream >> p1.in >> p1.out;
}
ostream& operator << (ostream& ostream , People& p1){
    return ostream << p1.in << " " << p1.out;
}
void swap(People& p1 , People& p2){
    People buff = p1;
    p1 = p2;
    p2 = buff;
}

bool isless(const People &p1, const People &p2){
    if(p1.in > p2.in) return true;
    if(p1.in == p2.in && p1.out < p2.out) return true;
    else return false;
}

template <class T>
void Build_Heap(T *arr, int n, int i, bool (*isless)(const T&, const T&));
template <class T>
void Sift_Down(T *arr, int heap_size, int i, bool (*isless)(const T&, const T&));

template <class T>
void HeapSort(T* a, int n,  bool (*isless)(const T&, const T&))
{
    int heapSize = n;
    Build_Heap(a, n, heapSize, isless);
    while( heapSize > 1 ) {
        swap( a[0], a[heapSize - 1] );
        --heapSize;
        Sift_Down(a, heapSize, 0, isless);
    }
}
template <class T>
void Build_Heap(T *arr, int n, int i, bool (*isless)(const T&, const T&))
{
    for( int i = n / 2 - 1; i >= 0; --i ) {
        Sift_Down(arr, n, i, isless);
    }
}

template <class T>
void Sift_Down(T *arr, int heap_size, int i , bool (*isless)(const T&, const T&))
{
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    int largest = i;
    if( left < heap_size && isless(arr[left] , arr[i]) )
        largest = left;
    if( right < heap_size && isless(arr[right] , arr[largest]) )
        largest = right;
    if( largest != i ) {
        swap( arr[i], arr[largest] );
        Sift_Down(arr, heap_size, largest, isless);
    }
}

void Erase(People *a, int &n, int x){
    for (int i = x; i < n - 1; ++i) {
        a[i].in = a[i + 1].in;
        a[i].out = a[i + 1].out;
    }
    n--;
}

template <class T>
int Quantity_of_show(T *p, int n) {
    //карта показа рекламы
    int time_size_aray = 0;
    for (int i = 0; i < n; ++i) {
        if (p[i].out > time_size_aray) time_size_aray = p[i].out;
    }
    time_size_aray++; //для удбства с единицы

    int *show = new int[time_size_aray];
    for (int k = 0; k < time_size_aray; ++k) show[k] = 0;
    /*************/

    while (n > 0){
        int i = n - 1;

        if(p[i].in == p[i - 1].in){
            Erase(p, n, i - 1);
        } //удаляем больший вектор с тем же началом

        else { // два вектора из разных начал
            int show_quantity = 0;  //проверяем кол-во рекоам в векторе
            for (int j = p[i].in; j <= p[i].out; ++j) {
                if (show[j] == 1) show_quantity++;
            }
            if (show_quantity == 0) show[p[i].in] = show[p[i].in + 1] = 1;
            if (show_quantity == 1) show[p[i].in] = 1;
            n--;
        }
    }

    int result = 0;
    for (int i = 0; i < time_size_aray; ++i) {
        if(show[i] == 1) result++;
    }

    delete[] show;
    return result;
}

int main() {
    int n = 0;
    cin >> n;

    People* p = new People[n];
    for (int i = 0; i < n; ++i) {
        cin >> p[i];
    }

    HeapSort( p , n , isless);

    cout << Quantity_of_show(p, n);

    delete[] p;

    return 0;
}
/*
 *Для сложения чисел используется старый компьютер. Время, затрачиваемое на нахождение суммы двух чисел равно их сумме.
 * Таким образом для нахождения суммы чисел 1,2,3 может потребоваться разное время, в зависимости от порядка вычислений.
 * ((1+2)+3) -> 1+2 + 3+3 = 9 ((1+3)+2) -> 1+3 + 4+2 = 10 ((2+3)+1) -> 2+3 + 5+1 = 11 Требуется написать программу,
 * которая определяет минимальное время, достаточное для вычисления суммы заданного набора чисел.
 */
#include <iostream>

using namespace std;

class Heap{
public:
    Heap(int* mass , int _size);
    ~Heap(){/* delete[] buff;*/ }

    void buildHeap();
    void siftDown(int i);
    void siftUp(int i);
    void add(int element);
    int get_min();
    bool isEmpty(){ return top == 1;}

private:
    int buff_size;
    int* buff;
    int top;
};

Heap::Heap(int* mass, int _size):buff_size(_size), top(_size) {
    buff = new int[buff_size];
    for (int i = 0; i <buff_size ; i++) {
        buff[i] = mass[i];
    }
    buildHeap();
};

void Heap::siftUp(int i){
    while(i > 0){
        int parent =(i-1) / 2;
        if(buff[i] >= buff[parent]) return;
        swap(buff[i], buff[parent]);
        i = parent;
    }
}
void Heap::siftDown(int i){
    int left = 2*i + 1;
    int right = 2*i + 2;
    int min = i;

    if(left < top-1 &&  buff[left] < buff[i] && buff[min] != 0) min = left;
    if(right < top-1 &&  buff[right] < buff[min] && buff[right] < buff[left] && buff[min] != 0) min = right;

    if(min != i){
        swap(buff[i] , buff[min]);
        siftDown(min);
    }
}
void Heap::buildHeap(){
    for (int i = buff_size/2 - 1; i >= 0 ; i--) {
        siftDown(i);
    }
}

void Heap::add(int elemet){
    if(top < buff_size) {
        buff[top] = elemet;
        top++;
        if(top > 3) this->siftUp(top - 1);
        else{
            int min = 0;
            if( buff[1] < buff[0] && buff[1] != 0) min = 1;
            if( buff[2] < buff[min] && buff[2] < buff[1] && buff[2] != 0) min = 2;
            swap(buff[0] , buff[min]);
        }
    }
}

int Heap::get_min(){
    int result = buff[0];
    buff[0] = buff[top - 1];
    buff[top - 1] = 0;
    top--;
    if(!this->isEmpty()){
        if(top > 3) siftDown(0);
        else{
            int min = 0;
            if( buff[1] < buff[0] && buff[1] != 0) min = 1;
            if( buff[2] < buff[0] && buff[2] < buff[1] && buff[2] != 0) min = 2;
            swap(buff[0] , buff[min]);
        }
    }
    return result;
}

int main() {
    int n = 0;
    cin >> n;

    int* a = new int[n];
    for(int i = 0; i < n; i++) cin >> a[i];

    Heap h(a, n);

    int result = 0;
    int buff = 0; // сумма двух
    while(!h.isEmpty()){
        buff = h.get_min() + h.get_min();
        h.add(buff);
        result += buff;
    }
    if(n == 1) result = a[0];

    cout << result;

    delete (a);

    return 0;
}

// 1 2 3 4 5 6
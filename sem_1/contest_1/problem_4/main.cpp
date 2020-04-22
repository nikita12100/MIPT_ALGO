/*
 * Реализовать очередь с динамическим зацикленным буфером.
Обрабатывать команды push back и pop front.
Формат ввода

В первой строке количество команд n. n ≤ 1000000.
Каждая команда задаётся как 2 целых числа: a b.
a = 2 - pop front
a = 3 - push back
Если дана команда pop front, то число b - ожидаемое значение. Если команда pop front вызвана для пустой структуры данных, то ожидается “-1”.
Формат вывода

Требуется напечатать YES - если все ожидаемые значения совпали. Иначе, если хотя бы одно ожидание не оправдалось, то напечатать NO.
 */

#include <iostream>

using namespace std;

class Queue{
public:
    Queue(int _buff_size): buff_size(_buff_size), tail(0) , first(0){
        buff = new int[buff_size];
    }

    void push_back(int a);
    int pop_front();
    void increase();

private :
    int* buff;
    int buff_size;
    int tail; // последний эл.
    int first;
};

void Queue::push_back(int a){
    if((tail + 1) % buff_size == first) this->increase();
    buff[tail] = a;
    tail = (tail + 1) % buff_size;
}

int  Queue::pop_front(){
    if(first == tail) return  -1;
    else {
        int result = buff[first];
        first = (first + 1) % buff_size;
        return result;
    }
}

void Queue::increase(){
    buff_size *= 2;
    int* new_buff = new int[buff_size];
    if(tail + 1 == first){
        for(int i = (first /*- 1*/); i < buff_size / 2 ; i++){
            new_buff[i - (first /*- 1*/)] = buff[i];
        }
        for(int i = 0; i < first; i++){
            new_buff[i + (buff_size/2 - first/*0first - 1*/)] = buff[i];
        }
        first = 0;
        tail = buff_size / 2;
    }
    else for(int i = 0; i < buff_size / 2; i++){
            new_buff[i] = buff[i];
        }
    delete[] (buff);
    buff = new_buff;
}

int main() {
    int n = 0;
    cin >> n;

    int *a = new int[n];
    int *b = new int[n];
    for(int i = 0; i <n; i++){
        cin >> a[i] >> b[i];
    }
    Queue q(40);

    bool waiting_result = true;

    for(int i = 0; i <n; i++){
        if(a[i] == 3) q.push_back(b[i]);
        if(a[i] == 2) if(q.pop_front() == b[i]) waiting_result = true;
            else {
                waiting_result = false;
                break;
            }
    }

    if(waiting_result) cout << "YES";
    else cout << "NO";

    delete[] a;
    delete[] b;

    return 0;
}
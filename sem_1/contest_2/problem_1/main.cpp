/*
Напишите программу, печатающую набор строк в лексикографическом порядке.
Строки разделяются символом перевода строки ’\n
’. Если последний символ в потоке ввода ’\n
’, считать, что после него нет пустой строки. Максимальная длина строки 255 символов.
Написать свою функцию сравнения строк.
*/
#include <iostream>

using namespace std;

bool isLess(const string &a, const string &b){
    for (int i = 0; i < a.length(); ++i) {
        if(a[i] > b[i]) return  false;
        if(a[i] < b[i]) return true;
    }
    //return true;
}

template <class T>
void Sort(T* str, int n, bool (*myCmp)(const T&, const T&)){
    T tmp;
    int i = n - 1;
    tmp = str[i];
    int j = i - 1;
    for (; j >= 0 && myCmp(tmp , str[j]) ; --j) {
        str[j + 1] = str[j];
    }
    str[j + 1] = tmp;
}

int main() {

    int n = 0;
    cin >> n;
    string* s = new string[n];
    for (int i = 0; i < n; ++i) {
        cin >> s[i];
        Sort(s, (i + 1), isLess);
    }

    for (int i = 0; i < n; ++i) {
        cout << s[i] << endl;
    }
    delete[] s;
    return 0;
}
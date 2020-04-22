/*
 * Дан фрагмент последовательности скобок, состоящей из символов (){}[].
 * Требуется определить, возможно ли продолжить фрагмент в обе стороны,
 * получив корректную последовательность. Длина исходной последовательности ≤ 800000.
Формат ввода

Строка, содержащая символы (){}[] и, возможно, перевод строки.
Формат вывода

Если возможно - вывести минимальную корректную последовательность, иначе - напечатать "IMPOSSIBLE".

 */

#include <iostream>
#include <string>

using namespace std;

class Stack{
public:
    Stack(int _buffer_size);

    void push(char a);
    char pop();
    char get_last();
    bool is_empty(){ return top == 0; }

private:
    int buffer_size;
    char* buff;
    int top;
    void increase();
};

Stack::Stack(int _buffer_size):top(0),buffer_size(_buffer_size) {
    buff = new char[buffer_size];
    for(int i = 0;  i < buffer_size; i++) buff[i] = 0;
}

void Stack::increase() {
    buffer_size *=2;
    char* new_buff = new char[buffer_size];
    for(int i = 0; i < buffer_size/2; i++) new_buff[i] = buff[i];
    delete[] buff;
    buff = new_buff;
}

void Stack::push(char a){
    if(top + 1 >= buffer_size) this->increase();
    buff[top] = a;
    top++;
}
char Stack::pop(){
    if(!is_empty()) {
        top--;
        char last = buff[top];
        buff[top] = 0;
        return last;
    }
}

char Stack::get_last(){
    return buff[top-1];
}

string Build_correct(Stack* stack , string raw){

    char element = stack->pop();
    string result = raw;
    string begin_raw;

    while(!((element == ')')|| (element == ']') || (element == '}')) ){
        switch(element){
            case '{':
                result += '}';
                break;
            case '[':
                result += ']';
                break;
            case '(':
                result += ')';
                break;
        }
        if(stack->is_empty()) break;
        element = stack->pop();
    }

    while(1){
        switch(element){
            case '}':
                begin_raw +='{';
                break;
            case ']':
                begin_raw +='[';
                break;
            case ')':
                begin_raw +='(';
                break;
        }
        if(stack->is_empty()) break;
        element = stack->pop();
    }

    result = begin_raw + result;

    return  result;
}

int main() {
    string s;
    cin >> s;
    Stack* st = new Stack(20);

    for(int i = 0; i < s.length(); i++ ){
        switch(s[i]){
            case '{':
                st->push('{');
                break;
            case '[':
                st->push('[');
                break;
            case '(':
                st->push('(');
                break;
            case '}':
                if(st->get_last() == '{') {
                    st->pop();
                }
                else {
                    if(st->get_last() == '[' || st->get_last() == '(' ){
                        cout << "IMPOSSIBLE";
                        return 0;
                    }
                    st->push('}');
                }
                break;
            case ']':
                if(st->get_last() == '[' ) {
                    st->pop();
                }
                else{
                    if(st->get_last() == '{' || st->get_last() == '(' ){
                        cout << "IMPOSSIBLE";
                        return 0;
                    }
                    st->push(']');
                }
                break;
            case ')':
                if(st->get_last() == '(') {
                    st->pop();
                }
                else{
                    if(st->get_last() == '[' || st->get_last() == '{' ){
                        cout << "IMPOSSIBLE";
                        return 0;
                    }
                    st->push(')');
                }
                break;
        }
    }

    cout << Build_correct(st , s) << endl;

    delete (st);

    return 0;
}
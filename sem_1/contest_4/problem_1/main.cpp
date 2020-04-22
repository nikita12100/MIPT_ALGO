/*
 * Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией.
 * Хранимые строки непустые и состоят из строчных латинских букв.
 * Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
 * Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте при добавлении элементов в случае,
 * когда коэффициент заполнения таблицы достигает 3/4. Структура данных должна поддерживать операции добавления строки в множество,
 * удаления строки из множества и проверки принадлежности данной строки множеству. 1_1. Для разрешения коллизий используйте квадратичное пробирование.
 * i-ая проба g(k, i)=g(k, i-1) + i (mod m). m - степень двойки. 1_2. Для разрешения коллизий используйте двойное хеширование.
*/
#include <iostream>
#include <vector>

using namespace std;

// Hash_String/////////
int Hash_First(string & key, int size) {
  int h = 0;
  for (char i : key) {
    h = (h * 13 + i) % size;
  }
  return h;
}
int Hash_Func(int h1, int attempt, int size) {
  if (attempt == 0) return h1;
  else
    return (Hash_Func(h1, attempt - 1, size) + attempt) % size;
}
//********************///

template <typename KeyType>
class Hash_Table {
 public:
  Hash_Table() : table(8), size(0) {
    for (auto &i:table) {
      i = EMPTY;
    }
  }

  bool insert(KeyType & key);
  bool erase(KeyType & key);
  bool check(KeyType & key);

 private:

  vector<KeyType> table;
  int size;

  const KeyType DEL = "DELETED";
  const KeyType EMPTY = "EMPTY";

  void reHash();
  void Delete_Table();
};
template <typename KeyType>
void Hash_Table<KeyType>::reHash() {
  vector<KeyType> buffer(table.size() * 2);
  for (auto &i:buffer){
    i = EMPTY;
  }
  for (auto &i : table) {
    if (i == EMPTY || i == DEL) continue;
    int attempt = 0;
    int h1 = Hash_First(i, buffer.size());
    int h = Hash_Func(h1, attempt++, buffer.size());
    while (attempt < buffer.size() && buffer[h] != EMPTY) {
      h = Hash_Func(h1, attempt++, buffer.size());
    }
    buffer[h] = i;
  }
  table = buffer;
}

template <typename KeyType>
bool Hash_Table<KeyType>::insert(KeyType & key) {
  int attempt = 0;
  if ((double) size / table.size() >= 0.75) reHash();
  int h1 = Hash_First(key, table.size());
  int h = Hash_Func(h1, attempt, table.size());
  while (table[h] != EMPTY && attempt < table.size()) {
    if (table[h] == key) return false;
    if (table[h] == DEL) {
      table[h] = key;
      return true;
    }
    h = Hash_Func(h1, ++attempt, table.size());
  }
  if (attempt == table.size()) return false;
  table[h] = key;
  size++;
  return true;
}

template <typename KeyType>
bool Hash_Table<KeyType>::erase(KeyType & key) {
  int h1 = Hash_First(key, table.size());
  int attempt = 0;
  int h = Hash_Func(h1, attempt++, table.size());
  while (table[h] != EMPTY && table[h] != DEL && table[h] != key && attempt < table.size()) {
    h = Hash_Func(h1, attempt++, table.size());
  }
  if (table[h] == EMPTY || table[h]== DEL) return false;
  table[h] = DEL;
  size--;
  return true;
}

template <typename KeyType>
bool Hash_Table<KeyType>::check(KeyType & key) {
  int attempt = 0;
  int h1 = Hash_First(key, table.size());
  int h = Hash_Func(h1, attempt++, table.size());
  while (table[h] != EMPTY && table[h] != DEL && table[h] != key && attempt < table.size()) {
    h = Hash_Func(h1, attempt++, table.size());
  }
  if (table[h] == EMPTY || table[h] == DEL) return false;
  return (table[h] == key);
}


void status(bool answer) {
  if(answer) cout << "OK" << '\n';
  else cout << "FAIL" << '\n';
}

int main() {
  Hash_Table<string> table;
  char comm;
  string key;
  while (cin >> comm >> key) {
    switch (comm) {
      case '+': {
        status(table.insert(key));
        break;
      }
      case '-': {
        status(table.erase(key));
        break;
      }
      case '?': {
        status(table.check(key));
        break;
      }
      default:
        break;
    }
  }
  return 0;
}
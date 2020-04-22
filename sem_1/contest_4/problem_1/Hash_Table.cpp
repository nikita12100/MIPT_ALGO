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

using namespace std;

// Hash_String/////////
int Hash_First(string & key, const int size) {
  int h = 0;
  for (auto i:key) {
    h = (h * 137 + i) % size;
  }
  return h;
}
int Hash_Second(string & key, const int size) {
  int h = 0;
  for (auto i:key) {
    h = (h * 23 + i) % size;
  }
  return h;
}
int Hash_Func(const int h1, const int h2, const int attempt, const int size) {
  return ((h1 + attempt*h2 ) % size);
}
//********************///

template <typename KeyType>
class Hash_Table {
 public:
  Hash_Table();
  ~Hash_Table() { delete[] table; }

  bool insert(KeyType & key);
  bool erase(KeyType & key);
  bool check(KeyType &key);

 private:

  const KeyType DELETED = "DELETED";
  const KeyType EMPTY = "EMPTY";

  int tableSize;
  KeyType * table;
  int size;

  void ReHash();
};

template <typename KeyType>
Hash_Table<KeyType>::Hash_Table() : tableSize(8), size(0) {
  table = new KeyType[tableSize];
  for (int i = 0; i < tableSize; ++i) {
    table[i] = EMPTY;
  }
}

template <typename KeyType>
void Hash_Table<KeyType>::ReHash() {
  tableSize *= 2;
  auto buffer = new KeyType[tableSize];
  for (int i = 0; i < tableSize; ++i)
    buffer[i] = EMPTY;
  for (int i = 0; i < tableSize / 2; ++i) {
    if (table[i] == EMPTY || table[i] == DELETED) continue;
    int h1 = Hash_First(table[i], tableSize);
    int h2 = Hash_Second(table[i], tableSize);
    int attempt = 0;
    int h = Hash_Func(h1, h2, attempt++, tableSize);
    while (buffer[h] != EMPTY) {
      h = Hash_Func(h1, h2, attempt++, tableSize);
    }
    buffer[h] = table[i]; // попали в хэш и записали
  }
  delete[] table;
  table = buffer;
}

template <typename KeyType>
bool Hash_Table<KeyType>::insert(KeyType & key) {
  if ((double) size / tableSize >= 0.75) ReHash();
  int h1 = Hash_First(key, tableSize);
  int h2 = Hash_Second(key, tableSize);
  int attempt = 0;
  int h = Hash_Func(h1, h2, attempt, tableSize);
  while (table[h] != EMPTY && attempt < tableSize) {
    if (table[h] == key) return false;      // эл-т уже есть
    h = Hash_Func(h1, h2, ++attempt, tableSize);
  }
  if (attempt >= tableSize) return false;
  table[h] = key;
  size++;
  return true;
}

template <typename KeyType>
bool Hash_Table<KeyType>::erase(KeyType & key) {
  int h1 = Hash_First(key, tableSize);
  int h2 = Hash_Second(key, tableSize);
  int attempt = 0;
  int h = Hash_Func(h1, h2, attempt++, tableSize);
  while (table[h] != EMPTY && table[h] != key && attempt < tableSize) {
    h = Hash_Func(h1, h2, attempt++, tableSize);
  }
  if (table[h] == EMPTY) return false;     // если эл-та нет
  table[h] = DELETED;
  return true;
}

template <typename KeyType>
bool Hash_Table<KeyType>::check(KeyType & key) {
  int h1 = Hash_First(key, tableSize);
  int h2 = Hash_Second(key, tableSize);
  int attempt = 0;
  int h = Hash_Func(h1, h2, attempt++, tableSize);
  while (table[h] != EMPTY && table[h] != key && attempt < tableSize) {
    h = Hash_Func(h1, h2, attempt++, tableSize);
  }
  if (table[h] == EMPTY) return false;
  return (table[h] == key);
}


void print(bool answer) {
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
        print(table.insert(key));
        break;
      }
      case '-': {
        print(table.erase(key));
        break;
      }
      case '?': {
        print(table.check(key));
        break;
      }
      default:
        break;
    }
  }
  return 0;
}
/*
 * Дано N кубиков. Требуется определить каким количеством способов можно выстроить из этих кубиков пирамиду.
 */

#include <iostream>

using namespace std;

//dp[n][k] = dp[n][k-1]+dp[n-k][k-1]

int64_t Pyramids_count(int64_t **& table, int n, int k) { // n - кол-во кубиков, k - размер основания
  if(k <= 0 || n <= 0) {
    table[n][k] = 0;
    return 0;
  }
  if(k == 1 && n != 1) {
    table[n][k] = 0;
    return 0;
  }
  if(k == 2 && n > 0){
    table[n][k] = 1;
    return 1;
  }
  if(n <= 2) {
    table[n][k] = 1;
    return 1;
  }

  if (table[n][k] >= 0)  return table[n][k];
  table[n][k] = Pyramids_count(table, n, k - 1) + Pyramids_count(table, n - k, k - 1);
  return table[n][k];
}

void Make_Table(int64_t cubes, int64_t **& table) {
  table = new int64_t *[cubes + 1];
  for (int64_t i = 0; i < cubes + 1; ++i) {
    table[i] = new int64_t[cubes + 1];
    for (int64_t j = 0; j < cubes + 1; ++j) {
      table[i][j] = -1;
    }
  }
}

void Delete_Table(int64_t **& table,int cubes){
  for (int i = 0; i < cubes + 1; ++i) {
    delete[] table[i];
  }
  delete[] table;
}

int main() {
  int64_t cubes = 0;
  cin >> cubes;

  int64_t ** table = nullptr;
  Make_Table(cubes, table);

  cout << Pyramids_count(table, cubes, cubes);

  cout << '\n';
  for (int i = 1; i < cubes + 1; ++i) {
    for (int j = 1; j < cubes + 1; ++j) {
      if(table[i][j] > 0)   cout << table[i][j] << "  |  " ;
      else cout << ++table[i][j] << "  |  ";
    }
    cout << '\n';
  }

  Delete_Table(table, cubes);

  return 0;
}
#include <iostream>
using namespace std;

//dp[n][k] = dp[n][k-1]+dp[n-k][k-1]
//в оновании нет k(<k) кубиков
//ровно k в основании

long countPyramids(long **&table, long cubes, long base) { // cubes - кол-во кубиков, base - размер основания
  if (base < 0 || cubes < 0) { return 0; }
  if (base == 1 && cubes == 1) { return 1; }
  if (base == 1) { return 0; }
  if (cubes < 3) { return 1; }
  if (table[cubes][base] >= 0) { return table[cubes][base]; }
  table[cubes][base] = countPyramids(table, cubes, base - 1) + countPyramids(table, cubes - base, base - 1);
  return table[cubes][base];
}

long **makeTable(long cubesCount) {
  long **table = new long *[cubesCount + 1];
  for (long i = 0; i < cubesCount + 1; i++) {
    table[i] = new long[cubesCount + 1];
    for (long j = 0; j < cubesCount + 1; j++) {
      table[i][j] = -1;
    }
  }
  return table;
}

int main() {
  long cubesCount;
  cin >> cubesCount;
  long **table = makeTable(cubesCount);
  cout << countPyramids(table, cubesCount, cubesCount);
  return 0;
}

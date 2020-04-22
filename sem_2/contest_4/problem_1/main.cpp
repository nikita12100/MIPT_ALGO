#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

class RSMQ {
 public:
  explicit RSMQ(const std::vector<int> &sequence_);

  int operator()(int a, int b);

 private:
  //компаратор, не возвр дублрующиея элементы
  std::pair<int, int> getMinPair(const std::pair<int, int> &a, const std::pair<int, int> &b);

  std::vector<int> sequence;    //  храним последовательность
  unsigned int size_seq;    //  размер последовательности
  std::vector<int> log2_;    // храним лгирифмы по осн.2, чтобы не вычислять часто
  std::vector<std::vector<std::pair<int, int>>> sparseTable;    //  сама таблица
};

RSMQ::RSMQ(const std::vector<int> &sequence_) :
    sequence(sequence_),
    size_seq(sequence.size()),
    sparseTable((int) ceil(log2(size_seq)),
                std::vector<std::pair<int, int>>(size_seq,
                                                 std::pair<int, int>(0, 0)))   //  наименьшее целое от логарифма размера (int)
{
  for (int i = 0; i <= size_seq; ++i)   //  заполняем "высоту" таблицы
    log2_.push_back((int) ceil(log2(i)));

  for (int i = 0; i < size_seq; ++i) {    // иницализируем степень 0 (саму последовательность)
    sparseTable[0][i].first = sequence[i];
    sparseTable[0][i].second = sequence[i];
  }

  for (int i = 1; i < sparseTable.size(); ++i) {
    for (int j = 0; j < size_seq + 1 - (1 << i); ++j) {   //  бегаем по степеням двойки
      std::pair<int, int> min = getMinPair(sparseTable[i - 1][j],
                                           sparseTable[i - 1][j + (1 << (i - 1))]);
      sparseTable[i][j].first = min.first;
      sparseTable[i][j].second = min.second;
    }
  }
}

//  возвращает вторую статистику на [a, b]
int RSMQ::operator()(int a, int b) {
  int hightSP = log2_[b - a + 1] - 1;   //  высота нужного сегмента таблицы

  return getMinPair(sparseTable[hightSP][a], sparseTable[hightSP][b - (1 << hightSP) + 1]).second;
}

std::pair<int, int> RSMQ::getMinPair(const std::pair<int, int> &a, const std::pair<int, int> &b) {
  std::pair<int, int> result;

  std::vector<int> order{a.first, a.second, b.first, b.second};
  std::sort(order.begin(), order.end());
  result.first = order[0];

  if (order[0] != order[1])
    result.second = order[1];
  else if (order[0] != order[2])
    result.second = order[2];
  else
    result.second = order[3];

  return result;
}

int main() {
  int n, m = 0;
  std::cin >> n >> m;

  std::vector<int> sequence(n);   // наша последовтельность
  for (int i = 0; i < n; ++i) {
    int x = 0;
    std::cin >> x;
    sequence[i] = x;
  }

  RSMQ rsmq(sequence);    // range second minimum query

  for (int i = 0; i < m; ++i) {
    int a, b = 0;
    std::cin >> a >> b;
    std::cout << rsmq(a, b) << '\n';
  }

  return 0;
}
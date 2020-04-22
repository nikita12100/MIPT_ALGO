#include "MST.h"

Graph MST(const Graph &graph) {
  unsigned int n = graph.GetVerticesNumber();
  vector<pair<unsigned int, double >> key(n, pair<unsigned int, double>(INT32_MAX, -1));  // храним вершины и их веса
  vector<bool> used(n, false);  // использованные вершины для обхода

  set<pair<double, unsigned int>> queue;  //  куча

  key[0].second = 0;
  for (int i = 0; i < n; ++i)
    queue.emplace(key[i].second, i);

  // Алгоритм Прима
  while (!queue.empty()) {
    unsigned int current = queue.begin()->second;
    queue.erase(queue.begin());

    used[current] = true;

    auto related = graph.GetRelated(current);
    for (auto ch : related) {
      if (!used[ch.first] && key[ch.first].second > ch.second
          || key[ch.first].second + 1 < 0.0001) {
        queue.erase(pair<double, unsigned int>(key[ch.first].second, ch.first));
        key[ch.first].first = current;
        key[ch.first].second = ch.second;
        queue.emplace(key[ch.first].second, ch.first);
      }
    }
  }

  Graph result(graph.GetVerticesNumber());
  for (unsigned int i = 1; i < n; i++)
    result.AddEdge(i, key[i].first, key[i].second);

  return result;
}
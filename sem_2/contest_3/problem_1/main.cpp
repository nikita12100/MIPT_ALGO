#include <iostream>
#include <list>
#include <vector>
#include <set>
#include <climits>
#include <cstdint>

using std::vector;
using std::pair;
using std::make_pair;

class Graph {
 public:
  explicit Graph(unsigned verticesNumber);

  void Add_Edge(unsigned from, unsigned to, unsigned weight);

  vector<pair<unsigned, unsigned>> Get_Related(unsigned vertex) const;

  unsigned Get_Size() const;

 private:
  unsigned size;
  vector<std::list<pair<unsigned, unsigned>>> edges;
};

Graph::Graph(unsigned verticesNumber) :
    size(verticesNumber),
    edges(verticesNumber, std::list<pair<unsigned, unsigned>>()) // заполнили путсыми листами
{}

void Graph::Add_Edge(unsigned from, unsigned to, unsigned weight) {
  edges[from].emplace_back(make_pair(to, weight));
  edges[to].emplace_back(make_pair(from, weight));  // неор граф
}

vector<pair<unsigned, unsigned>> Graph::Get_Related(unsigned vertex) const {
  vector<pair<unsigned, unsigned>> related;
  for (auto i:edges[vertex])
    related.emplace_back(i);

  return related;
}

unsigned Graph::Get_Size() const {
  return size;
}

// С помощью алгоритма Крускала.
unsigned MST_Weight(const Graph &graph) {
  unsigned const n = graph.Get_Size();

  vector<unsigned> key(n, INT_MAX);  //минимальные веса из исп в неисп, для[i]
  vector<bool> used(n, false);   // посезена ли вершина[i]

  std::set<pair<unsigned, unsigned>> queue; // очередь на посещение, куча чтоб брать минимальнный

  //начало
  key[0] = 0;
  for (int i = 0; i < n; ++i)
    queue.emplace(make_pair(key[i], i)); // добавляем все вершины, т.к. не факт что граф связный

  while (!queue.empty()) {
    unsigned current = queue.begin()->second;
    queue.erase(queue.begin());

    used[current] = true;   // пометили посещенной

    vector<pair<unsigned, unsigned >> related = graph.Get_Related(current);

    for (auto ch:related) { // child
      if (!used[ch.first] && key[ch.first] > ch.second) {
        queue.erase(queue.find(make_pair(key[ch.first],
                                         ch.first))); // ншли путь кароче и заменяем его (на вес ребра ch)
        key[ch.first] = ch.second;
        queue.emplace(key[ch.first], ch.first);    // добавили в очередь
      }
    }
  }

  unsigned way_weight = 0;
  for (auto i:key) {
    way_weight += i; // добавляем все веса, т.к. веса есть только у м.с.т.
  }

  return way_weight;
}

int main() {
  unsigned n, m = 0;
  std::cin >> n >> m;
  Graph graph(n);
  for (int i = 0; i < m; ++i) {
    unsigned x, y, w = 0;
    std::cin >> x >> y >> w;
    graph.Add_Edge(x - 1, y - 1, w);
  }

  std::cout << MST_Weight(graph);

  return 0;
}
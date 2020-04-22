#include <iostream>
#include <list>
#include <vector>
#include <queue>

using std::vector;
using std::list;
using std::pair;
using std::make_pair;


class Graph {
 public:
  explicit Graph(unsigned n);

  void AddEdge(unsigned from, unsigned to, unsigned weight);
  void IncreaseWeight(unsigned from, unsigned to, int new_weight);
  vector<pair<unsigned, unsigned>> GetRelated(unsigned vertex);
  unsigned GetSize() const;

 private:
  unsigned size;
  vector<vector<unsigned>> edges;

};

Graph::Graph(unsigned n) :
    size(n),
    edges(n, vector<unsigned>(n, 0)) {}

void Graph::AddEdge(unsigned from, unsigned to, unsigned weight) {
  edges[from][to] = weight;
}

void Graph::IncreaseWeight(unsigned from, unsigned to, int new_weight) {
  edges[from][to] += new_weight;
}

vector<pair<unsigned, unsigned>> Graph::GetRelated(unsigned vertex) {
  vector<pair<unsigned, unsigned>> related;
  for(unsigned i = 0; i < size; ++i)
    if(edges[vertex][i] != 0)
      related.emplace_back(make_pair(i, edges[vertex][i]));

  return related;
}

unsigned Graph::GetSize() const {
  return size;
}


///////////////---------------------------Max_Flow--------------------------//////////


// Алгоритм Эдмондса — Карпа O(VE^2), поиск в ширину
class Max_Flow {    // функциональный объект (функтор)
 public:
  Max_Flow() : web(0), target(0) {}

  unsigned operator()(const Graph &graph, unsigned from, unsigned to);

 private:
  unsigned increaseFlow(unsigned from);   // итерация

  Graph web;        // остаточый граф
  unsigned target;  // до куда будем искать

};

unsigned Max_Flow::operator()(const Graph &graph, unsigned from, unsigned to) {
  web = graph;
  target = to;

  unsigned current_flow = 0;     // поток на текущей итерации
  unsigned result_flow = 0;      // итоговый поток

  do {  // пока можем наращивать поток, наращваем
    current_flow = increaseFlow(from);
    result_flow += current_flow;

  } while (current_flow != 0);

  return result_flow;
}

unsigned Max_Flow::increaseFlow(unsigned from) {
  vector<pair<unsigned, unsigned>> parents(web.GetSize()); // вершина, родитель, вес ребра
  for (unsigned i = 0; i < web.GetSize(); ++i) {
    parents[i].first = INT32_MAX;
    parents[i].second = INT32_MAX;
  }

  parents[from].first = from;
  parents[from].second = 0;

  //BFS

  std::queue<unsigned> queue;
  queue.emplace(from);
  while(!queue.empty()){
    unsigned current_vertex = queue.front();
    queue.pop();
    if(current_vertex == target)
      break;
    vector<pair<unsigned, unsigned>> related = web.GetRelated(current_vertex);

    for (auto i:related) {
      if(parents[i.first].first == INT32_MAX){
        parents[i.first].first = current_vertex;
        parents[i.first].second = i.second;
        queue.push(i.first);
      }
    }
  }

  // не дастигли после обхода, знаит нет пути вообще
  if(parents[target].first == INT32_MAX)
    return 0;

  unsigned flow = INT32_MAX;
  unsigned current = target;
  while (current != from){
    if(parents[current].second < flow)
      flow = parents[current].second;    // записали все веса ребер
    current = parents[current].first;
  }

  current = target;

  while (current != from){
    web.IncreaseWeight(parents[current].first, current, -flow);
    web.IncreaseWeight(current, parents[current].first, flow);
    current = parents[current].first;
  }

  return flow;
}

///////////////-------------------------------------/////////////////////////////

int main() {
  unsigned n, m = 0;
  std::cin >> n >> m;
  Graph graph(n);
  for (int i = 0; i < m; ++i) {
    unsigned x, y, w = 0;
    std::cin >> x >> y >> w;
    graph.AddEdge(x - 1, y - 1, w);
  }

  Max_Flow max_flow;
  std::cout << max_flow(graph, 0, n - 1);

  return 0;
}
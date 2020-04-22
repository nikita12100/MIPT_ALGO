#include <iostream>
#include <vector>
#include <unordered_set>
#include <queue>
#include <memory>
#include <list>
#include <cmath>

using std::vector;

class Graph {
 public:
  explicit Graph(unsigned int verticesNumber);

  // Добавление ребра от from к to.
  void AddEdge(int from, int to);

  int VerticesCount() const;

  void GetNextVertices(int vertex, std::vector<int> &vertices) const;

  int Count_Ways(int from_, int to_);   // length way

 private:

  struct Edge {
    Edge():deep(0), path(0) {};


    int path;     //кол-во входящх путей
    int deep;   // глубина для подсчета длиы пути

  };

  unsigned int verticesNumber_;

  vector<std::list<int>> vertexs_; // вершины графа

};

Graph::Graph(unsigned int verticesNumber) :
    verticesNumber_(verticesNumber),
    vertexs_(verticesNumber)
{}

// Добавление ребра от from к to
void Graph::AddEdge(int from, int to) {
//  for(int i:vertexs_[from])
//    if(i == to)
//      return; //если ребро уже есть, чтобы не создавать копию

  vertexs_[to].push_back(from);
  vertexs_[from].push_back(to);
}

int Graph::VerticesCount() const {
  return verticesNumber_;
}

void Graph::GetNextVertices(int vertex, std::vector<int> &vertices) const {
  vertices.clear();
  for(auto i : vertexs_[vertex])
    vertices.push_back(i);
}

int Graph::Count_Ways(int from_, int to_) {

  vector<Edge> vertex_states(verticesNumber_);  // список пройденых, чтоьбы смотреть глбин и кол-во путей
  std::queue<int> vertex_queue;    // очередь на обход
  vertex_queue.push(from_);      // начальный эл-т
  vertex_states[from_].path = 1;

  while (!vertex_queue.empty()) {
    vector<int> vertices;      // следующие вершины
    int current_vertex = vertex_queue.front();
    vertex_queue.pop();
    GetNextVertices(current_vertex, vertices);

    for (int j:vertices) {
      if (vertex_states[j].path == 0)  {
        vertex_queue.push(j);      // добавляем в очередь обхода
        vertex_states[j].deep = vertex_states[current_vertex].deep + 1; // увеличиваем глубину
        vertex_states[j].path = vertex_states[current_vertex].path;     // вход такой же
      }
      else {
        if(vertex_states[j].deep == vertex_states[current_vertex].deep + 1)  // если "слияний" путей, т.к. обход BFS
          vertex_states[j].path += vertex_states[current_vertex].path;    // увеличиныем счетчик путей
      }
    }
  }
  return vertex_states[to_].path;
}
/////////////////////////////////_____________GRAPH_______________///////////////////////////////////


double Min_space(unsigned int right, unsigned int left, unsigned int radius, const vector<std::pair<int, int>> & columns){
  unsigned int points_count = columns.size();
  double width = right - left;
  double min_space = 0;

  while(width - min_space > 5e-6){  // до 5*10^(-6)
    double new_min_space = (min_space + width) / 2;  // на каждом шаге делим попалам
    Graph graph(points_count + 2);   // две фиктивные вершины для проверки расстояния до краев

    for (int i = 0; i < points_count; ++i) {
      for (int j = 0; j < points_count; ++j) {
        if(sqrt(pow(columns[i].first - columns[j].first, 2) + pow(columns[i].second - columns[j].second, 2)) - 2*radius < new_min_space)
          graph.AddEdge(i, j); // добавляем все вершины дист. меж. кот. < ширина/2
      }
    }

    for (int i = 0; i < points_count; ++i) {
      if(columns[i].first - radius  - left < new_min_space)
        graph.AddEdge(i, points_count);    // мно-во вершин  дист. от кот. до левой грани < ширины

      if(right - columns[i].first - radius < new_min_space)
        graph.AddEdge(i, points_count + 1);    // мно-во вершин  дист. от кот. до правой грани < ширины
    }
// елси сущ путь, то раст лев-т1, т1-т2 и т2-прав < ширина (не влезет)
    if(graph.Count_Ways(points_count, points_count + 1) == 0)
      min_space = new_min_space;
    else
      width = new_min_space;
  }
  return width;
}


int main() {
  unsigned int left, right = 0;
  std::cin >> left >> right;

  unsigned int radius = 0;
  std::cin >> radius;

  unsigned int n;
  std::cin >> n;

  vector<std::pair<int, int>> columns(n, std::pair<int, int>(0, 0));   //колоны
  for (int i = 0; i < n; ++i) {
    std::cin >> columns[i].first >> columns[i].second;
  }

  std::cout.precision(3);
  std::cout << std::fixed;
  std::cout << Min_space(right, left, radius, columns);

  return 0;
}
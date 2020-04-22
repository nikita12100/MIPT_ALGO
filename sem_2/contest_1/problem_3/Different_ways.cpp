#include <iostream>
#include <vector>
#include <unordered_set>
#include <queue>
#include <memory>
#include <list>

using std::vector;

struct IGraph {

  virtual ~IGraph() {}

  // Добавление ребра от from к to.
  virtual void AddEdge(int from, int to) = 0;

  virtual int VerticesCount() const = 0;

  virtual void GetNextVertices(int vertex, std::vector<int> &vertices) const = 0;
  virtual void GetPrevVertices(int vertex, std::vector<int> &vertices) const = 0;

};


class Graph: public IGraph {
 public:
  explicit Graph(unsigned int verticesNumber);
  explicit Graph(const IGraph* graph);

  // Добавление ребра от from к to.
  virtual void AddEdge(int from, int to) override;

  virtual int VerticesCount() const override;

  virtual void GetNextVertices(int vertex, std::vector<int> &vertices) const override;


 private:

  unsigned int verticesNumber_;

  vector<std::list<int>> vertexs_; // хештаблица вершин графа

};

  struct Edge {
    Edge():deep(0), path(0) {};

    int path;     //кол-во входящх путей
    int deep;   // глубина для подсчета длиы пути

  };

Graph::Graph(unsigned int verticesNumber) :
    verticesNumber_(verticesNumber),
    vertexs_(verticesNumber)
{}

Graph::Graph(const IGraph* graph):
    verticesNumber_(graph->VerticesCount()),
    vertexs_(verticesNumber_)
{
  vector<int> vertices;
  for(int i = 0; i < verticesNumber_; ++i){
    vertices.clear();
    graph->GetNextVertices(i, vertices);

    for(int j:vertices){
      AddEdge(i, j);
    }

  }
}

// Добавление ребра от from к to
void Graph::AddEdge(int from, int to) {
  for(int i:vertexs_[from])
    if(i == to)
      return; //если ребро уже есть, чтобы не создавать копию

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

int Count_Ways(IGraph* graph, int from_, int to_) {

  vector<Edge> vertex_states(graph->VerticesCount());  // список пройденых, чтоьбы смотреть глбин и кол-во путей
  std::queue<int> vertex_queue;    // очередь на обход
  vertex_queue.push(from_);      // начальный эл-т
  vertex_states[from_].path = 1;

  while (!vertex_queue.empty()) {
    vector<int> vertices;      // следующие вершины
    int current_vertex = vertex_queue.front();
    vertex_queue.pop();
    graph->GetNextVertices(current_vertex, vertices);

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

int main() {
  unsigned int n, m = 0;
  std::cin >> n >> m;
  std::shared_ptr<Graph> graph (new Graph(n));

  int x, y = 0;
  for (int i = 0; i < m; ++i) {
    std::cin >> x >> y;
    graph->AddEdge(x, y);
  }

  std::cin >> x >> y;

  std::cout << graph->Count_Ways(x, y);

  return 0;
}
/*

20 9

 1 2
 1 3
 2 4
 3 4
 4 5
 5 6
 5 8
 6 7
 8 7

 1 7

 */

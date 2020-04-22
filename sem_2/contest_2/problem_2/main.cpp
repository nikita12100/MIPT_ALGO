#include <iostream>
#include <vector>
#include <queue>

class Graph {
 public:
  explicit Graph(unsigned int verticesNumber);

  void AddEdge(int from, int to, double weight);

  int VerticesCount() const;

  void GetNext(int vertex, std::vector<int> & vertices) const;

  double GetWeight(int from, int to) const;

 private:

  unsigned int verticesNumber_;

  std::vector<std::vector<double>> matrix_;     //вершина , вес

};

Graph::Graph(unsigned int verticesNumber) :
    verticesNumber_(verticesNumber),
    matrix_(verticesNumber, std::vector<double>(verticesNumber, -1)) {}

// Добавление ребра от from к to
void Graph::AddEdge(int from, int to, double weight) {
  matrix_[from][to] = weight;
}

int Graph::VerticesCount() const {
  return verticesNumber_;
}

void Graph::GetNext(int vertex, std::vector<int> & vertices) const {
  vertices.clear();
  for (int i = 0; i < verticesNumber_; ++i)
    if(matrix_[vertex][i] != -1)
      vertices.push_back(i);
}

double Graph::GetWeight(int from, int to) const {
  return matrix_[from][to];
}

//обновление цены происходит во время обхода цила обмена по курсу валюты
bool arbitrage(const Graph & graph) {
  int n = graph.VerticesCount();

  std::vector<double> price(n , -1.0);

  for (int v = 0; v < n; ++v) {     // обновляем все цены
    std::queue<int> queue;
    queue.push(v);
    if(price[v] != -1) //   цена уже была в обходе
      continue;
    if(price[v] == -1.0) //   цена еще не была
      price[v] = 1.0;

    while (!queue.empty()) {
      int current_vertex = queue.front();
      queue.pop();

      std::vector<int> vertices;
      graph.GetNext(current_vertex, vertices);
      for (int i:vertices) {
        if (graph.GetWeight(current_vertex, i) == -1)   // если нет обмена
          continue;
        if (price[i] == -1.0) {      // цену еще не обновляли
          price[i] = price[current_vertex] * graph.GetWeight(current_vertex, i); // обновляем
          queue.push(i);              // добавляем в очередь но обход
        } else {
          if(price[i] < price[current_vertex] * graph.GetWeight(current_vertex, i)) // елси более выгодный курс
            price[i] = price[current_vertex] * graph.GetWeight(current_vertex, i);
          if (price[i] > 1.0)       // есть арбитраж
            return true;
        }
      }
    }
  }
  return false;
}

int main() {
  unsigned int n = 0;
  std::cin >> n;

  Graph graph(n);
  double w = 0;
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j) {
      if (i == j)
        continue;
      std::cin >> w;
      graph.AddEdge(i, j, w);
    }

  std::cout << ((arbitrage(graph)) ? "YES" : "NO");

  return 0;
}
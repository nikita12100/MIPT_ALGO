#include "Graph.h"

Graph::Graph(unsigned int verticesNumber_) :
    verticesNumber(verticesNumber_),
    matrix(verticesNumber, vector<double>(verticesNumber, 0.0)) {}

void Graph::AddEdge(unsigned int from, unsigned int to, double weight) {
  matrix[from][to] = weight;
  matrix[to][from] = weight;
}

unsigned int Graph::GetVerticesNumber() const {
  return verticesNumber;
}

vector<pair<unsigned int, double>> Graph::GetRelated(unsigned int vertex) const {
  vector<pair<unsigned int, double>> related(verticesNumber);
  for (unsigned i = 0; i < verticesNumber; ++i) {
    if (matrix[vertex][i] != 0)
      related.emplace_back(i, matrix[vertex][i]);
  }
  return related;
}

double Graph::GetWeight(unsigned int from, unsigned int to) const {
  return matrix[from][to];
}

void Graph::print() {
  for (int i = 0; i < verticesNumber; ++i) {
    for (int j = 0; j < verticesNumber; ++j) {
      std::cout << std::setw(7) << matrix[i][j] << ' ';
    }
    std::cout << '\n';
  }
}


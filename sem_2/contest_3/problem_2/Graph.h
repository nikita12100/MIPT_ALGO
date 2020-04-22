#ifndef TRAVELINGSSALESMAN_GRAPH_H
#define TRAVELINGSSALESMAN_GRAPH_H

#include <list>
#include <vector>
#include <set>
#include <climits>
#include <cstdint>
#include <iostream>
#include <iomanip>

using std::vector;
using std::pair;
using std::make_pair;
using std::min;

class Graph {
 public:
  explicit Graph(unsigned int verticesNumber_);

  void AddEdge(unsigned int from, unsigned int to, double weight);

  vector<pair<unsigned int, double >> GetRelated(unsigned int vertex) const;

  unsigned int GetVerticesNumber() const;

  double GetWeight(unsigned int from, unsigned int to) const;

  void print();

 private:
  unsigned int verticesNumber;
  vector<vector<double>> matrix;
};

#endif //TRAVELINGSSALESMAN_GRAPH_H

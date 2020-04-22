//
// Created by User on 013 13.02.18.
//

#ifndef SEM_2_1_CARCGRAPH_H
#define SEM_2_1_CARCGRAPH_H

#pragma once
#include "IGraph.h"

using std::pair;

// хранящий граф в виде одного массива пар {from, to}.

class CArcGraph:public IGraph{
 public:
  explicit CArcGraph(unsigned int verticesNumber);
  explicit CArcGraph(const IGraph* gr);


  // Добавление ребра от from к to.
  void AddEdge(int from, int to) override ;

  int VerticesCount() const ;

  void GetNextVertices(int vertex, std::vector<int> &vertices) const ;
  void GetPrevVertices(int vertex, std::vector<int> &vertices) const ;


 private:

  struct Edge{
    Edge() = default;
    Edge(int from_, int to_);

    int from;
    int to;
    bool visited;

  };


  unsigned int verticesNumber_;

  vector<Edge> graph_;

};

#endif //SEM_2_1_CARCGRAPH_H

//
// Created by User on 013 13.02.18.
//

#include "CArcGraph.h"

CArcGraph::CArcGraph(unsigned int verticesNumber):
  verticesNumber_(verticesNumber),
  graph_(verticesNumber)
{}

CArcGraph::CArcGraph(const IGraph* gr):
  verticesNumber_(gr->VerticesCount()),
  graph_(verticesNumber_)
{
  vector<int> vertices;
  for(int i = 0; i < verticesNumber_; ++i){
    vertices.clear();
    gr->GetNextVertices(i, vertices);

    for(int j:vertices){
      AddEdge(i, j);
    }

  }
}


// Добавление ребра от from к to.
void CArcGraph::AddEdge(int from, int to) {

  for(auto i : graph_)    // существует ли ребро ?
    if(i.from == from && i.to == to)
      return;

  graph_.push_back(Edge(from, to));

}

int CArcGraph::VerticesCount() const{
  return verticesNumber_;
}

void CArcGraph::GetNextVertices(int vertex, std::vector<int> &vertices) const{
  vertices.clear();
  for(auto i : graph_)
    if(i.from == vertex)
      vertices.push_back(i.to);
}

void CArcGraph::GetPrevVertices(int vertex, std::vector<int> &vertices) const{
  vertices.clear();
  for(auto i : graph_)
    if(i.to == vertex)
      vertices.push_back(i.from);
}

CArcGraph::Edge::Edge(int from_, int to_):
    from(from_),
    to(to_),
    visited(false)
{}

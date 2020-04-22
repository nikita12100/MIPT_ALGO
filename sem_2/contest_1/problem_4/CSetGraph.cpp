//
// Created by User on 017 17.02.18.
//

#include "CSetGraph.h"

CSetGraph::CSetGraph(unsigned int verticesNumber):
  verticesNumber_(verticesNumber),
  in_(verticesNumber_),
  out_(verticesNumber_)
{}

CSetGraph::CSetGraph(const CSetGraph* graph):
  verticesNumber_(graph->VerticesCount()),
  in_(verticesNumber_),
  out_(verticesNumber_)
{
  vector<int> vertices;
  for(int i = 0; i < verticesNumber_; ++i){
    vertices.clear();
    graph->GetNextVertices(i, vertices);
    for(int j:vertices)
      AddEdge(i, j);
  }
}

// Добавление ребра от from к to.
void CSetGraph::AddEdge(int from, int to){
  out_[from].insert(to);
  in_[to].insert(from);
}

int CSetGraph::VerticesCount() const{
  return verticesNumber_;
}

void CSetGraph::GetNextVertices(int vertex, std::vector<int> &vertices) const{
  vertices.clear();
  for(int i:out_[vertex]){
    vertices.push_back(i);
  }
}

void CSetGraph::GetPrevVertices(int vertex, std::vector<int> &vertices) const{
  vertices.clear();
  for(int i:in_[vertex]){
    vertices.push_back(i);
  }
}

bool CSetGraph::Is_stock(){
  if(out_.size() < in_.size())
    return true;
  else return false;
}
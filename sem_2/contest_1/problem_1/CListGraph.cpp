#include "CListGraph.h"

CListGraph::CListGraph(unsigned int verticesNumber) :
    verticesNumber_(verticesNumber),
    in_(verticesNumber),
    out_(verticesNumber) {}

CListGraph::CListGraph(const IGraph *graph) :
    verticesNumber_(graph->VerticesCount()),
    in_(verticesNumber_),
    out_(verticesNumber_)
{
  vector<int> vertices;
  for (int i = 0; i < verticesNumber_; ++i) {
    vertices.clear();
    graph->GetNextVertices(i, vertices);

    for (int j:vertices) {
      AddEdge(i, j);
    }

  }
}

void CListGraph::AddEdge(int from, int to) {
  if(in_.size() < out_.size()){
    for(int i:in_[to])
      if(i == from)
        return;
  }
  else{
    for(int i:out_[from])
      if(i == to)
        return;
  }

  in_[to].push_back(from);
  out_[from].push_back(to);
}

int CListGraph::VerticesCount() const {
  return verticesNumber_;
}

void CListGraph::GetNextVertices(int vertex, vector<int> & vertices) const {
  vertices.clear();
  for(int i:out_[vertex])
    vertices.push_back(i);
}

void CListGraph::GetPrevVertices(int vertex, vector<int> & vertices) const {
  vertices.clear();
  for(int i:in_[vertex])
    vertices.push_back(i);
}
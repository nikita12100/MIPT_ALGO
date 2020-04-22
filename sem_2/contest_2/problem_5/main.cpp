#include <vector>
#include <fstream>
#include <iostream>

using std::vector;
using std::min;


void Floyd(vector<vector<double>> &graph){
  const int n = graph.size();
  for(int k = 0; k < n; k++)
    for(int i = 0; i < n; i++)
      for(int j = 0; j < n; j++)
        graph[i][j] = min(graph[i][j], graph[i][k] + graph[k][j]);
}


int main() {
  unsigned n = 0;
  std::cin >> n;
  vector<vector<double>> graph(n, vector<double>(n, 0));
  for(int i = 0; i < n; i++)
    for(int j = 0; j < n; j++){
      int x = 0;
      std::cin >> x;
      graph[i][j] = x;
    }

  Floyd(graph);

  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++)
      std::cout << graph[i][j] << ' ';
    std::cout << '\n';
  }


  return 0;
}
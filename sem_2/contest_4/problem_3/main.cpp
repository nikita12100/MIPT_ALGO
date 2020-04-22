#include <iostream>
#include <vector>
#include <cmath>

class LCA {
 public:
  explicit LCA(const std::vector<std::vector<unsigned int>> &parents);    //строим дерво

  unsigned int operator()(std::pair<unsigned int, unsigned int> query);   // возвразает lca

 private:

  unsigned int  getLca(unsigned int a, unsigned int b);    // возвразает lca
  void dfs(unsigned int node, unsigned int parent);   // заполнение таблииы предков при обходе

  bool isAncestor(unsigned a, unsigned b);    // елси a родитель b

  unsigned int timer;   // используется в dfs
  unsigned int nodeCount;
  unsigned int largeAncTabl;    //размер табл предков

  const std::vector<std::vector<unsigned int>> tree;   //  зранит детей для i-й вершины
  std::vector<unsigned int> t_in;
  std::vector<unsigned int> t_out;
  std::vector<std::vector<unsigned int>> ancestors;   //  таблица предков
};

class NextQuery {
 public:
  NextQuery(unsigned int a1_,
            unsigned int b1_,
            unsigned long x_,
            unsigned long y_,
            unsigned long z_,
            unsigned int n_);

  std::pair<unsigned int, unsigned int> operator()(unsigned int result);

 private:
  unsigned int a;
  unsigned int b;
  unsigned long x;
  unsigned long y;
  unsigned long z;
  unsigned int n;
};
//-------------------------------REALIZATION---------------------------------

LCA::LCA(const std::vector<std::vector<unsigned int>> &parents) :
    tree(parents),
    nodeCount(parents.size()),
    largeAncTabl((ceil(log2(nodeCount)))),
    t_in(nodeCount),
    t_out(nodeCount),
    ancestors(nodeCount, std::vector<unsigned int>(largeAncTabl + 1)),
    timer(0)
{
  dfs(0, 0);
}

unsigned int LCA::getLca(unsigned int a, unsigned int b) {
  if (isAncestor(a, b))
    return a;
  if (isAncestor(b, a))
    return b;

  for (int i = largeAncTabl; i >= 0; --i)
    if (!isAncestor(ancestors[a][i], b))
      a = ancestors[a][i];

  return ancestors[a][0];
}

unsigned int LCA::operator()(std::pair<unsigned int, unsigned int> query) {
  return getLca(query.first, query.second);
}

void LCA::dfs(unsigned int node, unsigned int parent) {
  t_in[node] = timer++;
  ancestors[node][0] = parent;
  for (int i = 1; i <= largeAncTabl; ++i)
    ancestors[node][i] = ancestors[ancestors[node][i - 1]][i - 1];
  for (int i = 0; i < tree[node].size(); ++i) {
    unsigned int to = tree[node][i];
    if (to != parent)
      dfs(to, node);
  }

  t_out[node] = timer++;
}

bool LCA::isAncestor(unsigned a, unsigned b) {
  return t_in[a] <= t_in[b] && t_out[b] <= t_out[a];
}

//----------------------------------NextQuery---------------------------------

NextQuery::NextQuery(unsigned int a1_,
                     unsigned int b1_,
                     unsigned long x_,
                     unsigned long y_,
                     unsigned long z_,
                     unsigned int n_) :
    a(a1_),
    b(b1_),
    x(x_),
    y(y_),
    z(z_),
    n(n_) {}

std::pair<unsigned int, unsigned int> NextQuery::operator()(unsigned int result) {
  std::pair<unsigned int, unsigned int> ans((a + result) % n, b);
  a = (a * x + b * y + z) % n;
  b = (b * x + a * y + z) % n;

  return ans;
}

int main() {
  unsigned int n = 0;
  unsigned int m = 0;
  std::cin >> n >> m;

  std::vector<std::vector<unsigned int>> parents(n);  //  предки
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int x = 0;
    std::cin >> x;

    parents[x].push_back(i);
  }

  //запросы
  unsigned int a1 = 0;
  unsigned int b1 = 0;
  std::cin >> a1 >> b1;
  unsigned long x = 0;
  unsigned long y = 0;
  unsigned long z = 0;
  std::cin >> x >> y >> z;
  NextQuery nextQueryt(a1, b1, x, y, z, n);   //  исходная позиция

  LCA lca(parents);
  unsigned long ans = 0;
  unsigned long result = 0;
  for (int i = 0; i < m; ++i) {
    result = lca(nextQueryt(result));
    ans += result;
  }

  std::cout << ans;

  return 0;
}
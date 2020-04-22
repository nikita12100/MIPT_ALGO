#include <iostream>
#include <vector>

using namespace std;

class Tree {
 public:

  Tree(int n);

  void distance(vector<int> & out);
  void insert(int x, int y);

 private:

  struct Node {
    vector<Node *> children_;

    int i_;
    int depth_;
  };

  int MaxChildrenDepth(Node * node, int except);
  void FixDepth(Node * node);
  void FindDistance(Node * node, int maxDepth, vector<int> & out);

  vector<Node> nodes_;
};

Tree::Tree(int n) :
    nodes_(n) {
  for (int i = 0; i < n; i++)
    nodes_[i].i_ = i;
}

void Tree::distance(vector<int> & out) {
  FixDepth(&nodes_[0]);
  FindDistance(&nodes_[0], 0, out);
}

void Tree::insert(int x, int y) {
  nodes_[min(x, y)].children_.push_back(&nodes_[max(x, y)]);
}

int Tree::MaxChildrenDepth(Node * node, int except) {
  int maxDepth = 0;
  for (int i = 0; i < node->children_.size(); i++)
    if (node->children_[i]->i_ != except
        && node->children_[i]->depth_ > maxDepth) {
      maxDepth = node->children_[i]->depth_;
    }
  return maxDepth;
}

void Tree::FixDepth(Node * node) {
  if (node == nullptr)
    return;

  for (int i = 0; i < node->children_.size(); i++)
    FixDepth(node->children_[i]);

  node->depth_ = MaxChildrenDepth(node, -1) + 1;
}

void Tree::FindDistance(Node * node, int maxDepth, vector<int> & out) {
  if (node == nullptr)
    return;

  for (int i = 0; i < node->children_.size(); i++)
    FindDistance(node->children_[i], max(MaxChildrenDepth(node, node->children_[i]->i_), maxDepth) + 1, out);

  out[node->i_] = max(MaxChildrenDepth(node, -1), maxDepth);
}

int main() {
  int n = 0;
  cin >> n;

  Tree tree(n);

  for (int i = 0; i < n - 1; ++i) {
    int x, y = 0;
    cin >> x >> y;
    tree.insert(x, y);
  }
  vector<int> out(n);
  tree.distance(out);
  for (int i = 0; i < n; i++)
    cout << out[i] << '\n';
}
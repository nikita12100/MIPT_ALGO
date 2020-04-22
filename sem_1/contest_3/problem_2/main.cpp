//Дано число N < 106 и последовательность пар целых чисел из [-231, 231] длиной N.
//Построить декартово дерево из N узлов, характеризующихся парами чисел (Xi, Yi).
//Каждая пара чисел (Xi, Yi) определяет ключ Xi и приоритет Yi в декартовом дереве.
//Добавление узла в декартово дерево выполняйте второй версией алгоритма, рассказанного на лекции:
//        При добавлении узла (x, y) выполняйте спуск по ключу до узла P с меньшим приоритетом.
//Затем разбейте найденное поддерево по ключу x так, чтобы в первом поддереве все ключи меньше x,
//а во втором больше или равны x. Получившиеся два дерева сделайте дочерними для нового узла (x, y).
//Новый узел вставьте на место узла P. Построить также наивное дерево поиска по ключам Xi.
//Равные ключи добавляйте в правое поддерево. Вычислить разницу глубин наивного дерева поиска
//и декартового дерева. Разница может быть отрицательна.
#include <iostream>
#include <queue>
#include <stack>

using namespace std;

/////////////////////////////////////////////////Декартово дерево
template <typename KeyT>
class Dec_Tree{
 public:
  Dec_Tree();
  ~Dec_Tree();

  void insert(KeyT key, int priority);
  void print();
  int depth();
 private:

  struct Node{
    KeyT key;
    int priority;
    Node* left;
    Node* right;
    Node(KeyT k, int pr){
      key = k;
      priority = pr;
      left = nullptr;
      right = nullptr;
    }
  };

  Node* root;

  void Split(Node* currentNode, KeyT key, Node*& left, Node*& right);
  void Deleting(Node*& node);
  void Print_tree(Node* root);
  void Insert(Node*& beg_node, Node*& node);
  int Depth_Tree(Node* node);
};
template <typename KeyT>
Dec_Tree<KeyT>::Dec_Tree():
    root( nullptr )
{}
template <typename KeyT>
Dec_Tree<KeyT>::~Dec_Tree(){
  Deleting(root);
}
template <typename KeyT>
void Dec_Tree<KeyT>::Split(Node* currentNode, KeyT key, Node*& Left, Node*& Right){
  if( currentNode == nullptr ) {
    Left = nullptr;
    Right = nullptr;
  } else if( currentNode->key <= key ) {
    Split( currentNode->right, key, currentNode->right, Right );
    Left = currentNode;
  } else {
    Split( currentNode->left, key, Left, currentNode->left );
    Right = currentNode;
  }
}
template <typename KeyT>
void Dec_Tree<KeyT>::insert(KeyT key, int priority){
  Node* node = new Node(key, priority);
  if(root == nullptr){
    root = node;
    return;
  }
  else Insert(root, node);
}
template <typename KeyT>
void Dec_Tree<KeyT>::Insert(Node*& beg_node, Node*& node) {
  if(beg_node == nullptr){ // если лист
    beg_node = node;
    return;
  }
  if(!(beg_node->priority < node->priority)) { //меньше приоритет
    if(node->key < beg_node->key){          // идем влево
      Insert(beg_node->left, node);
    }
    else Insert(beg_node->right, node);   // идем вправо
    return;
  }

  Split(beg_node, node->key, node->left, node->right);
  beg_node = node;
}
template <typename KeyT>
void Dec_Tree<KeyT>::Deleting(Node*& node){
  if (node->left != nullptr) {
    Deleting(node->left);
  }
  if (node->right != nullptr) {
    Deleting(node->right);
  }
  delete (node);
}
template <typename KeyT>
void Dec_Tree<KeyT>::print(){
  print_tree(root);
}
template <typename KeyT>
void Dec_Tree<KeyT>::Print_tree(Node* root){
  queue<Node*> q;
  q.push(root);
  while( !q.empty() ){
    Node* node = q.front();
    q.pop();
    cout << node->key << " ";
    if( node->left != nullptr )
      q.push(node->left);

    if( node->right != nullptr )
      q.push(node->right);
  }

}
template <typename KeyT>
int Dec_Tree<KeyT>::Depth_Tree(Node* node){
  int h1 = 0;
  int h2 = 0;
  if(node == nullptr)
    return 0;
  if(node->left){
    h1 = Depth_Tree(node->left);
  }
  if(node->right){
    h2 = Depth_Tree(node->right);
  }
  return (max(h1, h2)+1);
}
template <typename KeyT>
int Dec_Tree<KeyT>::depth(){
  return Depth_Tree(root);
}
///////////////////////////////////////////////////Наивное дерево
template <typename KeyT>
class Naive_Tree{
 public:
  Naive_Tree();
  ~Naive_Tree();
  void insert(KeyT k);
  void print();
  int depth();
 private:
  struct Node{
    KeyT value;
    bool was_here;
    Node* left = nullptr;
    Node* right = nullptr;
    Node(KeyT k){
      value = k;
      was_here = 0;
      left = nullptr;
      right = nullptr;
    }
  };

  Node* root;

  void Insert_Tree(Node*& node, KeyT k);
  void Print_tree(Node* node);
  void Deleting(Node*& node);
  int Depth_Tree(Node* node);
};

template <typename KeyT>
Naive_Tree<KeyT>::Naive_Tree():
    root(nullptr)
{}
template <typename KeyT>
Naive_Tree<KeyT>::~Naive_Tree(){
  Deleting(root);
}
template <typename KeyT>
void Naive_Tree<KeyT>::insert(KeyT k){
  if(root == nullptr) root = new Node(k);
  else Insert_Tree(root, k);
}
template <typename KeyT>
void Naive_Tree<KeyT>::Insert_Tree(Node*& node, KeyT k){ // наивное дереово
  Node* n = root;
  while( 1 ){
    if( k < n->value && n->left != nullptr )
      n = n->left;
    else if( k > n->value &&  n->right != nullptr )
      n= n->right;
    else if( k < n->value && n->left == nullptr ) {
      n->left = new Node(k);
      return;
    }
    else if( k > n->value &&  n->right == nullptr ){
      n->right = new Node(k);
      return;
    }
  }
}
template <typename KeyT>
void Naive_Tree<KeyT>::print(){
  Print_tree(root);
}
template <typename KeyT>
void Naive_Tree<KeyT>::Deleting(Node*& node){
  queue<Node*> q;
  q.push(root);
  while( !q.empty() ){
    Node* node = q.front();
    q.pop();
    if( node->left != nullptr )
      q.push(node->left);

    if( node->right != nullptr )
      q.push(node->right);
    delete node;
  }
}
template <typename KeyT>
void Naive_Tree<KeyT>::Print_tree(Node* root){
  queue<Node*> q;
  q.push(root);
  while( !q.empty() ){
    Node* node = q.front();
    q.pop();
    cout << node->value << " ";
    if( node->left != nullptr )
      q.push(node->left);

    if( node->right != nullptr )
      q.push(node->right);
  }
}
template <typename KeyT>
int Naive_Tree<KeyT>::Depth_Tree(Node* node){
  int depth = 1;
  int maxdepth = 1;
  stack<Node*> q;
  q.push(root);
  while( !q.empty() ){
    Node* node = q.top();

    if( node->left && node->left->was_here == 0) {
      q.push(node->left);
      ++depth;
    }
    else if( node->right && node->right->was_here == 0) {
      q.push(node->right);
      ++depth;
    }
    else{
      if(node == root) break;
      node->was_here = 1;
      q.pop();
      --depth;
    }
    if(depth > maxdepth) maxdepth = depth;
  }
  return maxdepth;
}
template <typename KeyT>
int Naive_Tree<KeyT>::depth(){
  return Depth_Tree(root);
}

int main() {
  int n = 0;
  cin >> n;
  int key, priority = 0;

  Dec_Tree<int> dec_tree;
  Naive_Tree<int> naive_tree;

  for (int i = 0; i < n; ++i) {
    cin >> key >> priority;
    dec_tree.insert(key, priority);
    naive_tree.insert(key);
  }

  cout << naive_tree.depth() - dec_tree.depth();

  return 0;
}
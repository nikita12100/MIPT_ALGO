/*
 Дано число N < 106 и последовательность целых чисел из [-231..231] длиной N.
Требуется построить бинарное дерево, заданное наивным порядком вставки.
 Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K,
 то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
Выведите элементы в порядке level-order (по слоям, “в ширину”).
 */

#include <iostream>
#include <queue>

using namespace std;

template <class T>
class Naive_Tree{
public:
    Naive_Tree();
    ~Naive_Tree();
    void insert(T x);
    void print();

private:
    struct Node{
        T value;
        Node* left = nullptr;
        Node* right = nullptr;
        Node(T x){
            value = x;
            left = nullptr;
            right = nullptr;
        }
    };

    Node* root;

    void Insert_Tree(Node*& node, T x);
    void Print_tree(Node *node);
    void Deleting(Node *&node);
};
template <class T>
Naive_Tree<T>::Naive_Tree():
    root(nullptr)
{}
template <class T>
Naive_Tree<T>::~Naive_Tree(){
    Deleting(root);
}
template <class T>
void Naive_Tree<T>::insert(T x){
    Insert_Tree(root, x);
}
template <class T>
void Naive_Tree<T>::Insert_Tree(Node*& node, T x){ // наивное дереово
//
//    if( node == nullptr ) { // случай для корня
//        node = new Node(x);
//        return;
//    }
//
//    if( node->value > x )
//        Insert_Tree( node->left, x );
//    else
//        Insert_Tree( node->right, x );

  queue<Node*> q;
  q.push(root);
  while( !q.empty() ){
    Node* node = q.front();
    q.pop();
    if( k < node->value && node->left != nullptr )
      q.push(node->left);
    else if( k > node->value &&  node->right != nullptr )
      q.push(node->right);
    else if( k < node->value && node->left == nullptr ) {
      node->left = new Node(k);
      return;
    }
    else if( k > node->value &&  node->right == nullptr ){
      node->right = new Node(k);
      return;
    }
  }

}
template <class T>
void Naive_Tree<T>::print(){
    print_tree(root);
}
template <class T>
void Naive_Tree<T>::Deleting(Node*& node)
{
    if (node->left != nullptr) {
        deleting(node->left);
    }
    if (node->right != nullptr) {
        deleting(node->right);
    }
    delete (node);
};
template <class T>
void Naive_Tree<T>::Print_tree(Node* root){
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

int main() {
    int n = 0;
    cin >> n;

    Naive_Tree<int> tree;
    int buff = 0;
    for (int i = 0; i < n; ++i) {
        cin >> buff;
        tree.insert(buff);
    }

    tree.print();
    return 0;
}
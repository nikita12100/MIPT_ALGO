//3_1. Солдаты. В одной военной части решили построить в одну шеренгу по росту.
//Т.к. часть была далеко не образцовая, то солдаты часто приходили не вовремя,
//        а то их и вовсе приходилось выгонять из шеренги за плохо начищенные сапоги.
//Однако солдаты в процессе прихода и ухода должны были всегда быть выстроены
//по росту – сначала самые высокие, а в конце – самые низкие. За расстановку
//солдат отвечал прапорщик, который заметил интересную особенность – все солдаты
//в части разного роста. Ваша задача состоит в том, чтобы помочь прапорщику правильно
//расставлять солдат, а именно для каждого приходящего солдата указывать, перед каким
//        солдатом в строе он должен становится. Требуемая скорость
//выполнения команды - O(log n).
//Формат входных данных.
//Первая строка содержит число N – количество команд (1 ≤ N ≤ 30 000). В каждой
//следующей строке содержится описание команды: число 1 и X если солдат приходит
//        в строй (X – рост солдата, натуральное число до 100 000 включительно)
//и число 2 и Y если солдата, стоящим в строе на месте Y надо удалить из строя.
//Солдаты в строе нумеруются с нуля.
//Формат выходных данных.
//На каждую команду 1 (добавление в строй) вы должны выводить число K – номер позиции,
//        на которую должен встать этот солдат (все стоящие за ним двигаются назад).

#include <iostream>

using namespace std;

struct Node{
  int key;
  unsigned char height;
  unsigned char l_height;
  unsigned char r_height;
  Node* left;
  Node* right;
  Node(int k){
    key = k;
    height = 1;
    l_height = 0;
    r_height = 0;
    left = nullptr;
    right = nullptr;
  }
};

class AVL_Tree{
 public:
  AVL_Tree();
  ~AVL_Tree();
  void insert(int key);
  void remove(int position);
  int place_sold(int key); //место для вставляемого

 private:
  Node* root;

  unsigned char Height(Node* node); //высота в узле
  int Node_balance(Node* & node);   //баланс узла      --
  void Correct_height(Node* & node); //кореектная высота --
  Node* R_roation(Node* node);  // малый правый поворот
  Node* L_roation(Node* node); // малый левый поворот
  Node* Balance(Node* & node);
  Node* Insert(Node* & node, int k);
  Node* Find_min(Node* & node);
  Node* Remove_min(Node* & node);  //ищем сначала справа
  Node* Remove(Node* & node, int k);
  void Deleting(Node* & node);
  void Find_pos(Node* root, const int key, int & i, int & res);   // макс -> мин : находит позицию по ключу
  void Find_k(Node* node,int & i, const int pos, int & key);            // находит ключ по позиции i-счетчик
};

AVL_Tree::AVL_Tree():
    root(nullptr)
{}
AVL_Tree::~AVL_Tree(){
  Deleting(root);
}
unsigned char AVL_Tree::Height(Node* node){
  return node ? node->height : 0;
}
int AVL_Tree::Node_balance(Node* & node){
  if(!node) return 0;
  else return node->l_height - node->r_height;
}
void AVL_Tree::Correct_height(Node* & node){
  node->l_height = Height(node->left);
  node->r_height = Height(node->right);
  node->height = (node->l_height > node->r_height ? node->l_height : node->r_height) + 1;
}
Node* AVL_Tree::R_roation(Node* node){
  Node* l = node->left;
  node->left = l->right;
  Correct_height(l->right);
  l->right = node;
  Correct_height(node);
  Correct_height(l);
  return l;
}
Node* AVL_Tree::L_roation(Node* node){
  Node* r = node->right;
  node->right = r->left;
  Correct_height(r->left);
  r->left = node;
  Correct_height(node);
  Correct_height(r);
  return r;
}
Node* AVL_Tree::Balance(Node* & node){
  Correct_height(node);
  if(Node_balance(node) == 2){
    if(node->right == nullptr){ /////////////////
      if(node->left->right) {
        Node* q = node->left->right;
        node->left->right = nullptr;
        Correct_height(node->left);
        q->left = node->left;
        node->left = nullptr;
        q->right = node;
        Correct_height(node);
        node = q;
        Correct_height(node);
        return node;
      }
      else{
        Node* q = node->left;
        node->left = nullptr;
        Correct_height(node);
        q->right = node;
        Correct_height(node);
        node = q;
        Correct_height(node);
        return node;
      }
    }
    if(Node_balance(node->right) < 0)
      node->right = L_roation(node->right);
    node = R_roation(node);
    return node;
  }
  if(Node_balance(node) == -2) {
    if(node->left == nullptr){
      if(node->right->right){
        Node* q = node->right->left;
        node->right->left = nullptr;
        Correct_height(node->right);
        q->right = node->right;
        node->right = nullptr;
        q->left = node;
        Correct_height(node);
        node = q;
        Correct_height(node);
        return node;
      }
      else{
        Node* q = node->right->left;
        node->right->left = nullptr;
        Correct_height(node->right);
        Correct_height(node);
        q->right = node->right;
        Correct_height(node);
        q->left = node;
        Correct_height(node);
        node = q;
        Correct_height(node);
        return node;
      }
    }
    if(Node_balance(node->left) > 0)
      node->left = R_roation(node->left);
    node = L_roation(node);
    return node;
  }
  return node;
}
void AVL_Tree::insert(int key){
  Insert(root, key);
}
Node* AVL_Tree::Insert(Node* & node, int k){
  if(!node) {
    node = new Node(k);
    return node;
  }
  else if(k < node->key)
    node->left = Insert(node->left, k);
  else
    node->right = Insert(node->right, k);
  return Balance(node);
}
Node* AVL_Tree::Find_min(Node* & node){
  return node->left ? Find_min(node->left) : node;
}
Node* AVL_Tree::Remove_min(Node* & node){
  if(node->left == nullptr){
    node = node->right;
    return node;
  }
  node->left = Remove_min(node->left);
  return Balance(node);
}
void AVL_Tree::remove(int position){
  int i = 0;
  int k = 0;
  Find_k(root, i, position,k);
  Remove(root, k);
}
Node* AVL_Tree::Remove(Node* & node, int k){
  if(!node) return 0;
  else if(k < node->key){
    node->left = Remove(node->left, k);
  }
  else if(k > node->key){
    node->right = Remove(node->right, k);
  }
  else if(k == node->key){
    Node* l = node->left;
    Node* r = node->right;
    delete node;
    if(!r) {
      node = l;
      return node;
    }
    Node* min = Find_min(r);
    min->right = Remove_min(r);
    min->left = l;
    node = Balance(min);
    return node;
  }
  Balance(node);
  return node;
}
void AVL_Tree::Deleting(Node* & node){
  if (node->left) Deleting(node->left);
  if (node->right) Deleting(node->right);
  delete node;
}
int AVL_Tree::place_sold(int key){
  int i = 0;
  int result = 0;
  Find_pos(root, key, i, result);
  return result;
}

void AVL_Tree::Find_pos(Node* node, const int k, int & i, int & res) {
  if(node->key == k){
    res = node->r_height + i; //  + высота правого поддерева
    return;
  }
  else if(node->key < k && node->right){     // идем вправо
    Find_pos(node->right, k, i, res);
  }
  else if(node->left){
    i += node->r_height;
    Find_pos(node->left, k, ++i, res);
  }
}

void AVL_Tree::Find_k(Node* node, int & i, const int position, int & key) {
  if(position == (node->r_height + i)){
    key = node->key;
    return;
  }
  else if(position > node->l_height && node->left){ // идем влево
    i += node->r_height;
    Find_k(node->left, ++i, position, key);
  }
  else if(node->right){
    Find_k(node->right, i, position, key);
  }
}

int main() {
  int n = 0;
  cin >> n;
  AVL_Tree tree;
  int key = 0;
  int comm = 0;
  for (int i = 0; i < n; ++i) {
    cin >> comm;
    cin >> key;
    if (comm == 1) {
      tree.insert(key);
      cout << '\n' << tree.place_sold(key);
    }
    else if(comm == 2) {
      tree.remove(key); // key -> position
    }
  }

  return 0;
}
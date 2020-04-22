#include "Huffman.h"
#include <fstream>
#include <map>
#include <iostream>
#include <list>
#include <string>
#include <bits/stdc++.h>


using namespace std;

typedef char byte;
typedef bool bit;

/*
 Струтура архива:
     * 1 бит хранит кол-во неисп. битов в конце потока
     * 1 бит хранит количество байт в коде
     * 1 бит хранит размер "алфавита"
    */

///////////////работа склассами ввода/вывода///////////////////

class Node{
 public:
  Node();
  Node(byte value, int weight, Node* left, Node* right);

  byte value;
  int weight;

  Node* left;
  Node* right;
};

class bitostream{
 public:
  bitostream(IOutputStream & stream); //выдает входящий поток
  bool    write(bit value);    // true - когда бит записан

 private:
  IOutputStream &  stream_; // хранит поток
  byte        currentByte_; // хранит записываемый байт
  char        bitsWritten_; // хранит колчество битов в текущем байте
};

class bitistream{
 public:
  bitistream(IInputStream & stream);   //получает выходной поток
  bool read(bit & value);  //читает бит, flase - если поток закончился

 private:
  IInputStream &  stream_; //хранит поток
  byte    currentByte_;  //хранит текищий байт для чтения
  char    bitsRead_; //хранит кол-во прочтенных битов
};

bitistream::bitistream(IInputStream & stream):  //конструктор
    stream_(stream),
    bitsRead_(0)
{}

bool bitistream::read(bit & value){        // чтение
  if(bitsRead_ == 0){
    byte buff = 0;
    if (!stream_.Read(buff))
      return false;
    else {
      currentByte_ = 0;
      for (int i = 0; i < 8; i++){
        currentByte_ <<= 1;
        currentByte_ |= 1 & buff;
        buff >>= 1;
      }
    }
  }
  value = currentByte_ & 1;
  currentByte_ >>= 1;
  bitsRead_ = (bitsRead_ + 1) % 8;
  return true;
}

bitostream::bitostream(IOutputStream& stream):      // конструктор
    stream_(stream),
    bitsWritten_(0)
{}

bool bitostream::write(bit value){       // запись
  currentByte_ <<= 1;
  currentByte_ |= (value) ? 1:0;
  ++bitsWritten_;
  if(bitsWritten_ == 8){
    stream_.Write(currentByte_);
    bitsWritten_ = 0;
    return true;
  }
  return false;
}

//////////////////////////////////////////////////////

static void copyStream(IInputStream& input, IOutputStream& output){
  byte value;
  while (input.Read(value))
  {
    output.Write(value);
  }
}

Node::Node():
    value(0),
    weight(0),
    left(nullptr),
    right(nullptr)
{}

Node::Node(byte _value, int _weight, Node* _left, Node* _right):
    value(_value),
    weight(_weight),
    left(_left),
    right(_right)
{}

bool nodeCmp(Node* & left, Node* right){    // l<r
  return left->weight < right->weight;
}

Node* Build_Tree(map<byte, int > freq){
  list<Node*> list;
  for(auto i:freq){
    if(i.second != 0){
      Node* node = new Node(i.first , i.second, nullptr, nullptr)  ;
      list.push_back(node);
    }
  }
  while(list.size() > 1){
    list.sort(nodeCmp);
    Node* left = list.front();
    list.pop_front();
    Node* right = list.front();
    list.pop_front();
    Node* father = new Node(0 , left->weight + right->weight, left, right);
    list.push_back(father);
  }
  Node* tree = list.front();
  return tree;
}

void deleteTree(Node* tree){
  if(tree->left) deleteTree(tree->left);
  if(tree->right) deleteTree(tree->right);
  delete tree;
}

void getCodes(Node* node, map<byte,int> & codes, int code){    // ***********************
  if(node->left == nullptr){
    int result = 1;
    while(code != 1){
      result <<= 1;
      result |= code & 1;
      code >>= 1;
    }
    codes[node->value] = result;
    return;
  }

  getCodes(node->left, codes, code << 1);
  getCodes(node->right, codes, (code << 1) + 1);
}

void countCodeLenghts(const map<byte,int> & codes,
                      map<byte,char> & codeLengths){
  for (auto i:codes) {
    codeLengths[i.first] = 0;
    if(i.second != 0){
      int code = i.second;
      while (code != 1){
        ++codeLengths[i.first];
        code >>= 1;
      }
    }
  }
}

void fixTree(Node* node){    // убираем левые листья
  if(node == nullptr)
    return;

  if(node->left != nullptr && node->right == nullptr){
    deleteTree(node->left);
    node->left = nullptr;
    return;
  }

  fixTree(node->left);
  fixTree(node->right);
}

void Encode(IInputStream& original, IOutputStream& compressed){
  map<byte, int> freq; // карта количества букв

  string file;
  byte value = 0;
  while(original.Read(value)){
    ++freq[value];
    file += value;
  }
  --freq[value];  // последниц прочтенный бит
  file.erase(file.length() - 1);


  Node* tree = Build_Tree(freq); //  готовое дерево Хаффмана

  map<byte,int> codes;      //таблица кодов для букв
  getCodes(tree, codes, 1);
  deleteTree(tree);

  map<byte,char> codeLengths;          //посчет длинн кадого кода
  countCodeLenghts(codes, codeLengths);

  char maxlenght = 0;
  for(auto i:codeLengths)
    if(i.second > maxlenght) maxlenght = i.second;    //посчет макс. длинны

  size_t encodedSize = 0;         // подсчет кол-ва неиспользованных бит
  byte alphabetSize = 0;         // размер алфавита
  for(auto i:freq) {
    if (i.second != 0)
      alphabetSize++;
    encodedSize = (encodedSize + i.second * codeLengths[i.first]) % 8;
  }
  byte restBits = 8 - encodedSize;     // неныжный остаток

  //подсчитываем длинныу кода , с учетом хранение длинны(в потоке)
  byte bytesPerCode = (maxlenght % 8 == 0) ? (maxlenght / 8):(maxlenght / 8 + 1);

  bitostream bout(compressed);   //записываем в поток на вывод

  //запись

  compressed.Write(restBits);         // до куда считывать декодеру(остаток)
  compressed.Write(bytesPerCode);     // бит на код
  compressed.Write(alphabetSize);     // размер "алфавита"
  for(auto i:codes)
    if(freq[i.first] != 0){
      int code = i.second;
      int j = 0;
      compressed.Write(byte(i.first));
      while(code != 1){
        bout.write(code & 1);
        code >>= 1;
        ++j;
      }
      for(; j < bytesPerCode * 8; ++j)
        bout.write(0);
    }
  for(size_t i = 0; i < file.length(); ++i){
    int code = codes[file[i]];
    while(code != 1){
      bout.write(code & 1);
      code >>= 1;
    }
  }
  for(int i = 0; i < restBits; ++i)  // остаток заполняем нулями
    bout.write(0);
}

void Decode(IInputStream & compressed, IOutputStream & decoded){
  // сначала считываем нужную нам инфрмацию
  byte restBits;
  byte bytesPerCode;
  byte buff;

  compressed.Read(restBits);       // до куда считывать декодеру(остаток)
  compressed.Read(bytesPerCode);   // бит на код
  compressed.Read(buff);           // размер "алфавита"
  unsigned alphabetSize = (unsigned char) buff;
  if(alphabetSize == 0)
    alphabetSize = 256;

  Node* tree = new Node(0, 0, nullptr, nullptr);     // строим дерео Хаффмана
  bitistream bin(compressed);
  for(int i = 0; i < alphabetSize; ++i){        // весь "алфивит" записываем в дерево
    byte symbol;
    Node* node = tree;
    compressed.Read(symbol);
    for(int j = 0; j < 8 * bytesPerCode; ++j){
      bit bit1;
      bin.read(bit1);
      if(bit1 == 0){
        if(node->left == nullptr) //запиываем влево
          node->left = new Node(symbol, 0, nullptr, nullptr);
        node = node->left;
      }
      else{
        if(node->right == nullptr)   // записывам вправо
          node->right = new Node(symbol, 0, nullptr, nullptr);
        node = node->right;
      }
    }
  }
  fixTree(tree);  // подчищаем концы

  // чтиние потока (имея дерево)
  string file;
  byte symbol;
  while(compressed.Read(symbol)){
    file += symbol;
  }
  file.erase(file.length() - 1);

  // запись закодированного файла
  Node* node = tree;              //обход по дереву Хаффмана
  for(int i = 0; i < file.length() - 1; ++i){
    for(int j = 7; j >= 0; --j){
      if(((file[i] >> j) & 1) == 0)
        node = node->left;
      else
        node = node->right;
      if(node->left == nullptr){
        decoded.Write(node->value);
        node = tree;
      }
    }
  }
  for(int j = 7; j >= restBits; --j){
    if(((file[file.length() - 1] >> j) & 1) == 0)
      node = node->left;
    else
      node = node->right;
    if(node->left == nullptr){
      decoded.Write(node->value);
      node = tree;
    }
  }

  deleteTree(tree);
}

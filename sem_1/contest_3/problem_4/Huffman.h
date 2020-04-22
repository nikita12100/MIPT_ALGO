#ifndef NNN4_HUFFMAN_H
#define NNN4_HUFFMAN_H

#include <fstream>
#include <map>
#include <iostream>
#include <list>
#include <string>
#include <bits/stdc++.h>


using namespace std;

typedef char byte;
typedef bool bit;

class IInputStream {
 public:
  IInputStream(ifstream & fin):
      fin_(fin)
  {}

  bool Read(byte& value){   // Возвращает false, если поток закончился
    if(fin_.eof())
      return false;
    fin_.get(value);
    return true;
  }

 private:
  ifstream & fin_;
};

class IOutputStream {
 public:
  IOutputStream(ofstream & fout):
      fout_(fout)
  {}
  void Write(byte value){
    fout_ << value;
  }

 private:
  ofstream & fout_;
};



void    Encode(IInputStream & original, IOutputStream & compressed);  //кодирование потока

void    Decode(IInputStream & compressed, IOutputStream & decoded); // декодирование потока

#endif //NNN4_HUFFMAN_H
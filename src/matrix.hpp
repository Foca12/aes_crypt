#include "../include/chunk.hpp"
#include "../include/types.hpp"
#include "../include/constants.hpp"
#include "../include/matrix.hpp"
#include <string>
#include <memory.h>
#include <vector>
#include <cmath>

class ByteMatrix{
  types::bclist chunks; // vettore di chunks
  
  public:
  ByteMatrix(int x=0){
    this->chunks.insert(this->chunks.end(), x, ByteChunk128());
  }
  ByteMatrix(types::bclist list){
    this->chunks = list;
  }

  // costruttori alternativi che dividono il testo
  static ByteMatrix divide_string(std::string bytes){
    return ByteMatrix::divide_bytearray(Bytearray(bytes));
  }
  static ByteMatrix divide_bytearray(Bytearray bytes){
    types::bclist chunks;
    int i = 0;
    for (; i < bytes.length()-16; i += num_chars){
      chunks.push_back(ByteChunk128(bytes.slice(i, i+num_chars))); // riempe vettore chunks con i chunks
    }
    chunks.push_back(ByteChunk128(bytes.slice(i, bytes.length()))); // aggiunge chunk con padding
    return ByteMatrix(chunks);
  }

  // metodi vettore
  int length(){
    return this->chunks.size();
  }
  int size(){
    return this->length();
  }
  int& operator[](int idx){
    int x = floor(idx / num_chars);
    int y = idx % num_chars;
    return chunks[x][y];
  }
  ByteChunk128& get_chunk(int idx){
    return this->chunks[idx];
  }
  
  void push_back(ByteChunk128 bytes){
    this->chunks.push_back(bytes);
  }

  // cast di tipi
  operator std::string() {
    std::string str = "";
    for (auto i : this->chunks){
      str += (std::string)i;
    }
    return str;
  }
};
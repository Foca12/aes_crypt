#pragma once

#include "./chunk.hpp"
#include "./constants.hpp"
#include "./types.hpp"
#include <string>
#include <memory.h>
#include <vector>
#include <cmath>

typedef std::vector<ByteChunk128> bclist;
typedef bclist::iterator bclist_iterator;

class ByteMatrix{
  bclist chunks; // vettore di chunks
  
  public:
  ByteMatrix(int x=0){
    this->chunks.insert(this->chunks.end(), x, ByteChunk128());
  }
  ByteMatrix(bclist list){
    this->chunks = list;
  }

  // costruttori alternativi che dividono il testo
  static ByteMatrix divide_string(std::string bytes){
    return ByteMatrix::divide_bytearray(Bytearray(bytes));
  }
  static ByteMatrix divide_bytearray(Bytearray bytes){
    bclist chunks;
    int i = 0;
    for (; i < bytes.length()-16; i += chars_per_chunk){
      chunks.push_back(ByteChunk128(bytes.slice(i, i+chars_per_chunk))); // riempe vettore chunks con i chunks
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
    int chunk_idx = floor(abs(idx) / chars_per_chunk);
    int num_idx = abs(idx) % chars_per_chunk;
    if (idx < 0){
      chunk_idx = this->length() - chunk_idx - 1;
      num_idx = chars_per_chunk - num_idx;
    }
    return this->chunks[chunk_idx][num_idx];
  }
  int& at(int chunk_idx, int num_idx){
    return this->chunks[chunk_idx >= 0? chunk_idx : this->length()+chunk_idx][num_idx >= 0? num_idx : chars_per_chunk+num_idx];
  }
  ByteChunk128& get_chunk(int idx){
    return this->chunks[idx];
  }
  bclist_iterator begin(){
    return this->chunks.begin();
  }
  bclist_iterator end(){
    return this->chunks.end();
  }
  ByteMatrix operator<<(int rounds){
    ByteMatrix result (this->chunks);
    for (int i = 0; i < this->length(); i++){
        result.get_chunk(i) <<= rounds;
    }
    return result;
  }
  ByteMatrix operator>>(int rounds){
    ByteMatrix result (this->chunks);
    for (int i = 0; i < this->length(); i++){
        result.get_chunk(i) >>= rounds;
    }
    return result;
  }
  ByteMatrix shift_left_crypt(){
    ByteMatrix result (this->chunks);
    for (int i = 0; i < this->length(); i++){
        result.get_chunk(i) = result.get_chunk(i).shift_left_crypt();
    }
    return result;
  }
  ByteMatrix shift_right_crypt(){
    ByteMatrix result (this->chunks);
    for (int i = 0; i < this->length(); i++){
        result.get_chunk(i) = result.get_chunk(i).shift_right_crypt();
    }
    return result;
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
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
  
  void push_back(int x){
    (this->get_chunk(-1)).push_back(x);
  }
  void extend(Bytearray x){
    int padding = this->get_padding();
    if (padding == 0){
      ByteChunk128 empty;
      this->chunks.insert(this->chunks.end(), empty.begin(), empty.end());
      this->extend(x);
    }
    bool end = false;
    if (padding > x.length()){
      padding = x.length();
      end = true;
    }
    this->get_chunk(-1).extend(x.slice(padding));
    if (!end){
      this->extend(x.slice(padding, x.length()));
    }
  }

  ByteMatrix slice(int start, int stop, int step){
    ByteMatrix result;
    for (int i = start; i < stop; i += step){
      result.push_back(this->operator[](i));
    }
    return result;
  }
  ByteMatrix slice(int start, int stop){
    return this->slice(start, stop, 1);
  }
  ByteMatrix slice(int stop){
    return this->slice(0, stop, 1);
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
    return this->chunks[idx >= 0? idx : this->length() + idx];
  }
  int get_padding(){
    return chars_per_chunk - (this->get_chunk(-1).length());
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
  operator types::ilist(){
    types::ilist list = {};
    for (auto i : this->chunks){
      list.insert(list.end(), ((types::ilist)i).begin(), ((types::ilist)i).end());
    }
    return list;
  }
  operator Bytearray(){
    Bytearray array;
    for (auto i : this->chunks){
      array.extend((Bytearray) i);
    }
    return array;
  }

  // conversioni di formato
  string hex(){
    std::stringstream ss;
    
    for (auto i : this->chunks) {
      ss << std::hex << std::setw(2) << std::setfill('0') << i.hex();
    }

    return ss.str();
  }
  string oct(){
    std::stringstream ss;
    
    for (auto i : this->chunks) {
      ss << std::oct << std::setw(3) << std::setfill('0') << i.oct();
    }

    return ss.str();
  }

  // costruttori alternativi
  static ByteMatrix from_hex(string str){
    Bytearray array = Bytearray::from_hex(str);
    return ByteMatrix::divide_bytearray(array);
  }
  static ByteMatrix from_oct(string str){
    Bytearray array = Bytearray::from_oct(str);
    return ByteMatrix::divide_bytearray(array);
  }
};
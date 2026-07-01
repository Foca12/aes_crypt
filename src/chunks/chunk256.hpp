#pragma once

#include "../types.hpp"
#include "../bytearray.hpp"
#include "./chunk128.hpp"
#include <iostream>


class ByteChunk256 : private ByteChunk128{
  std::array<ByteChunk128, 2> chunks;

  void basic_constructor(Bytearray bytes){
    if (bytes.length() > chars_per_chunk256){
      throw std::invalid_argument("Input array dimension is bigger than "+std::to_string(chars_per_chunk256)+", got "+std::to_string(bytes.length()));
    }
    if (bytes.length() < chars_per_chunk256){
      throw std::invalid_argument("Input array dimension is smaller than "+std::to_string(chars_per_chunk256)+", got "+std::to_string(bytes.length()));
    }
    int acc = 0;
    for (int i = 0; i < chars_per_chunk256; i += chars_per_chunk128){
      this->chunks[acc].basic_constructor(bytes.slice(i, i+chars_per_chunk128)); // setta array
      acc++;
    }
  }

  public:
  
  ByteChunk256(int x=0){
    for (int i = 0; i < 2; i++){
      this->chunks[i].fill(x);
    }
  }
  ByteChunk256(std::string bytes){
    this->basic_constructor(Bytearray(bytes));
  }
  ByteChunk256(Bytearray bytes){
    this->basic_constructor(bytes);
  };
  ByteChunk256(types::iarr<chars_per_chunk256> bytes){
    this->basic_constructor(bytes);
  };
  ByteChunk256(types::ilist bytes){
    this->basic_constructor(bytes);
  };
  ByteChunk256(int in_bytes[], int size){
    this->basic_constructor(Bytearray(in_bytes, size));
  }
  
  types::balist get_rows(){
    types::balist rows;
    // per ogni start di riga (0, 1, 2, 3...)
    for (int i = 0; i < chunk256_rows; i++){
      Bytearray row(0);
      // scorri del chunk256_side per prendere il valore della riga (2, 6, 10, 14...)
      for (ByteChunk128 j : this->chunks){
        row.extend(j.get_row(i));
      }
      rows.push_back(row);
    }
    return rows;
  }
  Bytearray get_row(int idx){
    return this->get_rows()[idx < 0? chunk256_rows + idx : idx];
  }
  void set_row(int row_idx, types::ilist value){
    this->set_row(row_idx, Bytearray(value));
  }
  void set_row(int row_idx, Bytearray value){
    int count = 0;
    if (row_idx < 0){
      row_idx = chunk256_rows + row_idx;
    }
    for (int i = row_idx; i < chars_per_chunk256; i += chunk256_rows){
      this->operator[](i) = value[count];
      count++;
    }
  }
  
  types::balist get_columns(){
    // unisci le colonne dei due chunks
    types::balist columns;
    for (ByteChunk128 i : this->chunks){
      types::balist vct = i.get_columns();
      columns.insert(columns.end(), vct.begin(), vct.end());
    }
    return columns;
  }
  Bytearray get_column(int idx){
    return this->get_columns()[idx < 0? chunk256_columns + idx : idx];
  }
  void set_column(int column_idx, types::ilist value){
    this->set_column(column_idx, Bytearray(value));
  }
  void set_column(int column_idx, Bytearray value){
    int count = 0;
    if (column_idx < 0){
      column_idx = chunk256_columns + column_idx;
    }
    for (int i = column_idx*chunk256_rows; count < chunk256_rows; i++){
      this->operator[](i) = value[count];
      count++;
    }
  }

  int get_first_null(){
    int empty_idx = -1;
    for (int i = 1; i >= 0; i--){
      ByteChunk128 current_chunk = this->chunks[i];
      int first_null = current_chunk.get_first_null();

      // se è riempito a metà ritorna first null con il bias del blocco
      if (first_null > 0){
        return first_null + (chars_per_chunk128 * i);
      }
      // se è vuoto salva il suo inizio
      else if (first_null == 0){
        empty_idx = chars_per_chunk128 * i;
      }
      else {
        break;
      }
    }
    // se un blocco è pieno e quello dopo è vuoto ritorna l'inzio salvato
    return empty_idx;
  }
  int length(){
    int idx = this->get_first_null();
    return idx == -1? chars_per_chunk256 : idx;
  }
  int size(){
    return this->length();
  }
  int padding(){
    return chars_per_chunk256-this->length();
  }

  void push_back(int x){
    for (int i = 0; i < 2; i++){
      if (this->chunks.operator[](i).padding() > 0){
        this->chunks.operator[](i).push_back(x);
        return;
      }
    }
    throw std::overflow_error("Trying to push back an element into an already full chunk 256");
  }
  void extend(Bytearray bytes){
    int free_idx = this->length(); // primo posto libero
    int free_chunk_idx = free_idx / chars_per_chunk128; // chunk che contiene il posto libero
    int mod_idx = free_idx % chars_per_chunk128; // indice del posto libero all'interno del chunk

    if (bytes.length() > chars_per_chunk256 - free_idx){
      throw std::invalid_argument("Input array exceed free chunk dimension");
    };

    // se bytes può essere contenuto in un solo chunk
    if (bytes.length() <= chars_per_chunk128-mod_idx){
      this->chunks[free_chunk_idx].extend(bytes);
    }
    
    else {
      // estendi il chunk con il bytearray fino a riempirlo
      this->chunks[free_chunk_idx].extend(bytes.slice(chars_per_chunk128-mod_idx));
      // fai ricorsione
      this->extend(bytes.slice(chars_per_chunk128-mod_idx, bytes.length()));
    }

  }

  types::iarr<chars_per_chunk256>::iterator begin(){
    return this->chunks[0].begin();
  }
  types::iarr<chars_per_chunk256>::iterator end(){
    return this->chunks[1].end();
  }

  int& operator[](int idx){
    // idx del chunk
    int chunk_idx = idx < 0? 1 + floor(idx / chars_per_chunk128) : floor(idx / chars_per_chunk128);
    // idx all'interno del chunk
    int num_idx = idx < 0? chars_per_chunk128 + (idx % chars_per_chunk128) : idx - (chars_per_chunk128 * chunk_idx);
    if (idx < 0 && idx % chars_per_chunk128 == 0){
      chunk_idx = 1 + (idx / chars_per_chunk128);
      num_idx = 0;
    }
    if (idx == -chars_per_chunk256){
      chunk_idx = 0;
      num_idx = 0;
    }
    return this->chunks[chunk_idx][num_idx];
  }

  Bytearray slice(int start, int stop, int step){
    Bytearray result;
    for (int i = start; i < stop; i += step){
      result.push_back(this->operator[](i));
    }
    return result;
  }
  Bytearray slice(int start, int stop){
    return this->slice(start, stop, 1);
  }
  Bytearray slice(int stop){
    return this->slice(0, stop, 1);
  }

  ByteChunk256& operator=(const ByteChunk256& x){
    for (int i = 0; i < 2; i++){
      this->chunks[i] = x.chunks[i];
    }
    return *this;
  }

  ByteChunk256 operator^(ByteChunk256 key){
    for (int i = 0; i < chars_per_chunk256; i++){
      key[i] = key[i] ^ this->operator[](i);
    }
    return key;
  }
  ByteChunk256 operator&(ByteChunk256 key){
    for (int i = 0; i < chars_per_chunk256; i++){
      key[i] = key[i] & this->operator[](i);
    }
    return key;
  }
  ByteChunk256 operator|(ByteChunk256 key){
    for (int i = 0; i < chars_per_chunk256; i++){
      key[i] = key[i] | this->operator[](i);
    }
    return key;
  }
  
  ByteChunk256 operator<< (int rounds) {
    if (rounds == 0){
      return *this;
    }
    types::balist rows = this->get_rows();
    ByteChunk256 result;
    for (auto row : rows){
      Bytearray shifted_row;
      for (auto col : row){
        shifted_row.push_back(col);
      }
      shifted_row <<= 1;
      result.extend(shifted_row);
    }
    return result << rounds-1;
  }
  ByteChunk256 operator>> (int rounds) {
    if (rounds == 0){
      return *this;
    }
    types::balist rows = this->get_rows();
    ByteChunk256 result;
    for (auto row : rows){
      Bytearray shifted_row;
      for (auto col : row){
        shifted_row.push_back(col);
      }
      shifted_row >>= 1;
      result.extend(shifted_row);
    }
    return result >> rounds-1;
  }
  ByteChunk128& operator<<= (int rounds){
    return this->operator=(this->operator<<(rounds));
  }
  ByteChunk128& operator>>= (int rounds){
    return this->operator=(this->operator>>(rounds));
  }

  ByteChunk256 shift_rows_left() {
    ByteChunk256 result;
    for (int row_idx = 0; row_idx < chunk256_rows; row_idx++){
      result.set_row(row_idx, this->get_row(row_idx) << row_idx);
    }
    return result;
  }
  ByteChunk256 shift_rows_right() {
    ByteChunk256 result;
    for (int row_idx = 0; row_idx < chunk256_rows; row_idx++){
      result.set_row(row_idx, this->get_row(row_idx) >> row_idx);
    }
    return result;
  }

  operator std::string(){
    std::string str = "";
    for (ByteChunk128 i : this->chunks){
      str += i;
    }
    return str;
  }
  operator Bytearray(){
    Bytearray str;
    for (ByteChunk128 i : this->chunks){
      str.extend((Bytearray)i);
    }
    return str;
  }
  operator types::ilist(){
    types::ilist str = {};
    for (ByteChunk128 i : this->chunks){
      str.insert(str.end(), i.begin(), i.end());
    }
    return str;
  }

  std::string hex(){
    std::string result = "";
    for (ByteChunk128 i : this->chunks){
      result += i.hex();
    }
    return result;
  }

  std::string oct(){
    std::string result = "";
    for (ByteChunk128 i : this->chunks){
      result += i.oct();
    }
    return result;
  }

  static ByteChunk256 from_hex(std::string str){
    return ByteChunk256(basic_from_hex(str));
  }
  static ByteChunk256 from_oct(std::string str){
    return ByteChunk256(basic_from_oct(str));
  }
};

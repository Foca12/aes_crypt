#pragma once

#include "bytearray.hpp"
#include "constants.hpp"
#include "types.hpp"

class ByteChunk128{
  void basic_constructor(Bytearray bytes){
    if (bytes.length() > chars_per_chunk){
      throw std::invalid_argument("Input array dimension is bigger than "+std::to_string(chars_per_chunk)+", got "+std::to_string(bytes.length()));
    }
    if (bytes.length() < 0){
      throw std::invalid_argument("Input array dimension must be a positive integer, got "+std::to_string(bytes.length()));
    }
    for (int i = 0; i < bytes.length(); i++){
      this->bytes[i] = bytes[i]; // setta array
    }
  }
  
  types::iarr<chars_per_chunk> bytes = {};

  public:
  
  ByteChunk128(int x=0){
    this->bytes.fill(x);
  }
  ByteChunk128(std::string bytes){
    this->basic_constructor(Bytearray(bytes));
  }
  ByteChunk128(Bytearray bytes){
    this->basic_constructor(bytes);
  };
  ByteChunk128(types::iarr<chars_per_chunk> bytes){
    types::ilist vct = {};
    vct.assign(bytes.begin(), bytes.end());
    this->basic_constructor(vct);
  };
  ByteChunk128(types::ilist bytes){
    this->basic_constructor(bytes);
  };
  ByteChunk128(int in_bytes[], int size){
    this->basic_constructor(Bytearray(in_bytes, size));
  }
  
  types::balist get_rows(){
    types::balist rows;
    // per ogni start di riga (0, 1, 2, 3)
    for (int chunk_row_idx = 0; chunk_row_idx < chunk_side; chunk_row_idx++){
      Bytearray row = {};
      // scorri del chunk_side per prendere il valore della riga (2, 6, 10, 14)
      for (int chunk_idx = chunk_row_idx; chunk_idx < chars_per_chunk; chunk_idx += chunk_side){
        row.push_back(bytes[chunk_idx]);
      }
      rows.push_back(row);
    }
    return rows;
  }
  Bytearray get_row(int idx){
    return this->get_rows()[idx < 0? chunk_side + idx : idx];
  }
  void set_row(int row_idx, types::ilist value){
    this->set_row(row_idx, Bytearray(value));
  }
  void set_row(int row_idx, Bytearray value){
    int count = 0;
    if (row_idx < 0){
      row_idx = chunk_side + row_idx;
    }
    for (int i = row_idx; i < chars_per_chunk; i += chunk_side){
      this->operator[](i) = value[count];
      count++;
    }
  }
  
  types::balist get_columns(){
    types::balist columns;
    // per ogni start di colonna (0, 4, 8, 12)
    for (int chunk_column_idx = 0; chunk_column_idx < chars_per_chunk; chunk_column_idx += chunk_side){
      Bytearray column = {};
      // scorri di 1 per prendere il valore della colonna (4, 5, 6, 7)
      for (int chunk_idx = chunk_column_idx; chunk_idx < chunk_side+chunk_column_idx; chunk_idx++){
        column.push_back(bytes[chunk_idx]);
      }
      columns.push_back(column);
    }
    return columns;
  }
  Bytearray get_column(int idx){
    return this->get_columns()[idx < 0? chunk_side + idx : idx];
  }
  void set_column(int column_idx, types::ilist value){
    this->set_column(column_idx, Bytearray(value));
  }
  void set_column(int column_idx, Bytearray value){
    int count = 0;
    if (column_idx < 0){
      column_idx = chunk_side + column_idx;
    }
    for (int i = column_idx*chunk_side; count < chunk_side; i++){
      this->operator[](i) = value[count];
      count++;
    }
  }

  int get_first_null(){
    for (int i = 0; i < chars_per_chunk; i++){
      if (this->operator[](i) == 0){
        return i;
      }
    }
    return -1;
  }
  int length(){
    int idx = this->get_first_null();
    return idx == -1? chars_per_chunk : idx;
  }
  int size(){
    return this->length();
  }
  int padding(){
    return chars_per_chunk-this->length();
  }

  void push_back(int x){
    int idx = this->get_first_null();
    if (idx != -1){
      this->operator[](idx) = x;
    }
  }
  void extend(Bytearray bytes){
    int idx = this->get_first_null();
    for (int i = idx; i < chars_per_chunk; i++){
      if (i - idx == bytes.length()) return;
      this->operator[](i) = bytes[i - idx];
    }
  }

  types::iarr<chars_per_chunk>::iterator begin(){
    return this->bytes.begin();
  }
  types::iarr<chars_per_chunk>::iterator end(){
    return this->end();
  }

  int& operator[](int idx){
    return this->bytes[idx >= 0? idx : chars_per_chunk + idx];
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

  ByteChunk128& operator=(const ByteChunk128& x){
    for (int i = 0; i < chars_per_chunk; i++){
      this->operator[](i) = x.bytes[i];
    }
    return *this;
  }

  ByteChunk128 operator^(ByteChunk128 key){
    for (int i = 0; i < chars_per_chunk; i++){
      key[i] = key[i] ^ this->operator[](i);
    }
    return key;
  }
  ByteChunk128 operator&(ByteChunk128 key){
    for (int i = 0; i < chars_per_chunk; i++){
      key[i] = key[i] & this->operator[](i);
    }
    return key;
  }
  ByteChunk128 operator|(ByteChunk128 key){
    for (int i = 0; i < chars_per_chunk; i++){
      key[i] = key[i] | this->operator[](i);
    }
    return key;
  }
  
  ByteChunk128 operator<< (int rounds) {
    if (rounds == 0){
      return ByteChunk128();
    }
    types::balist rows = this->get_rows();
    ByteChunk128 result;
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
  ByteChunk128 operator>> (int rounds) {
    if (rounds == 0){
      return ByteChunk128();
    }
    types::balist rows = this->get_rows();
    ByteChunk128 result;
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

  ByteChunk128 shift_rows_left() {
    ByteChunk128 result;
    for (int row_idx = 0; row_idx < chunk_side; row_idx++){
      result.set_row(row_idx, this->get_row(row_idx) << row_idx);
    }
    return result;
  }
  ByteChunk128 shift_rows_right() {
    ByteChunk128 result;
    for (int row_idx = 0; row_idx < chunk_side; row_idx++){
      result.set_row(row_idx, this->get_row(row_idx) >> row_idx);
    }
    return result;
  }

  operator std::string(){
    return convert_to_string(this->bytes);
  }
  operator Bytearray(){
    return Bytearray(this->bytes);
  }
  operator types::ilist(){
    return (types::ilist) (Bytearray(this->bytes));
  }

  std::string hex(){
    types::ilist vct;
    vct.assign(this->bytes.begin(), this->bytes.end());
    return basic_hex(vct);
  }
  std::string oct(){
    types::ilist vct;
    vct.assign(this->bytes.begin(), this->bytes.end());
    return basic_oct(vct);
  }

  static ByteChunk128 from_hex(std::string str){
    return ByteChunk128(basic_from_hex(str));
  }
  static ByteChunk128 from_oct(std::string str){
    return ByteChunk128(basic_from_oct(str));
  }
};
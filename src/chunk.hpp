#pragma once

#include "bytearray.hpp"
#include "constants.hpp"
#include "types.hpp"
#include <stdexcept>
#include <iostream>
#include <memory.h>
#include <vector>
#include <cmath>

using std::string;

class ByteChunk128{
  void basic_constructor(Bytearray bytes){
    memset(this->bytes, 0, sizeof(this->bytes)); // pulisce memoria
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
  
  public:
  int bytes[chars_per_chunk];
  
  ByteChunk128(int x=0){
    memset(this->bytes, x, sizeof(this->bytes));
  }
  ByteChunk128(string bytes){
    this->basic_constructor(Bytearray(bytes));
  }
  ByteChunk128(Bytearray bytes){
    this->basic_constructor(bytes);
  };
  ByteChunk128(types::ilist bytes){
    this->basic_constructor((types::ilist)bytes);
  };
  ByteChunk128(int in_bytes[], int size){
    this->basic_constructor(Bytearray(in_bytes, size));
  }
  
  std::vector<Bytearray> get_rows(){
    std::vector<Bytearray> rows;
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
    int count = 0;
    for (int i = row_idx; i < chars_per_chunk; i += chunk_side){
      this->operator[](i) = value[count];
      count++;
    }
  }
  void set_row(int row_idx, Bytearray value){
    int count = 0;
    for (int i = row_idx; i < chars_per_chunk; i += chunk_side){
      this->operator[](i) = value[count];
      count++;
    }
  }
  
  std::vector<Bytearray> get_columns(){
    std::vector<Bytearray> columns;
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
    int count = 0;
    for (int i = column_idx*chunk_side; count < chunk_side; i++){
      this->operator[](i) = value[count];
      count++;
    }
  }
  void set_column(int column_idx, Bytearray value){
    int count = 0;
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

  int* begin(){
    return this->bytes;
  }
  int* end(){
    return this->begin() + chars_per_chunk;
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

  ByteChunk128 operator=(const ByteChunk128& x){
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
      return ByteChunk128(this->bytes, chars_per_chunk);
    }
    std::vector<Bytearray> rows = this->get_rows();
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
      return ByteChunk128(this->bytes, chars_per_chunk);
    }
    std::vector<Bytearray> rows = this->get_rows();
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
  void operator<<= (int rounds){
    this->operator=(this->operator<<(rounds));
  }
  void operator>>= (int rounds){
    this->operator=(this->operator>>(rounds));
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

  operator string(){
    return convert_to_string(this->bytes, chars_per_chunk);
  }
  operator Bytearray(){
    return Bytearray(this->bytes, chars_per_chunk);
  }
  operator types::ilist(){
    return (types::ilist) (Bytearray(this->bytes, chars_per_chunk));
  }

  string hex(){
    std::stringstream ss;
    
    for (int i = 0; i < chars_per_chunk; i++) {
      ss << std::hex << std::setw(2) << std::setfill('0') << this->bytes[i];
    }

    return ss.str();
  }
  string oct(){
    std::stringstream ss;
    
    for (int i : this->bytes) {
      ss << std::oct << std::setw(3) << std::setfill('0') << i;
    }

    return ss.str();
  }

  static ByteChunk128 from_hex(string str){
    if (str.size() != chars_per_chunk*2){
      throw std::invalid_argument("Input array must be "+std::to_string(chars_per_chunk*2)+", got "+std::to_string(str.size()));
    }
    int vct[chars_per_chunk];

    auto hex_to_int = [](string s) -> int{
      int t = 0;
      for (int n = 0; n < s.length(); n++){
        int current;
        if (s[n] >= 'a' && s[n] <= 'f'){
          current = s[n] - 'a' + 10;
        }
        if (s[n] >= '0' && s[n] <= '9'){
          current = s[n] - '0';
        }
        t += current * pow(16, s.length()-n-1);
      }
      return t;
    };

    for (int i = 0; i < str.length(); i += 2){
      vct[i/2] = hex_to_int({str[i], str[i+1]});
    }
    return ByteChunk128(vct, chars_per_chunk);
  }
  static ByteChunk128 from_oct(string str){
    if (str.size() != chars_per_chunk*3){
      throw std::invalid_argument("Input array must be "+std::to_string(chars_per_chunk*3)+", got "+std::to_string(str.size()));
    }
    int vct[chars_per_chunk];

    auto oct_to_int = [](string s) -> int{
      int t = 0;
      for (int n = 0; n < s.length(); n++){
        int current = s[n] - '0';
        t += current * pow(8, s.length()-n-1);
      }
      return t;
    };

    for (int n = 0; n < str.length(); n += 3){
      vct[n] = oct_to_int({str[n], str[n+1], str[n+2]});
    }

    return ByteChunk128(vct, chars_per_chunk);
  }
};
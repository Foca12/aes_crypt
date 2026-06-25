#pragma once

#include "./bytearray.hpp"
#include "./constants.hpp"
#include "./types.hpp"
#include <stdexcept>
#include <memory.h>
#include <cmath>

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

  int get_first_null(){
    for (int i = 0; i < chars_per_chunk; i++){
      if (this->operator[](i) == 0){
        return i;
      }
    }
    return -1;
  }

  public:
  int bytes[chars_per_chunk];
  
  ByteChunk128(Bytearray bytes){
    this->basic_constructor(bytes);
  };
  ByteChunk128(types::ilist bytes){
    this->basic_constructor((types::ilist)bytes);
  };
  ByteChunk128(int in_bytes[], int size){
    this->basic_constructor(Bytearray(in_bytes, size));
  }
  ByteChunk128(){
    memset(this->bytes, 0, sizeof(this->bytes));
  }
  
  types::chunk_rows get_rows(){
    types::chunk_rows rows;
    // per ogni start di riga (0, 1, 2, 3)
    int array_row_idx = 0;
    for (int chunk_row_idx = 0; chunk_row_idx < chunk_side; chunk_row_idx++){
      rows.push_back(types::ilist {});
      // scorri del chunk_side per prendere il valore della riga (2, 6, 10, 14)
      for (int chunk_idx = chunk_row_idx; chunk_idx < chars_per_chunk; chunk_idx += chunk_side){
        rows[array_row_idx].push_back(bytes[chunk_idx]);
      }
      array_row_idx++;
    }
    return rows;
  }

  int length(){
    int idx = this->get_first_null();
    return idx == -1? chars_per_chunk : idx;
  }
  int size(){
    return this->length();
  }

  int push_back(int x){
    int idx = this->get_first_null();
    if (idx != -1){
      this->operator[](idx) = x;
    }
    return idx == -1;
  }
  
  void extend(Bytearray bytes){
    int idx = this->get_first_null();
    for (int i = idx; i < chars_per_chunk; i++){
      if (i - idx == bytes.length()) return;
      this->operator[](i) = bytes[i - idx];
    }
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
    for (int i = 0; i < this->length(); i++){
      this->operator[](i) = x.bytes[i];
    }
    return *this;
  }

  ByteChunk128 operator<< (int rounds) {
    if (rounds == 0){
      return ByteChunk128(this->bytes, chars_per_chunk);
    }
    types::chunk_rows rows = this->get_rows();
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
    types::chunk_rows rows = this->get_rows();
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

  ByteChunk128 shift_left_crypt() {
    types::chunk_rows rows = this->get_rows();
    ByteChunk128 result;
    for (int row_idx = 0; row_idx < chunk_side; row_idx++){
      Bytearray shifted_row (rows[row_idx]);
      shifted_row <<= row_idx;
      result.extend(shifted_row);
    }
    return result;
  }
  ByteChunk128 shift_right_crypt() {
    types::chunk_rows rows = this->get_rows();
    ByteChunk128 result;
    for (int row_idx = 0; row_idx < chunk_side; row_idx++){
      Bytearray shifted_row;
      shifted_row >>= row_idx;
      result.extend(shifted_row);
    }
    return result;
  }

  operator string(){
    std::stringstream ss;
    string str;
    bool only_valid = true;

    for (int i : this->bytes) {
      if (i < 32 || i > 126){
        only_valid = false;
      }
    }

    for (int i : this->bytes) {
      if (only_valid){
        str += (char) i;
      }
      else {
        ss << "\\x";
        ss << std::hex << std::setw(2) << std::setfill('0') << i;
      }
    }

    return only_valid? str : ss.str();
  }
  operator Bytearray(){
    return Bytearray(this->bytes, chars_per_chunk);
  }

  string hex(){
    std::stringstream ss;
    
    for (int i : this->bytes) {
      ss << std::hex << std::setw(2) << std::setfill('0') << i;
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
    if (str.size() > chars_per_chunk){
      throw std::invalid_argument("Input array dimension is bigger than "+std::to_string(n_keys)+", got "+std::to_string(str.size()));
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
      vct[i] = hex_to_int({str[i], str[i+1]});
    }
    return ByteChunk128(vct, chars_per_chunk);
  }
  static ByteChunk128 from_oct(string str){
    if (str.size() > chars_per_chunk){
      throw std::invalid_argument("Input array dimension is bigger than "+std::to_string(n_keys)+", got "+std::to_string(str.size()));
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
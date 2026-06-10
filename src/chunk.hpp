#include "../include/bytearray.hpp"
#include "../include/types.hpp"
#include "../include/constants.hpp"
#include <stdexcept>
#include <memory.h>
#include <cmath>
 
class ByteChunk128{
  void basic_constructor(Bytearray bytes){
    memset(this->bytes, 0, sizeof(this->bytes)); // pulisce memoria
    if (bytes.length() > num_chars){
      throw std::invalid_argument("Input array dimension was bigger than "+std::to_string(num_chars)+", got "+std::to_string(bytes.length()));
    }
    for (int i = 0; i < bytes.length(); i++){
      this->bytes[i] = bytes[i]; // setta array
    }
  }

  int get_first_null(){
    for (int i = 0; i < num_chars; i++){
      if (this->operator[](i) == 0){
        return i;
      }
    }
    return -1;
  }

  public:
  int bytes[num_chars];
  
  ByteChunk128(Bytearray bytes){
    this->basic_constructor(bytes);
  };
  ByteChunk128(types::ilist bytes){
    this->basic_constructor((types::ilist)bytes);
  };
  ByteChunk128(int in_bytes[], int size){
    memset(this->bytes, 0, sizeof(this->bytes));
    if (size > num_chars){
      throw std::invalid_argument("Input array dimension was bigger than "+std::to_string(num_chars)+", got "+std::to_string(size));
    }
    for (int i = 0; i < size; i++){
      this->bytes[i] = in_bytes[i];
    }
  }
  ByteChunk128(const int in_bytes[], int size){
    memset(this->bytes, 0, sizeof(this->bytes));
    if (size > num_chars){
      throw std::invalid_argument("Input array dimension was bigger than "+std::to_string(num_chars)+", got "+std::to_string(size));
    }
    for (int i = 0; i < size; i++){
      this->bytes[i] = in_bytes[i];
    }
  }
  ByteChunk128(){
    memset(this->bytes, 0, sizeof(this->bytes));
  }
  
  types::chunk_rows get_rows(){
    types::chunk_rows rows;
    // per ogni start di riga (0, 1, 2, 3)
    int array_row_idx = 0;
    for (int chunk_row_idx = 0; chunk_row_idx < side; chunk_row_idx++){
      rows.push_back(types::ilist {});
      // scorri del side per prendere il valore della riga (2, 6, 10, 14)
      for (int chunk_idx = chunk_row_idx; chunk_idx < num_chars; chunk_idx += side){
        rows[array_row_idx].push_back(bytes[chunk_idx]);
      }
      array_row_idx++;
    }
    return rows;
  }

  int length(){
    int idx = this->get_first_null();
    return idx == -1? num_chars : idx;
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
    for (int i = idx; i < num_chars; i++){
      if (i - idx == bytes.length()) return;
      this->operator[](i) = bytes[i - idx];
    }
  }

  int& operator[](int idx){
    return this->bytes[idx >= 0? idx : num_chars + idx];
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
      return ByteChunk128(this->bytes, num_chars);
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
      return ByteChunk128(this->bytes, num_chars);
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
    for (int row_idx = 0; row_idx < side; row_idx++){
      Bytearray shifted_row (rows[row_idx]);
      shifted_row <<= row_idx;
      result.extend(shifted_row);
    }
    return result;
  }
  ByteChunk128 shift_right_crypt() {
    types::chunk_rows rows = this->get_rows();
    ByteChunk128 result;
    for (int row_idx = 0; row_idx < side; row_idx++){
      Bytearray shifted_row;
      shifted_row >>= row_idx;
      result.extend(shifted_row);
    }
    return result;
  }

  operator std::string(){
    std::string res = "";
    for (int i = 0; i < num_chars; i++){
      if (this->operator[](i) != 0){
        res += (char)this->operator[](i);
      }
    }
    return res;
  }
}; 
#include "../include/bytearray.hpp"
#include "../include/types.hpp"
#include "../include/constants.hpp"
#include "../include/chunk.hpp"
#include <iostream>
using std::cout;
using std::endl;
#include <stdexcept>
#include <memory.h>
#include <string>
#include <vector>
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

  public:
  int bytes[num_chars];
  
  ByteChunk128(Bytearray bytes){
    this->basic_constructor(bytes);
  };
  ByteChunk128(types::ilist bytes){
    this->basic_constructor((ilist)bytes);
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
  ByteChunk128(){
    memset(this->bytes, 0, sizeof(this->bytes));
  }

  int& operator[](int idx){
    return this->bytes[idx];
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

  ByteChunk128 operator<< (int rounds) {
    if (rounds == 0){
      return ByteChunk128(this->bytes, num_chars);
    }
    int side = sqrt(num_chars);
    Bytearray result (num_chars);
    // per ogni start di riga (es: 0, 1, 2, 3)
    for (int row_idx = 0; row_idx < side; row_idx++){
      Bytearray shifted_row;
      // aumenta del lato per scorrere la riga (es: 4, )
      for (int actual_idx = row_idx; actual_idx < num_chars; actual_idx+=side){
        shifted_row.push_back(this->operator[](actual_idx));
      }
      shifted_row <<= row_idx;
      // assegna la riga alla colonna
      for (int actual_idx = 0; actual_idx < num_chars; actual_idx+=side){
        cout << actual_idx << endl;
        result[actual_idx+row_idx] = shifted_row[actual_idx / side];
      }
    }
    return (ByteChunk128 (result)) << rounds-1;
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
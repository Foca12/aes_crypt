#pragma once

#include "../matrix.hpp"

namespace crypt_operations
{
  Bytearray shift_left(Bytearray input){
    return ByteMatrix::divide_bytearray(input).shift_rows_left();
  }
  ByteChunk128 shift_left(ByteChunk128 input){
    return input.shift_rows_left();
  }
  ByteMatrix shift_left(ByteMatrix input){
    return input.shift_rows_left();
  }
  Bytearray shift_right(Bytearray input){
    return ByteMatrix::divide_bytearray(input).shift_rows_right();
  }
  ByteChunk128 shift_right(ByteChunk128 input){
    return input.shift_rows_right();
  }
  ByteMatrix shift_right(ByteMatrix input){
    return input.shift_rows_right();
  }
}
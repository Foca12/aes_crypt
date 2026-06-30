#pragma once

#include "../matrix.hpp"

namespace crypt_operations
{
  Bytearray shift_rows(Bytearray input){
    return ByteMatrix::divide_bytearray(input).shift_rows_left();
  }
  ByteChunk128 shift_rows(ByteChunk128 input){
    return input.shift_rows_left();
  }
  ByteMatrix shift_rows(ByteMatrix input){
    return input.shift_rows_left();
  }
  Bytearray inv_shift_rows(Bytearray input){
    return ByteMatrix::divide_bytearray(input).shift_rows_right();
  }
  ByteChunk128 inv_shift_rows(ByteChunk128 input){
    return input.shift_rows_right();
  }
  ByteMatrix inv_shift_rows(ByteMatrix input){
    return input.shift_rows_right();
  }
}
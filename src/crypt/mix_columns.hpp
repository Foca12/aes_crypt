#pragma once

#include <stdexcept>
#include <functional>
#include "../constants.hpp"
#include "../matrix.hpp"
#include "../chunks/chunk128.hpp"

namespace crypt_operations
{ 
  Bytearray basic_mix_columns(Bytearray column, const std::function<int(int)> matrix[chunk128_side][chunk128_side]){
    if (column.length() != chunk128_side){
      throw std::invalid_argument("Error, column length must be "+std::to_string(chunk128_side)+", got "+std::to_string(column.length()));
    }
    Bytearray column_result;
    for (int result_idx = 0; result_idx < chunk128_side; result_idx++){
      int sum = 0;
      for (int mul_idx = 0; mul_idx < chunk128_side; mul_idx++){
        sum ^= matrix[result_idx][mul_idx](column[mul_idx]);
      }
      column_result.push_back(sum);
    }
    return column_result;
  }

  Bytearray mix_columns(Bytearray column){
    return basic_mix_columns(column, mul_matrix);
  }
  ByteChunk128 mix_columns(ByteChunk128 state){
    ByteChunk128 result;
    for (int i = 0; i < chunk128_side; i++){
      result.set_column(i, mix_columns(state.get_column(i)));
    }
    return result;
  }
  ByteMatrix mix_columns(ByteMatrix matrix){
    ByteMatrix result;
    for (ByteChunk128 i : matrix.chunk_iterator()){
      result.extend(mix_columns(i));
    }
    return result;
  }
  
  Bytearray inv_mix_columns(Bytearray column){
    return basic_mix_columns(column, inv_mul_matrix);
  }
  ByteChunk128 inv_mix_columns(ByteChunk128 state){
    ByteChunk128 result;
    for (int i = 0; i < chunk128_side; i++){
      result.set_column(i, inv_mix_columns(state.get_column(i)));
    }
    return result;
  }
  ByteMatrix inv_mix_columns(ByteMatrix matrix){
    ByteMatrix result;
    for (ByteChunk128 i : matrix.chunk_iterator()){
      result.extend(inv_mix_columns(i));
    }
    return result;
  }
}
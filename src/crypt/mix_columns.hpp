#pragma once

#include <stdexcept>
#include "../constants.hpp"
#include "../matrix.hpp"
#include "../chunk.hpp"

namespace crypt_operations
{ 
  Bytearray mix_columns(Bytearray column){
    if (column.length() != chunk_side){
      throw std::invalid_argument("Error, column length must be "+std::to_string(chunk_side)+", got "+std::to_string(column.length()));
    }
    Bytearray column_result;
    for (int result_idx = 0; result_idx < chunk_side; result_idx++){
      int sum = 0;
      for (int mul_idx = 0; mul_idx < chunk_side; mul_idx++){
        sum ^= mul_matrix[result_idx][mul_idx](column[mul_idx]);
      }
      column_result.push_back(sum);
    }
    return column_result;
  }
  
  ByteChunk128 mix_columns(ByteChunk128 state){
    ByteChunk128 result;
    for (int i = 0; i < chunk_side; i++){
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
}
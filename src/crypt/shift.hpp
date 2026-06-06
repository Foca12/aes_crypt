#include "../../include/chunk.hpp"
#include "../../include/matrix.hpp"
#include "../../include/constants.hpp"
#include "../../include/types.hpp"
#include <functional>
#include <cmath>

ByteMatrix shift_left(ByteMatrix input){
  ByteMatrix result;
  for (int bc_idx = 0; bc_idx < input.length(); bc_idx++){
    result.push_back(input.get_chunk(bc_idx) << 1);
  }
  return result;
}
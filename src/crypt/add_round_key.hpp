#pragma once

#include <stdexcept>
#include "../matrix.hpp"
#include "../keys.hpp"
#include "../constants.hpp"

namespace crypt_operations
{
  ByteChunk128 add_round_key(ByteChunk128 msg, Key round_key, int round){
    if (! round_key.is_expanded()){
      throw std::runtime_error("Error: the key is not expanded, to expand it use key.expand()");
    }

    return msg ^ round_key.get_key(round);
  }

  ByteMatrix add_round_key(ByteMatrix msg, Key round_key, int round){
    ByteMatrix result(0);
    for (ByteChunk128 i : msg.chunk_iterator()){
      result.extend(add_round_key(i, round_key, round));
    }
    return result;
  }
}
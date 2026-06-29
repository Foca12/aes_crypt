#pragma once

#include "../matrix.hpp"
#include "../constants.hpp"

namespace crypt_operations
{
  ByteMatrix sub_bytes(ByteMatrix input){
    for (int i = 0; i < chars_per_chunk * input.length(); i++){
      input[i] = sbox[input[i]];
    }
    return input;
    }
  ByteChunk128 sub_bytes(ByteChunk128 input){
    for (int i = 0; i < chars_per_chunk; i++){
      input[i] = sbox[input[i]];
    }
    return input;
  }
  Bytearray sub_bytes(Bytearray input){
    for (int i = 0; i < input.length(); i++){
      input[i] = sbox[input[i]];
    }
    return input;
  }

  ByteMatrix desub_bytes(ByteMatrix input){
    for (int i = 0; i < chars_per_chunk * input.length(); i++){
      input[i] = inv_sbox[input[i]];
    }
    return input;
  }
  ByteChunk128 desub_bytes(ByteChunk128 input){
    for (int i = 0; i < chars_per_chunk; i++){
      input[i] = inv_sbox[input[i]];
    }
    return input;
  }
  Bytearray desub_bytes(Bytearray input){
    for (int i = 0; i < input.length(); i++){
      input[i] = inv_sbox[input[i]];
    }
    return input;
  }
}

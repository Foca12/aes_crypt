#pragma once

#include "../chunk.hpp"
#include "./substitute.hpp"
#include "../bytearray.hpp"
#include "..//types.hpp"
#include "../constants.hpp"
#include <memory>
#include <cmath>

Bytearray g(Bytearray x, int round){
  x <<= 1;
  x = crypt_operations::crypt_substitute(x);
  x[0] = x[0] ^ rcon[round];
  return x;
}


class ExpandedKey{
  bool expanded = false;

  public:
  ByteChunk128 keys[n_keys];

  ExpandedKey(ByteChunk128 key){
    memset(this->keys, 0, sizeof(this->keys));
    keys[0] = key;
  }
  ExpandedKey(ByteChunk128 keys[], int size){
    memset(this->keys, 0, sizeof(this->keys));
    if (size > n_keys){
      throw std::invalid_argument("Input array dimension is bigger than "+std::to_string(n_keys)+", got "+std::to_string(size));
    }
    if (size < 0){
      throw std::invalid_argument("Input array dimension must be a positive integer, got "+std::to_string(size));
    }
    for (int i = 0; i < size; i++){
      this->keys[i] = keys[i];
    }
  }
  ExpandedKey(std::vector<ByteChunk128> keys){
    memset(this->keys, 0, sizeof(this->keys));
    if (keys.size() > n_keys){
      throw std::invalid_argument("Input array dimension is bigger than "+std::to_string(n_keys)+", got "+std::to_string(keys.size()));
    }
    if (keys.size() < 0){
      throw std::invalid_argument("Input array dimension must be a positive integer, got "+std::to_string(keys.size()));
    }
    for (int i = 0; i < keys.size(); i++){
      this->keys[i] = keys[i];
    }
  }

  
};
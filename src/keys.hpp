#pragma once

#include "./crypt/substitute.hpp"
#include "chunk.hpp"
#include "bytearray.hpp"
#include "types.hpp"
#include "constants.hpp"
#include <memory>
#include <cmath>

Bytearray g(Bytearray x, int round){
  x <<= 1;
  x = crypt_operations::crypt_substitute(x);
  x[0] = x[0] ^ rcon[round];
  return x;
}


class Key{
  bool expanded = false;

  public:
  ByteChunk128 keys[n_keys];

  Key(ByteChunk128 key){
    memset(this->keys, 0, sizeof(this->keys));
    keys[0] = key;
  }
  
  void expand(){
    if (this->expanded) return;

    for (int key_idx = 1; key_idx < n_keys; key_idx++){
      ByteChunk128 current_key = this->keys[0];

      for (int row_idx = 0; row_idx < chunk_side; row_idx++){
        Bytearray current_row;
        if (row_idx == 0){
          current_row = g(current_key.get_row(0), key_idx);
        }
        else{
          current_row = current_key.get_row(row_idx) ^ (this->keys[key_idx-1].get_row(row_idx));
        }
        current_key.set_row(row_idx, current_row);
      }

      this->keys[key_idx] = current_key;
    }
  }
};
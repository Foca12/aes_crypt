#pragma once

#include <iostream>

#include "./crypt/substitute.hpp"
#include "chunk.hpp"
#include "bytearray.hpp"
#include "types.hpp"
#include "constants.hpp"
#include <memory>
#include <cmath>

Bytearray g(ByteChunk128 x, int round){
  Bytearray column(4);
  column = x.get_column(chunk_side-1) << 1;
  column = crypt_operations::crypt_substitute(column);
  column[0] = column[0] ^ rcon[round];
  return column;
}

class Key{
  bool expanded = false;

  public:
  ByteChunk128 keys[n_keys];

  Key(ByteChunk128 key){
    memset(this->keys, 0, sizeof(this->keys));
    this->keys[0] = key;
  }
  
  void expand(){
    if (this->expanded) return;

    for (int key_idx = 1; key_idx < n_keys; key_idx++){
      ByteChunk128 current_key;
      ByteChunk128 last_key = this->keys[key_idx-1];

      for (int column_idx = 0; column_idx < words_per_key; column_idx++){
        Bytearray current_column;
        if (column_idx == 0){
          current_column = g(last_key, key_idx) ^ last_key.get_column(0);
        }
        else{
          current_column = current_key.get_column(column_idx-1) ^ this->keys[key_idx-1].get_column(column_idx);
        }
        current_key.set_column(column_idx, current_column);
      }

      this->keys[key_idx] = current_key;
    }
  }

  std::vector<std::string> hex(){
    std::vector<std::string> strs = {};
    for (int i = 0; i < n_keys; i++){
      strs.push_back(this->keys[i].hex());
    }
    return strs;
  }
  std::vector<std::string> get_keys(){
    std::vector<std::string> strs = {};
    for (int i = 0; i < n_keys; i++){
      strs.push_back((string) this->keys[i]);
    }
    return strs;
  }

  operator std::string() {
    return convert_to_string(this->keys[0].bytes, chars_per_chunk);
  }
  operator types::ilist() {
    std::vector<int> vct;
    vct.assign(this->keys[0].bytes, this->keys[0].bytes+chars_per_chunk);
    return vct;
  }
  operator Bytearray() {
    std::vector<int> vct;
    vct.assign(this->keys[0].bytes, this->keys[0].bytes+chars_per_chunk);
    return Bytearray(vct);
  }
};
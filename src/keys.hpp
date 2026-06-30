#pragma once

#include <iostream>
#include <stdexcept>

#include "./crypt/sub_bytes.hpp"
#include "chunk.hpp"
#include "bytearray.hpp"
#include "types.hpp"
#include "constants.hpp"

Bytearray g(ByteChunk128 x, int round){
  Bytearray column(4);
  column = x.get_column(chunk_side-1) << 1;
  column = crypt_operations::sub_bytes(column);
  column[0] = column[0] ^ rcon[round];
  return column;
}

class Key{
  bool expanded = false;
  types::key_array keys = {};

  public:
  Key(){}
  Key(ByteChunk128 key){
    this->keys[0] = key;
  }

  void expand(){
    if (this->expanded) return;

    this->expanded = true;

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

  bool is_expanded() {return this->expanded;}

  types::bclist get_keys(){
    types::bclist vct;
    vct.assign(this->keys.begin(), this->keys.end());
    return vct;
  }
  ByteChunk128 get_key(int idx){
    return this->keys[idx];
  }

  std::string hex(){
    return this->keys[0].hex();
  }
  std::string oct(){
    return this->keys[0].oct();
  }

  Key operator=(ByteChunk128 chunk){
    if (this->expanded){
      throw std::runtime_error("Trying to assign value to an already expanded key");
    }
    this->keys[0] = chunk;
    return *this;
  }
  Key& operator=(Key key){
    if (this->expanded){
      if (!key.expanded){
        throw std::runtime_error("Trying to assign value of a not expanded key to an already expanded key");
      }
      for (int i = 0; i < n_keys; i++){
        this->keys[i] = key.keys[i];
      }
    }
    if (!this->expanded){
      if (key.expanded){
        throw std::runtime_error("Trying to assign value of already expanded key to a not expanded key");
      }
      this->keys[0] = key.keys[0];
    }
    return *this;
  }

  operator std::string() {
    return convert_to_string(this->keys[0]);
  }
  operator types::ilist() {
    std::vector<int> vct;
    vct.assign(this->keys[0].begin(), this->keys[0].end());
    return vct;
  }
  operator Bytearray() {
    std::vector<int> vct;
    vct.assign(this->keys[0].begin(), this->keys[0].end());
    return Bytearray(vct);
  }
  static Key from_hex(std::string str){
    Key key;
    if (str.length() == chars_per_chunk*2){
      key.keys[0] = ByteChunk128::from_hex(str);
    }
    else{
      throw std::invalid_argument("Input array must be "+std::to_string(chars_per_chunk*2)+" or "+std::to_string(chars_per_chunk*2*n_keys)+", got "+std::to_string(str.size()));
    }
    return key;
  }
  static Key from_oct(std::string str){
    Key key;
    if (str.length() == chars_per_chunk*3){
      key.keys[0] = ByteChunk128::from_oct(str);
    }
    else{
      throw std::invalid_argument("Input array must be "+std::to_string(chars_per_chunk*2)+"or "+std::to_string(chars_per_chunk*2*n_keys)+", got "+std::to_string(str.size()));
    }
    return key;
  }
};
#pragma once

#include <string>
#include <vector>
#include "src/crypt/add_round_key.hpp"
#include "src/crypt/mix_columns.hpp"
#include "src/crypt/shift_rows.hpp"
#include "src/crypt/sub_bytes.hpp"
#include "src/keys.hpp"
#include "src/matrix.hpp"
#include "src/constants.hpp"

ByteMatrix crypt_aes_128(ByteMatrix msg, Key key){
  key.expand();
  ByteMatrix crypted = msg;
  for (int round = 0; round < n_keys; round++){
    if (round > 0){
      crypted = crypt_operations::sub_bytes(crypted);
      crypted = crypt_operations::shift_left(crypted);
      if (round < n_keys-1){
        crypted = crypt_operations::mix_columns(crypted);
      }
    }
    crypted = crypt_operations::add_round_key(crypted, key, round);
  }
  return crypted;
}
ByteChunk128 crypt_aes_128(ByteChunk128 msg, Key key){
  return crypt_aes_128(ByteMatrix(std::vector<ByteChunk128> {msg}), key).get_chunk(0);
}
Bytearray crypt_aes_128(Bytearray msg, Key key){
  return crypt_aes_128(ByteMatrix::divide_bytearray(msg), key);
}
Bytearray crypt_aes_128(std::string msg, Key key){
  return crypt_aes_128(ByteMatrix::divide_string(msg), key);
}

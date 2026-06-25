#include "../../include/crypt_operations.hpp"
#include "../../include/matrix.hpp"
#include "../../include/constants.hpp"

namespace crypt_operations
{
  ByteMatrix crypt_substitute(ByteMatrix input){
    for (int i = 0; i < num_chars * input.length(); i++){
      input[i] = sbox[input[i]];
    }
    return input;
    }
  ByteChunk128 crypt_substitute(ByteChunk128 input){
    for (int i = 0; i < num_chars; i++){
      input[i] = sbox[input[i]];
    }
    return input;
  }
  Bytearray crypt_substitute(Bytearray input){
    for (int i = 0; i < input.length(); i++){
      input[i] = sbox[input[i]];
    }
    return input;
  }

  ByteMatrix decrypt_substitute(ByteMatrix input){
    for (int i = 0; i < num_chars * input.length(); i++){
      input[i] = inv_sbox[input[i]];
    }
    return input;
  }
  ByteChunk128 decrypt_substitute(ByteChunk128 input){
    for (int i = 0; i < num_chars; i++){
      input[i] = inv_sbox[input[i]];
    }
    return input;
  }
  Bytearray decrypt_substitute(Bytearray input){
    for (int i = 0; i < input.length(); i++){
      input[i] = inv_sbox[input[i]];
    }
    return input;
  }
}

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
  ByteMatrix decrypt_substitute(ByteMatrix input){
      for (int i = 0; i < num_chars * input.length(); i++){
        input[i] = inv_sbox[input[i]];
      }
      return input;
    }
}

#pragma once

#include "../matrix.hpp"

namespace crypt_operations{
  ByteMatrix shift_left(ByteMatrix input){
    return input.shift_left_crypt();
  }
}
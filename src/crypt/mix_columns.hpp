#pragma once

#include "../constants.hpp"
#include "../matrix.hpp"
#include "../chunk.hpp"

#define mul_01(x) (x)
#define mul_02(x) xtime(x)
#define mul_03(x) (xtime(x) ^ (x))

int xtime(int x){
  if (x & 0x80){
    return (x ^ 0x1b) << 1;
  }
  return x << 1;
}
ByteChunk128 xtime(ByteChunk128 x){
  for (int i = 0; i < chars_per_chunk; i++){
    x[i] = xtime(x[i]);
  }
  return x;
}
ByteMatrix xtime(ByteMatrix x){
  for (int i = 0; i < x.length(); i++){
    x[i] = xtime(x[i]);
  }
  return x;
}

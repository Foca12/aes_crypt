#include "../../include/constants.hpp"
#include "../../include/matrix.hpp"
#include "../../include/chunk.hpp"

int xtime(int x){
  if (x & 0x80){
    return (x ^ 0x1b) << 1;
  }
  return x << 1;
}
ByteChunk128 xtime(ByteChunk128 x){
  for (int i = 0; i < num_chars; i++){
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
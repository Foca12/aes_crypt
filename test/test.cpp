#include <iostream>
#include "../include/bytearray.hpp"
#include "../include/matrix.hpp"
#include "../include/crypt_operations.hpp"
#include "../include/types.hpp"

using namespace std;

int main(){
  Bytearray byte ("0123456789abcdef");

  // 0 4 8 c
  // 1 5 9 d
  // 2 6 a e
  // 3 7 b f

  ByteChunk128 chunk (byte);
  ByteMatrix matrix = ByteMatrix::divide_bytearray(byte);

  cout << xtime(0b00101101);

  return 0;
}
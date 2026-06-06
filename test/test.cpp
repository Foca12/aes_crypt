#include <iostream>
#include "../include/bytearray.hpp"
#include "../include/matrix.hpp"
#include "../include/crypt_operations.hpp"
#include "../include/types.hpp"

using namespace std;

int main(){
  Bytearray byte ("0123456789abcdef");
  ByteChunk128 chunk (byte);
  ByteMatrix matrix = ByteMatrix::divide_bytearray(byte);

  cout << (string)(byte << 1) << endl;

  return 0;
}
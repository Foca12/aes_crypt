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
  // ByteMatrix msg = ByteMatrix::divide_bytearray(byte);
  // auto crypt = crypt_operations::crypt_substitute(msg);
  // auto decrypt = crypt_operations::decrypt_substitute(crypt);

  cout << (string)chunk << endl;
  cout << (string)(shift_left(matrix)) << endl;

  return 0;
}
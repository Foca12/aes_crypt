#pragma once

#include "constants.hpp"

class Bytearray;
class ByteChunk128;

namespace types
{
  using ilist = std::vector<int>;
  using ilist_iterator = std::vector<int>::iterator;

  using balist = std::vector<Bytearray>;

  template <int len>
  using iarr = std::array<int, len>;
  template <int len>
  using iarr_iterator = typename std::array<int, len>::iterator;

  using bclist = std::vector<ByteChunk128>;
  using bclist_iterator = bclist::iterator;

  using key_array = std::array<ByteChunk128, n_keys>;
}
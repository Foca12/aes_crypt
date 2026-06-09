#pragma once

#include "./constants.hpp"
#include <vector>

class ByteChunk128;

namespace types
{
  typedef std::vector<int> ilist;
  typedef std::vector<int>::iterator ilist_iterator;
  typedef std::vector<ByteChunk128> bclist;
  typedef types::bclist::iterator bclist_iterator;
  typedef std::vector<types::ilist> chunk_rows;
}
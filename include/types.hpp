#pragma once

#include "./constants.hpp"
#include "../src/crypt/keys.hpp"
#include "./chunk.hpp"
#include <vector>

namespace types
{
  typedef std::vector<int> ilist;
  typedef std::vector<int>::iterator ilist_iterator;
  typedef std::vector<ByteChunk128> bclist;
  typedef types::bclist::iterator bclist_iterator;
  typedef std::vector<types::ilist> chunk_rows;
}
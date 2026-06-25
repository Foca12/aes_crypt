#pragma once

#include "./chunk.hpp"
#include <vector>

namespace types
{
  typedef std::vector<int> ilist;
  typedef std::vector<int>::iterator ilist_iterator;
  typedef std::vector<types::ilist> chunk_rows;
}
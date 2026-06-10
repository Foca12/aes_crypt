#include "../../include/chunk.hpp"
#include "../../include/bytearray.hpp"
#include "../../include/types.hpp"
#include "../../include/constants.hpp"
#include <cmath>

class Key : public ByteChunk128{
  int idx = 0;

  public:
  Key() : ByteChunk128() {}
  Key(Bytearray bytes) : ByteChunk128((types::ilist)bytes){}

  Bytearray get_slice(){
    return ByteChunk128::slice(this->idx, this->idx+step);
  }
  Bytearray get_slice(int i){
    return ByteChunk128::slice(i * step, i * step + step);
  }

  void inc(){
    this->idx += step;
    if (this->idx == num_chars){
      this->idx = 0;
    }
  }
  void dec(){
    this->idx = this->idx > 0? this->idx - step : 0;
  }

  Key& operator=(Key key){
    this->idx = key.idx;
    ByteChunk128::operator=(ByteChunk128(key.bytes, num_chars));
    return *this;
  }
};

class ExpandedKey{
  types::key_array keys;
  bool expanded = false;
  public:
  ExpandedKey (Key key){
    this->keys[0] = key;
  }

  Key& operator[](int idx){
    return this->keys[idx];
  }
  Key& get_slice(int idx){
    return this->operator[](idx);
  }

  void expand(){
    if (this->expanded) return;
    for (int idx = words_per_key * step; idx < total_words; idx += step){
      Key current = this->operator[](idx);
      if (idx % step != 0){

      }
    }
  }
};
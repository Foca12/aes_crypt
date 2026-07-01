#pragma once

#include "./chunks/chunk128.hpp"
#include "./constants.hpp"
#include "./types.hpp"

class ByteMatrix{
  types::bclist chunks; // vettore di chunks

  public:
  ByteMatrix(int n=0){
    if (n < 0){
      throw std::invalid_argument("N must be bigger or equal than 0, got "+std::to_string(n));
    }
    this->chunks.insert(this->chunks.end(), n, ByteChunk128());
  }
  ByteMatrix(types::bclist list){
    this->chunks = list;
  }

  // costruttori alternativi che dividono il testo
  static ByteMatrix divide_string(std::string bytes){
    return ByteMatrix::divide_bytearray(Bytearray(bytes));
  }
  static ByteMatrix divide_bytearray(Bytearray bytes){
    types::bclist chunks;
    int i = 0;
    for (; i < bytes.length()-16; i += chars_per_chunk128){
      chunks.push_back(ByteChunk128(bytes.slice(i, i+chars_per_chunk128))); // riempe vettore chunks con i chunks
    }
    chunks.push_back(ByteChunk128(bytes.slice(i, bytes.length()))); // aggiunge chunk con padding
    return ByteMatrix(chunks);
  }

  // metodi vettore
  int length() {
    return this->chunks.size();
  }
  int size() {
    return this->length();
  }
  
  void push_back(int x) {
    if (this->get_padding() > 0){
      this->get_chunk(-1).push_back(x);
    }
    else{
      ByteChunk128 chunk(0);
      chunk[0] = x;
      this->extend(chunk);
    }
  }
  void extend(Bytearray x){
    int i = 0;
    for (; i < floor(x.length() / chars_per_chunk128); i++){
      this->extend(ByteChunk128(x.slice(i*chars_per_chunk128, (i+1)*chars_per_chunk128)));
    }
    this->extend(ByteChunk128(x.slice(i*chars_per_chunk128, x.length())));
  }
  void extend(ByteMatrix x) {
    for (ByteChunk128 i : x.chunk_iterator()){
      this->extend(i);
    }
  }
  void extend(ByteChunk128 x){
    int padding = this->get_padding();
    if (padding <= 0){
      this->chunks.push_back(x);
      return;
    }
    bool end = false;
    if (padding > x.length()){
      padding = x.length();
      end = true;
    }
    this->get_chunk(-1).extend(x.slice(padding));
    if (!end){
      this->extend(x.slice(padding, x.length()));
    }
  }

  ByteMatrix slice(int start, int stop, int step){
    ByteMatrix result;
    for (int i = start; i < stop; i += step){
      result.push_back(this->operator[](i));
    }
    return result;
  }
  ByteMatrix slice(int start, int stop){
    return this->slice(start, stop, 1);
  }
  ByteMatrix slice(int stop){
    return this->slice(0, stop, 1);
  }

  int& operator[](int idx) {
    int chunk_idx = floor(abs(idx) / chars_per_chunk128);
    int num_idx = abs(idx) % chars_per_chunk128;
    if (idx < 0){
      chunk_idx = this->length() - chunk_idx - 1;
      num_idx = chars_per_chunk128 - num_idx;
    }
    return this->chunks[chunk_idx][num_idx];
  }
  int& at(int chunk_idx, int num_idx){
    return this->chunks[chunk_idx >= 0? chunk_idx : this->length()+chunk_idx][num_idx >= 0? num_idx : chars_per_chunk128+num_idx];
  }
  ByteChunk128& get_chunk(int idx){
    return this->chunks[idx >= 0? idx : this->length() + idx];
  }
  int get_padding(){
    if (this->length() == 0){return -1;} 
    return this->get_chunk(-1).padding();
  }

  types::iarr_iterator<chars_per_chunk128> begin(){
    return this->chunks[0].begin();
  }
  types::iarr_iterator<chars_per_chunk128> end(){
    return this->chunks[this->length()-1].end();
  }
  
  struct support_iterator {
    types::bclist support_chunks;

    types::bclist_iterator begin(){
      return this->support_chunks.begin();
    }
    types::bclist_iterator end(){
      return this->support_chunks.end();
    }
  };
  support_iterator chunk_iterator(){
    return support_iterator{this->chunks};
  }

  ByteMatrix& operator=(ByteMatrix matrix){
    this->chunks = matrix.chunks;
    return *this;
  }

  ByteMatrix operator^(ByteChunk128 chunk){
    ByteMatrix result;
    for (ByteChunk128 i : this->chunk_iterator()){
      result.extend(chunk ^ i);
    }
    return result;
  }
  ByteMatrix operator&(ByteChunk128 chunk){
    ByteMatrix result;
    for (ByteChunk128 i : this->chunk_iterator()){
      result.extend(chunk & i);
    }
    return result;
  }
  ByteMatrix operator|(ByteChunk128 chunk){
    ByteMatrix result;
    for (ByteChunk128 i : this->chunk_iterator()){
      result.extend(chunk | i);
    }
    return result;
  }

  ByteMatrix operator<<(int rounds){
    ByteMatrix result (0);
    for (ByteChunk128 i : this->chunk_iterator()){
      result.extend(i << rounds);
    }
    return result;
  }
  ByteMatrix operator>>(int rounds){
    ByteMatrix result (0);
    for (ByteChunk128 i : this->chunk_iterator()){
      result.extend(i >> rounds);
    }
    return result;
  }
  ByteMatrix& operator<<=(int rounds){
    return this->operator=(this->operator<<(rounds));
  }
  ByteMatrix& operator>>=(int rounds){
    return this->operator=(this->operator>>(rounds));
  }

  ByteMatrix shift_rows_left(){
    ByteMatrix result (0);
    for (ByteChunk128 i : this->chunk_iterator()){
      result.extend(i.shift_rows_left());
    }
    return result;
  }
  ByteMatrix shift_rows_right(){
    ByteMatrix result (0);
    for (ByteChunk128 i : this->chunk_iterator()){
      result.extend(i.shift_rows_right());
    }
    return result;
  }

  // cast di tipi
  operator std::string() {
    std::string str = "";
    for (auto i : this->chunk_iterator()){
      str += (std::string)i;
    }
    return str;
  }
  operator types::ilist(){
    types::ilist list = {};
    for (auto i : this->chunks){
      list.insert(list.end(), ((types::ilist)i).begin(), ((types::ilist)i).end());
    }
    return list;
  }
  operator Bytearray(){
    Bytearray array;
    for (auto i : this->chunks){
      array.extend((Bytearray) i);
    }
    return array;
  }

  // conversioni di formato
  std::string hex(){
    std::stringstream ss;
    
    for (auto i : this->chunks) {
      ss << std::hex << std::setw(2) << std::setfill('0') << i.hex();
    }

    return ss.str();
  }
  std::string oct(){
    std::stringstream ss;
    
    for (auto i : this->chunks) {
      ss << std::oct << std::setw(3) << std::setfill('0') << i.oct();
    }

    return ss.str();
  }

  // costruttori alternativi
  static ByteMatrix from_hex(std::string str){
    Bytearray array = Bytearray::from_hex(str);
    return ByteMatrix::divide_bytearray(array);
  }
  static ByteMatrix from_oct(std::string str){
    Bytearray array = Bytearray::from_oct(str);
    return ByteMatrix::divide_bytearray(array);
  }
};
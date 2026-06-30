#pragma once

#include "types.hpp" 
#include "constants.hpp"

class Bytearray{
  types::ilist bytes = {};
  public:
  // costruttori
  Bytearray(int x=0){
    this->bytes.insert(this->bytes.end(), x, 0);
  }
  Bytearray(std::string str){
    for (int i : str){
      this->bytes.push_back(i);
    }
  }
  Bytearray(types::ilist str){
    this->extend(str);
  }
  Bytearray(int str[], int length){
    this->bytes.insert(this->bytes.end(), str, str+length);
  }
  template <size_t len>
  Bytearray(types::iarr<len> str){
    this->bytes.assign(str.begin(), str.end());
  }

  // metodi vettori
  void push_back(int n){
    this->bytes.push_back(n);
  }
  
  void extend(std::string str) {
    for (int i : str){
      this->bytes.push_back(i);
    }
  }
  void extend(types::ilist str) {
    this->bytes.insert(this->bytes.end(), str.begin(), str.end());
  }
  void extend(Bytearray str){
    this->extend(str.bytes);
  }

  int length(){
    return this->bytes.size();
  }
  int size(){
    return this->bytes.size();
  }
  
  Bytearray slice(int start, int stop, int step){
    Bytearray result;
    for (int i = start; i < stop; i += step){
      result.push_back(this->operator[](i));
    }
    return result;
  }
  Bytearray slice(int start, int stop){
    return this->slice(start, stop, 1);
  }
  Bytearray slice(int stop){
    return this->slice(0, stop, 1);
  }

  int& operator[](int idx){
    return this->bytes[idx >= 0? idx : this->length() + idx];
  }
  
  types::ilist_iterator begin(){
    return this->bytes.begin();
  }
  types::ilist_iterator end(){
    return this->begin()+this->length();
  }
  
  Bytearray& operator=(const Bytearray& x){
    this->bytes = x.bytes;
    return *this;
  }

  // shift
  Bytearray operator<<(int rounds){
    Bytearray copy (this->bytes);
    rounds = rounds % copy.length();
    // se devi fare 0 shift o il bytearray ha 0 o 1 elemento
    if (rounds == 0 || copy.length() < 2){
      return copy;
    }
    // l'elemento che sta prima diventa uguale a quello dopo
    for (int i = 0; i < copy.length(); i++){
      copy[i-1] = copy[i];
    }
    // perché l'ultimo elemento viene perso
    copy[-2] = this->operator[](-1);
    return copy << rounds-1;
  }
  Bytearray operator>>(int rounds){
    Bytearray copy (this->bytes);
    rounds = rounds % copy.length();
    // se devi fare 0 shift o il bytearray ha 0 o 1 elemento
    if (rounds == 0 || copy.length() < 2){
      return copy;
    }
    // l'elemento che sta dopo diventa uguale a quello prima
    for (int i = copy.length()-1; i > 0; i--){
      copy[i] = copy[i-1];
    }
    // perché l'ultimo elemento viene perso
    copy[0] = this->operator[](-1);
    return copy >> rounds-1;
  }
  Bytearray& operator<<=(int rounds){
    return (this->operator= (this->operator<<(rounds)));
  }
  Bytearray& operator>>=(int rounds){
    return (this->operator= (this->operator>>(rounds)));
  }
  
  // operatori logici
  Bytearray operator|(Bytearray arr){
    Bytearray xored;

    if (this->length() == 0){
      return xored;
    };

    if (arr.length() == 0){
      return *this;
    };

    for (int i = 0; i < this->length(); i++){
      int xored_char = this->bytes[i] | arr[i % arr.length()];
      xored.push_back(xored_char);
    }
    return xored;
    }
  Bytearray operator&(Bytearray arr){
    Bytearray xored;

    if (this->length() == 0){
      return xored;
    };

    if (arr.length() == 0){
      return *this;
    };

    for (int i = 0; i < this->length(); i++){
      int xored_char = this->bytes[i] & arr[i % arr.length()];
      xored.push_back(xored_char);
    }
    return xored;
    }
  Bytearray operator^(Bytearray arr){
    Bytearray xored;

    if (this->length() == 0){
      return xored;
    };

    if (arr.length() == 0){
      return *this;
    };

    for (int i = 0; i < this->length(); i++){
      int xored_char = this->bytes[i] ^ arr[i % arr.length()];
      xored.push_back(xored_char);
    }
    return xored;
  }
  
  // conversioni di tipo
  operator std::string(){
    return convert_to_string(this->bytes);
  }
  operator types::ilist(){
    return this->bytes;
  }

  // conversioni di formato
  std::string hex(){
    return basic_hex(this->bytes);
  }
  std::string oct(){
    return basic_oct(this->bytes);
  }

  // costruttori alternativi
  static Bytearray from_hex (std::string str){
    return Bytearray(basic_from_hex(str));
  }
  static Bytearray from_oct (std::string str){
    return Bytearray(basic_from_oct(str));
  }
}; 
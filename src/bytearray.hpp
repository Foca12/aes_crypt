#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
 
typedef std::vector<int> ilist;
typedef std::vector<int>::iterator ilist_iterator;
using std::string, std::hex;

class Bytearray{
  ilist bytes = {};
  public:
  // costruttori
  Bytearray(int x=0){
    this->bytes.insert(this->bytes.end(), x, 0);
  }
  Bytearray(string str){
    for (int i : str){
      this->bytes.push_back(i);
    }
  }
  Bytearray(ilist str){
    for (int i : str){
      this->bytes.push_back(i);
    }
  }

  // metodi vettori
  void push_back(string str){
    this->bytes.push_back((int)((char)str[0]));
  }
  void push_back(Bytearray str){
    this->bytes.insert(this->bytes.end(), str.bytes.begin(), str.bytes.end());
  }
  void push_back(char str){
  this->bytes.push_back((int)str);
  }
  void push_back(int n){
    this->bytes.push_back(n);
  }
  
  void extend(string str){
    for (int i : str){
      this->bytes.push_back(i);
    }
  }
  void extend(Bytearray str){
    for (int i : str){
      this->bytes.push_back(i);
    }
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
  
  ilist_iterator begin(){
    return this->bytes.begin();
  }
  ilist_iterator end(){
    return this->bytes.end();
  }
  
  void operator=(Bytearray x){
    this->bytes = x.bytes;
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
  void operator<<=(int rounds){
    this->bytes = (ilist)(this->operator<<(rounds));
  }
  void operator>>=(int rounds){
    this->bytes = (ilist)(this->operator>>(rounds));
  }
  
  // operatori logici
  Bytearray operator|(Bytearray arr){
    Bytearray xored;

    if (this->length() == 0){
      return xored;
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

    for (int i = 0; i < this->length(); i++){
      int xored_char = this->bytes[i] ^ arr[i % arr.length()];
      xored.push_back(xored_char);
    }
    return xored;
    }

  // conversioni di tipo
  operator string(){
    std::stringstream ss;
    string str;
    bool only_valid = true;

    for (int i : this->bytes) {
      if (i < 32 || i > 126){
        only_valid = false;
      }
    }

    for (int i : this->bytes) {
      if (only_valid){
        str += (char) i;
      }
      else {
        ss << "\\x";
        ss << std::hex << std::setw(2) << std::setfill('0') << i;
      }
    }

    return only_valid? str : ss.str();
  }
  operator ilist(){
    return this->bytes;
  }

  // conversioni di formato
  string hex(){
    std::stringstream ss;
    
    for (int i : this->bytes) {
      ss << std::hex << std::setw(2) << std::setfill('0') << i;
    }

    return ss.str();
  }
  string oct(){
    std::stringstream ss;
    
    for (int i : this->bytes) {
      ss << std::oct << std::setw(3) << std::setfill('0') << i;
    }

    return ss.str();
  }

  // costruttori alternativi
  static Bytearray from_hex(string str){
    ilist vct;

    auto hex_to_int = [](string s) -> int{
      int t = 0;
      for (int n = 0; n < s.length(); n++){
        int current;
        if (s[n] >= 'a' && s[n] <= 'f'){
          current = s[n] - 'a' + 10;
        }
        if (s[n] >= '0' && s[n] <= '9'){
          current = s[n] - '0';
        }
        t += current * pow(16, s.length()-n-1);
      }
      return t;
    };

    for (int i = 0; i < str.length(); i += 2){
      vct.push_back(hex_to_int({str[i], str[i+1]}));
    }
    return Bytearray(vct);
  }
  static Bytearray from_oct(string str){
    ilist vct;

    auto oct_to_int = [](string s) -> int{
      int t = 0;
      for (int n = 0; n < s.length(); n++){
        int current = s[n] - '0';
        t += current * pow(8, s.length()-n-1);
      }
      return t;
    };

    for (int n = 0; n < str.length(); n += 3){
      vct.push_back(oct_to_int({str[n], str[n+1], str[n+2]}));
    }

    return Bytearray(vct);
  }
};
#include <string>
#include "types.hpp"
#include "constants.hpp"

template <typename T>
class ByteCollection{
  public:

  virtual ~ByteCollection() = default;

  virtual std::string hex() = 0;
  virtual std::string oct() = 0;
  virtual T from_hex(std::string str) = 0;
  virtual T from_oct(std::string str) = 0;

  virtual operator std::string() = 0;

  protected:
  std::string convert_to_string(std::vector<int> vct){
    std::stringstream ss;
    std::string str;
    bool only_valid = true;
    
    for (int i : vct) {
      if ((i < 32 || i > 126) && i != 0){
        only_valid = false;
        }
      }

      for (int i : vct) {
        if (only_valid && i != 0){
          str += (char) i;
        }
        else {
          ss << "\\x";
          ss << std::setfill('0') << std::setw(2) << std::hex << i;
        }
      }

      return only_valid? str : ss.str();
    }
  std::string convert_to_string(int* arr, int size){
      types::ilist vct;
      vct.assign(arr, arr+size);
      return convert_to_string(vct);
    }

  types::ilist basic_from_hex(std::string str){
    types::ilist vct;
    auto hex_to_int = [](std::string s) -> int{
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
    return vct;
    }
  types::ilist basic_from_oct(std::string str){
    types::ilist vct;
    auto oct_to_int = [](std::string s) -> int{
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
    return vct;
  }

  std::string hex(types::ilist bytes){
    std::stringstream ss;
    
    for (int i : bytes) {
      ss << std::hex << std::setw(2) << std::setfill('0') << i;
    }

    return ss.str();
  }
  std::string oct(types::ilist bytes){
    std::stringstream ss;
    
    for (int i : bytes) {
      ss << std::oct << std::setw(3) << std::setfill('0') << i;
    }

    return ss.str();
  }
};
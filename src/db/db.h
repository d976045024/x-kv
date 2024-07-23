#pragma once

#include <string>

namespace xkv {

struct Range {
  std::string start;
  std::string limit;

  Range() = default;
  Range(const std::string &s, std::string &l) : start(s), limit(l) {} 
};

class DB {
 public:
   
};

}
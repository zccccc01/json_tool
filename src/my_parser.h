#pragma once
#include <iostream>

#include "json.h"
namespace cl {
namespace json {
class Parser {
 public:
  Parser();
  void Load(const std::string& str);
  Json Parse();

 private:
  void SkipWhiteSpace();
  char GetNextToken();
  Json ParseNull();
  Json ParseBool();
  Json ParseNumber();
  std::string ParseString();
  Json ParseArray();
  Json ParseObject();
 private:
  std::string str_;
  int index_;
};

}  // namespace json
}  // namespace cl
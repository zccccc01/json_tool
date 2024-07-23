#include <fstream>
#include <iostream>

#include "json.h"
#include "my_parser.h"
using namespace cl::json;

void test() {
  std::ifstream fin("./my.json");
  std::stringstream ss;
  ss << fin.rdbuf();
  const std::string& str = ss.str();
  Json j;
  j.Parse(str);
  // std::string city = j["address"]["city"];// shanghai
  // int age = j["age"];// 30
  std::cout << j.Str() << std::endl;
}

int main() {
  test();
  return 0;
}

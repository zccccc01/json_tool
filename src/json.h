#pragma once
#include <map>
#include <sstream>
#include <stdexcept>  // Å×³öÒì³£
#include <string>
#include <vector>

namespace cl {
namespace json {
class Json {
 public:
  enum Type {
    json_null = 0,
    json_bool,
    json_int,
    json_double,
    json_string,
    json_array,
    json_object
  };
  Json();
  Json(bool value);
  Json(int value);
  Json(double value);
  Json(const char* value);
  Json(const std::string& value);
  Json(Type type);
  Json(const Json& other);
  ~Json();
  operator bool();
  operator int();
  operator double();
  operator std::string();
  Json& operator[](int index);
  Json& operator[](const char* key);
  Json& operator[](const std::string& key);
  void operator=(const Json& other);
  bool operator==(const Json& other);
  bool operator!=(const Json& other);
  void Append(const Json& other);
  std::string Str() const;
  void Copy(const Json& other);
  typedef std::vector<Json>::iterator iterator;
  iterator begin() { return value_.array_->begin(); }
  iterator end() { return value_.array_->end(); }
  bool IsNull() const;
  bool IsBool() const;
  bool IsInt() const;
  bool IsDouble() const;
  bool IsString() const;
  bool IsArray() const;
  bool IsObject() const;
  bool AsBool() const;
  int AsInt() const;
  double AsDouble() const;
  std::string AsString() const;
  bool Has(int index);
  bool Has(const char* key);
  bool Has(const std::string& key);
  void Remove(int index);
  void Remove(const char* key);
  void Remove(const std::string& key);
  void Parse(const std::string& str);

 private:
  union Value {
    bool bool_;
    int int_;
    double double_;
    std::string* string_;
    std::vector<Json>* array_;
    std::map<std::string, Json>* object_;
  };
  Type type_;    // key
  Value value_;  // value
};
}  // namespace json
}  // namespace cl
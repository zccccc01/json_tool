#include "json.h"
#include "my_parser.h"
using namespace cl::json;

Json::Json() : type_(json_null) {}

Json::Json(bool value) : type_(json_bool) { value_.bool_ = value; }

Json::Json(int value) : type_(json_int) { value_.int_ = value; }

Json::Json(double value) : type_(json_double) { value_.double_ = value; }

Json::Json(const char* value) : type_(json_string) {
  value_.string_ = new std::string(value);
}

Json::Json(const std::string& value) : type_(json_string) {
  value_.string_ = new std::string(value);
}

Json::Json(Type type) : type_(type) {
  switch (type_) {
    case json_null:
      break;
    case json_bool:
      value_.bool_ = false;
      break;
    case json_int:
      value_.int_ = 0;
      break;
    case json_double:
      value_.double_ = 0.0;
      break;
    case json_string:
      value_.string_ = new std::string("");
      break;
    case json_array:
      value_.array_ = new std::vector<Json>();
      break;
    case json_object:
      value_.object_ = new std::map<std::string, Json>();
      break;
    default:
      break;
  }
}

Json::Json(const Json& other) { Copy(other); }

void Json::operator=(const Json& other) { Copy(other); }

void Json::Copy(const Json& other) {
  type_ = other.type_;
  switch (type_) {
    case json_null:
      break;
    case json_bool:
      value_.bool_ = other.value_.bool_;
      break;
    case json_int:
      value_.int_ = other.value_.int_;
      break;
    case json_double:
      value_.double_ = other.value_.double_;
      break;
    case json_string:
      value_.string_ = new std::string(*other.value_.string_);
      break;
    case json_array:
      value_.array_ = new std::vector<Json>(*other.value_.array_);
      break;
    case json_object:
      value_.object_ = new std::map<std::string, Json>(*other.value_.object_);
      break;
    default:
      break;
  }
}

bool Json::operator==(const Json& other) {
  if (type_ != other.type_) {
    return false;
  }
  switch (type_) {
    case json_null:
      return true;
    case json_bool:
      return value_.bool_ == other.value_.bool_;
    case json_int:
      return value_.int_ == other.value_.int_;
    case json_double:
      return value_.double_ == other.value_.double_;
    case json_string:
      return *(value_.string_) == *(other.value_.string_);
    case json_array: {
      bool ans = true;
      size_t size1 = value_.array_->size();
      size_t size2 = other.value_.array_->size();
      if (size1 != size2) {
        ans = false;
      }
      size_t minSize = std::min(size1, size2);
      for (size_t i = 0; i < minSize; ++i) {
        auto& elem1 = (*value_.array_)[i];
        auto& elem2 = (*other.value_.array_)[i];
        if (elem1 != elem2) {
          ans = false;
          break;
        }
      }
      return ans;
    }
    case json_object: {
      bool ans = value_.object_->size() == other.value_.object_->size();
      if (ans) {
        for (const auto& pair : *value_.object_) {
          auto it = other.value_.object_->find(pair.first);
          if (it == other.value_.object_->end() || it->second != pair.second) {
            ans = false;
            break;
          }
        }
      }
      return ans;
    }
    default:
      break;
  }
  return false;
}

bool Json::operator!=(const Json& other) { return !((*this) == other); }

Json::~Json() {
  if (type_ == json_string && value_.string_ != NULL) {
    delete value_.string_;
    value_.string_ = NULL;
  }
  if (type_ == json_array && value_.array_ != NULL) {
    delete value_.array_;
    value_.array_ = NULL;
  }
  if (type_ == json_object && value_.object_ != NULL) {
    delete value_.object_;
    value_.object_ = NULL;
  }
}

Json::operator bool() {
  if (type_ != json_bool) {
    throw std::logic_error("type error, not bool value");
  }
  return value_.bool_;
}

Json::operator int() {
  if (type_ != json_int) {
    throw std::logic_error("type error, not int value");
  }
  return value_.int_;
}

Json::operator double() {
  if (type_ != json_double) {
    throw std::logic_error("type error, not double value");
  }
  return value_.double_;
}

Json::operator std::string() {
  if (type_ != json_string) {
    throw std::logic_error("type error, not string value");
  }
  return *value_.string_;
}

Json& Json::operator[](int index) {
  if (type_ != json_array) {
    type_ = json_array;
    value_.array_ = new std::vector<Json>();
  }
  if (index < 0) {
    throw std::logic_error("array[] index < 0");
  }
  int size = (value_.array_)->size();
  // 扩容,往数组添加空的json
  if (index >= size) {
    (value_.array_)->resize(index + 1);
  }
  return (value_.array_)->at(index);
}

void Json::Append(const Json& other) {
  if (type_ != json_array) {
    type_ = json_array;
    value_.array_ = new std::vector<Json>();
  }
  (value_.array_)->push_back(other);
}

std::string Json::Str() const {
  std::stringstream ss;
  switch (type_) {
    case json_null:
      ss << "null";
      break;
    case json_bool:
      if (value_.bool_) {
        ss << "true";
      } else {
        ss << "false";
      }
      break;
    case json_int:
      ss << value_.int_;
      break;
    case json_double:
      ss << value_.double_;
      break;
    case json_string:
      ss << '\"' << *value_.string_ << '\"';
      break;
    case json_array: {
      ss << '[';
      for (auto it = (value_.array_)->begin(); it != (value_.array_)->end();
           it++) {
        if (it != (value_.array_)->begin()) {
          ss << ',';
        }
        ss << it->Str();  // 递归调用,读出每个
      }
      ss << ']';
      break;
    }
    case json_object: {
      ss << '{';
      for (auto it = (value_.object_)->begin(); it != (value_.object_)->end();
           it++) {
        if (it != (value_.object_)->begin()) {
          ss << ',';
        }
        ss << '\"' << it->first << '\"' << ':' << it->second.Str();
      }
      ss << '}';
      break;
    }
    default:
      break;
  }
  return ss.str();
}

Json& Json::operator[](const char* key) {
  std::string name(key);
  return (*(this))[name];
}

Json& Json::operator[](const std::string& key) {
  if (type_ != json_object) {
    type_ = json_object;
    value_.object_ = new std::map<std::string, Json>();
  }
  return (*(value_.object_))[key];
}

bool Json::IsNull() const { return type_ == json_null; }

bool Json::IsBool() const { return type_ == json_bool; }

bool Json::IsInt() const { return type_ == json_int; }

bool Json::IsDouble() const { return type_ == json_double; }

bool Json::IsString() const { return type_ == json_string; }

bool Json::IsArray() const { return type_ == json_array; }

bool Json::IsObject() const { return type_ == json_object; }

bool Json::AsBool() const {
  if (type_ != json_bool) {
    throw std::logic_error("type error, not bool value");
  }
  return value_.bool_;
}

int Json::AsInt() const {
  if (type_ != json_int) {
    throw std::logic_error("type error, not int value");
  }
  return value_.int_;
}

double Json::AsDouble() const {
  if (type_ != json_double) {
    throw std::logic_error("type error, not double value");
  }
  return value_.double_;
}

std::string Json::AsString() const {
  if (type_ != json_string) {
    throw std::logic_error("type error, not string value");
  }
  return *(value_.string_);
}

bool Json::Has(int index) {
  if (type_ != json_array) {
    return false;
  }
  int size = (value_.array_)->size();
  return (index >= 0 && index < size);
}

bool Json::Has(const char* key) {
  std::string name(key);
  return Has(name);
}

bool Json::Has(const std::string& key) {
  if (type_ != json_object) {
    return false;
  }
  return (value_.object_)->find(key) != (value_.object_)->end();
}

void Json::Remove(int index) {
  if (Has(index)) {
    (value_.array_)->erase(value_.array_->begin() + index);
  }
}

void Json::Remove(const char* key) {
  std::string name(key);
  Remove(name);
}

void Json::Remove(const std::string& key) {
  if (Has(key)) {
    (value_.object_)->erase(key);
  }
}

void Json::Parse(const std::string& str) {
  Parser p;
  p.Load(str);
  *this = p.Parse();
}

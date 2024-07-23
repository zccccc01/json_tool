#include "my_parser.h"
using namespace cl::json;

Parser::Parser() : str_(""), index_(0) {}

void Parser::Load(const std::string& str) {
  str_ = str;
  index_ = 0;
}

void Parser::SkipWhiteSpace() {
  while (str_[index_] == ' ' || str_[index_] == '\n' || str_[index_] == '\r' ||
         str_[index_] == '\t') {
    index_++;
  }
}

char Parser::GetNextToken() {
  SkipWhiteSpace();
  return str_[index_++];
}

Json Parser::Parse() {
  char ch = GetNextToken();

  switch (ch) {
    case 'n':
      index_--;
      return ParseNull();
    case 't':
    case 'f':
      index_--;
      return ParseBool();
    case '-':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      index_--;
      return ParseNumber();
    case '"':
      return Json(ParseString());
    case '[':
      index_--;
      return ParseArray();
    case '{':
      return ParseObject();
    default:
      break;
  }
  throw std::logic_error("unexpected char");
}

Json Parser::ParseNull() {
  if (str_.compare(index_, 4, "null") == 0) {
    index_ += 4;
    return Json();
  }
  throw std::logic_error("parse null error");
}

Json Parser::ParseBool() {
  if (str_.compare(index_, 4, "true") == 0) {
    index_ += 4;
    return Json(true);
  } else if (str_.compare(index_, 5, "false") == 0) {
    index_ += 5;
    return Json(false);
  }
  throw std::logic_error("parse bool error");
}

Json Parser::ParseNumber() {
  int pos = index_;
  if (str_[index_] == '-') {
    index_++;
  }
  if (str_[index_] < '0' || str_[index_] > '9') {
    throw std::logic_error("parse number error");
  }
  while (str_[index_] >= '0' && str_[index_] <= '9') {
    index_++;
  }
  if (str_[index_] != '.') {
    int i = std::atoi(str_.c_str() + pos);
    return Json(i);
  }
  index_++;
  if (str_[index_] < '0' || str_[index_] > '9') {
    throw std::logic_error("parse number error");
  }
  while (str_[index_] >= '0' && str_[index_] <= '9') {
    index_++;
  }
  double f = std::atof(str_.c_str() + pos);
  return Json(f);
}

std::string Parser::ParseString() {
  std::string out;
  while (true) {
    char ch = str_[index_++];
    if (ch == '"') {
      break;
    }
    if (ch == '\\') {
      ch = str_[index_++];
      switch (ch) {
        case '\n':
          out += '\n';
          break;
        case '\r':
          out += '\r';
          break;
        case '\t':
          out += '\t';
          break;
        case '\b':
          out += '\b';
          break;
        case '\f':
          out += '\f';
          break;
        case '"':
          out += "\\\"";
          break;
        case '\\':
          out += "\\\\";
          break;
        case 'u':
          out += "\\u";
          for (int i = 0; i < 4; i++) {
            out += str_[index_++];
          }
          break;
        default:
          break;
      }
    } else {
      out += ch;
    }
  }
  return out;
}

Json Parser::ParseArray() {
  Json arr(Json::json_array);
  char ch = GetNextToken();
  if (ch == ']') {
    return arr;
  }
  while (true) {
    arr.Append(Parse());
    ch = GetNextToken();
    if (ch == ']') {
      break;
    }
    if (ch != ',') {
      throw std::logic_error("parse array error");
    }
    index_++;
  }
  return arr;
}

Json Parser::ParseObject() {
  Json obj(Json::json_object);
  char ch = GetNextToken();
  if (ch == '}') {
    return obj;
  }
  index_--;
  while (true) {
    ch = GetNextToken();
    if (ch != '"') {
      throw std::logic_error("parse object error");
    }
    std::string key = ParseString();
    ch = GetNextToken();
    if (ch != ':') {
      throw std::logic_error("parse object error");
    }
    obj[key] = Parse();
    ch = GetNextToken();
    if (ch == '}') {
      break;
    }
    if (ch != ',') {
      throw std::logic_error("parse object error");
    }
    index_++;
  }
  return obj;
}
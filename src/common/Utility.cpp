#include "Utility.hpp"

#include <iostream>
#include <sstream>

std::string trim(const std::string &str) {
  size_t first = str.find_first_not_of(' ');
  if (std::string::npos == first) {
    return str;
  }
  size_t last = str.find_last_not_of(' ');
  return str.substr(first, (last - first + 1));
}

std::vector<std::string> tokenize(const std::string &str, const char delim) {
  std::stringstream ss(str);
  std::vector<std::string> tokens;
  std::string temp;
  while (std::getline(ss, temp, ',')) {
    tokens.push_back(temp);
  }

  return tokens;
}

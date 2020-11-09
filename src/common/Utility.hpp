#pragma once

#include <string>
#include <vector>

std::string trim(const std::string &str);
std::vector<std::string> tokenize(const std::string &str,
                                  const char delim = ',');

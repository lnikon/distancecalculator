#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <istream>
#include <string>
#include <vector>

/**
 * @brief Returns trimmed copy of a string
 */
std::string trim(const std::string& str);

/**
 * @brief Tokenizes string by a given delimeter and returns vector of string tokens
 */
std::vector<std::string> tokenize(const std::string& str, const char delim = ',');

/**
 * @brief Counts number of lines in csv via std::istream
 */
size_t countLines(std::istream& is);

/**
 * @brief Counts number of columns in csv via std::istream
 */
size_t countColumns(std::istream& is);

#endif

#include "Utility.hpp"

#include <algorithm>
#include <iostream>
#include <istream>
#include <sstream>

std::string trim(const std::string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first)
    {
        return str;
    }

    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

std::vector<std::string> tokenize(const std::string& str, const char delim)
{
    std::stringstream        ss(str);
    std::vector<std::string> tokens;
    std::string              temp;
    while (std::getline(ss, temp, ','))
    {
        tokens.push_back(temp);
    }


    return tokens;
}

size_t countLines(std::istream& is)
{
    // Skip when bad
    if (is.bad())
    {
        return 0;
    }

    std::size_t lineCount{0};
    std::string line;
    while (std::getline(is, line))
    {
        if (trim(line).empty())
        {
            continue;
        }

        lineCount++;
    }

    is.clear();
    is.seekg(0, std::ios::beg);

    return lineCount;
}

size_t countColumns(std::istream& is)
{
    // Skip when bad
    if (is.bad())
    {
        return 0;
    }

    std::size_t columnCount{0};
    std::string line;
    if (std::getline(is, line))
    {
        std::size_t commaCount = std::count(std::begin(line), std::end(line), ',');
        if (commaCount > 0)
        {
            columnCount = commaCount + 1;
        }
    }

    is.clear();
    is.seekg(0, std::ios::beg);

    return columnCount;
}

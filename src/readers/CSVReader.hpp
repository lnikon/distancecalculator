#pragma once

#include "CSVContainer.hpp"
#include "ICSVReader.hpp"
#include "Path.hpp"
#include "Utility.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>

namespace readers
{

template <typename ValueType>
class CSVReader final : public ICSVReader<ValueType>
{
public:
    structures::CSVContainerSPtr<ValueType> read(const Path& path) const override;
};

template <typename ValueType>
structures::CSVContainerSPtr<ValueType> CSVReader<ValueType>::read(const Path& path) const
{
    if (path.empty())
    {
        // TODO: throw exception
        return nullptr;
    }

    auto csvContainer = std::make_shared<structures::CSVContainer<ValueType>>();
    std::fstream inStream(path);
    if (!inStream.is_open())
    {
        // TODO: throw exception
        return nullptr;
    }

    std::string line;
    while (std::getline(inStream, line))
    {
        line = trim(line);
        if (line.empty())
        {
            continue;
        }

        auto tokens = tokenize(line);
        auto row = typename structures::CSVContainer<ValueType>::Row{};
        auto columnNumber = std::size_t{};
        for (const auto& token : tokens)
        {
            std::stringstream ss(token);
            ValueType value;
            ss >> value;
            if (ss.fail())
            {
                std::runtime_error e("Unable to parse token: \"" + token + "\" at line " +
                                     std::to_string(csvContainer->rowCount()) + ", column " +
                                     std::to_string(row.size()));
                throw e;
            }

            row.emplace_back(std::move(value));
        }

        csvContainer->append(row);
    }

    return csvContainer;
}

} // namespace readers

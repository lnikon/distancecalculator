#pragma once

#include "CSVContainer.hpp"
#include "ICSVReader.hpp"
#include "Path.hpp"
#include "Utility.hpp"

#include <filesystem>
#include <fstream>

namespace readers
{

template <typename ValueType> class CSVReader final : public ICSVReader<ValueType>
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
        typename structures::CSVContainer<ValueType>::Row row;
        for (const auto& token : tokens)
        {
            row.push_back(atof(token.data()));
        }

        csvContainer->append(row);
    }

    return csvContainer;
}

} // namespace readers

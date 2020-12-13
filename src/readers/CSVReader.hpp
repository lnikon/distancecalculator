#pragma once

#include "CSVContainer.hpp"
#include "ICSVReader.hpp"
#include "Path.hpp"
#include "Utility.hpp"

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
        throw std::runtime_error("can\'t read when path is empty");
    }

    std::fstream inStream(path);
    if (!inStream.is_open())
    {
        std::string msg("can\'t open ");
        msg += path;
        msg += std::string(" to read\n");
        throw std::runtime_error(msg.data());
    }

    // Calculate number of rows
    const std::size_t rowCount    = countLines(inStream);
    const std::size_t columnCount = countColumns(inStream);
    assert(columnCount > 0);

    auto csvContainer = std::make_shared<structures::CSVContainer<ValueType>>();
    csvContainer->resize(rowCount, columnCount);

    std::string line;
	std::size_t lineNumber{0};
    while (std::getline(inStream, line))
    {
        line = trim(line);
        if (line.empty())
        {
            continue;
        }


        auto tokens = tokenize(line);
        auto columnNumber = std::size_t{};
        for (const auto& token : tokens)
        {
            std::stringstream ss(token);
            ValueType         value;
            ss >> value;
            if (ss.fail())
            {
                std::runtime_error e("Unable to parse token: \"" + token + "\" at line " +
                                     std::to_string(csvContainer->rowCount()) + ", column " +
                                     std::to_string(csvContainer->columnCount()));
                throw e;
            }

            csvContainer->set(lineNumber, columnNumber, std::move(value));
			columnNumber++;
        }
		
		lineNumber++;
    }

    return csvContainer;
}

} // namespace readers

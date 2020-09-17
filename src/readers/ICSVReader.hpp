#pragma once

#include "CSVContainer.hpp"
#include "Path.hpp"

namespace readers
{

template <typename ValueType>
class ICSVReader
{
public:
    virtual structures::CSVContainerSPtr<ValueType> read(const Path& path) const = 0;
};

} // namespace readers

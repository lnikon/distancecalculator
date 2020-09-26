#include "CSVReader.hpp"
#include "DistanceCalculator.hpp"

#include <iostream>
#include <optional>
#include <thread>
#include <vector>

/*
 * Initially distance calculation will be possible only with one engine
 */
int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cerr << "usage: disc /path/to/query.csv /path/to/dataset.csv\n";
        return 1;
    }

    const std::string queryPath   = argv[1];
    const std::string datasetPath = argv[2];

    std::cout << "Query as: " << queryPath << "\n";
    std::cout << "Dataset as: " << datasetPath << "\n";

    auto csvReader = std::make_unique<readers::CSVReader<float>>();
    auto query     = csvReader->read(queryPath);
    auto dataset   = csvReader->read(datasetPath);

    DistanceCalculator<float> dc;
    dc.setDistanceCalculatorEngineType(DistanceCalculatorEngineType::CPPThreads);
    auto parallel = dc.calculate(query, dataset);
    dc.setDistanceCalculatorEngineType(DistanceCalculatorEngineType::Sequential);
    auto sequential = dc.calculate(query, dataset);

    const auto rowCount    = parallel->rowCount();
    const auto columnCount = parallel->columnCount();
    for (std::size_t rowIdx = 0; rowIdx < rowCount; rowIdx++)
    {
        for (std::size_t colIdx = 0; colIdx < columnCount; colIdx++)
        {
            const auto lhs = (*parallel->get(rowIdx))[colIdx];
            const auto rhs = (*sequential->get(rowIdx))[colIdx];
            if (lhs != rhs)
            {
                std::cout << "FAIL: Mismatch at (" << rowIdx << colIdx << "): " << lhs
                          << " != " << rhs << "\n";
                return 1;
            }
        }
    }

    std::cout << "PASS\n";

    return 0;
}

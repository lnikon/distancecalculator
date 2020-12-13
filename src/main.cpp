#include "CSVReader.hpp"
#include "DistanceCalculator.hpp"

#include <cassert>
#include <chrono>
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

    DistanceCalculator<float, MetricKind::L1Metric> dc;
    dc.setDistanceCalculatorEngineKind(DistanceCalculatorEngineKind::CUDA);

    auto start    = std::chrono::steady_clock::now();
    auto parallel = dc.calculate(query, dataset);
    assert(parallel != nullptr);
    auto end = std::chrono::steady_clock::now();

    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Elapsed time cuda: " << elapsed_seconds.count() << "s\n";

    dc.setDistanceCalculatorEngineKind(DistanceCalculatorEngineKind::CPPThreads);

    start           = std::chrono::steady_clock::now();
    auto sequential = dc.calculate(query, dataset);
    assert(sequential != nullptr);
    end = std::chrono::steady_clock::now();

    elapsed_seconds = end - start;
    std::cout << "Elapsed time thread pool: " << elapsed_seconds.count() << "s\n";

    assert(parallel->rowCount() == sequential->rowCount());
    assert(parallel->rowCount() == sequential->rowCount());
    assert(parallel->columnCount() == sequential->columnCount());

    /* Verification of a selected calculation engine with the naive one */
    const auto rowCount    = parallel->rowCount();
    const auto columnCount = parallel->columnCount();
    for (std::size_t rowIdx = 0; rowIdx < rowCount; rowIdx++)
    {
        for (std::size_t colIdx = 0; colIdx < columnCount; colIdx++)
        {
            const auto lhs = parallel->at(rowIdx, colIdx);
            const auto rhs = sequential->at(rowIdx, colIdx);
            if (lhs != rhs)
            {
                std::cout << "FAIL: Mismatch at (" << rowIdx << ", " << colIdx << "): " << lhs
                          << " != " << rhs << "\n";
                std::cout << "Verification FAIL\n";
                return 1;
            }
        }
    }

    std::cout << "Verification PASS\n";

    return 0;
}

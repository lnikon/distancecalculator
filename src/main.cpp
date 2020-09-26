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
    dc.calculate(query, dataset);

    return 0;
}

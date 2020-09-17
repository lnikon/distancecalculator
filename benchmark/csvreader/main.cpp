
#include "CSVReader.hpp"

#include <benchmark/benchmark.h>

#include <memory>

static void BM_StringCreation(benchmark::State& state)
{
    for (auto _ : state)
    {
        auto csvReader = std::make_unique<readers::CSVReader<float>>();
        auto container = csvReader->read(
            Path("/home/nikon/projects/distancecalculator/test/data/csv/test_data.csv"));
    }
}
BENCHMARK(BM_StringCreation);

BENCHMARK_MAIN();
